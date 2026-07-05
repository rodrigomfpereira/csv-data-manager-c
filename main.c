#include <stdio.h>  //input/output
#include <stdlib.h> //para o system()
#include <unistd.h> //para o getpid
#include <string.h> //manipulação de strings
#include <sys/types.h> //para o tipo pid_t
#include <sys/wait.h> // para o wait()
#include <signal.h> // para os sinais
#include <pthread.h> //manipulação de threads

//variáveis globais
pid_t pidA = 0, pidB = 0, pidC = 0; //ficha 3
int sinal_recebido = 0; //ficha 3
char g_nome_ficheiro[100]; //ficha 4
int g_colunas[3]; // g_colunas[0] para Thread 1, g_colunas[1] para Thread 2, etc. ficha 4
int g_repeticoes; //ficha 4
pthread_mutex_t g_mutex; //ficha 4, opção C

//para a opção A
void listarProcessos() {
    int opcao;
    char filtro[50], comando[100];
    printf("\nSUBMENU - LISTAGEM DE PROCESSOS\n");
    printf("\n1. Listagem (ps aux)\n");
    printf("2. Listagem com parametros (ps)\n");
    printf("3. Listagem à escolha do utilizador (ps aux | grep)\n");
    printf("4. Listagem em tempo real com destruição de processo (top)\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    switch(opcao) {
        case 1:
            system("ps aux");
            break;
        case 2:
            printf("Introduza os parametros: ");
            scanf("%s", filtro);
            strcpy(comando, "ps ");
            strcat(comando, filtro);
            system(comando);
            break;
        case 3:
            printf("Processo a procurar: ");
            scanf("%s", filtro);
            strcpy(comando, "ps aux | grep ");
            strcat(comando, filtro);
            system(comando);
            break;
        case 4:
            printf("A encerrar programa e executar top.\n");
            execlp("top","top", NULL);
            break;
        default:
            printf("Opcao inexistente.\n");
    }
}

//para a opção B
void consultarPIDs() {
    int opcao;
    printf("\nSUBMENU - CONSULTAR PIDs\n");
    printf("\n1. PID do processo atual\n");
    printf("2. PID do processo pai\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("\nPID Atual: %d\n", getpid());
    } else if (opcao == 2) {
        printf("\nPID Pai: %d\n", getppid());
    } else {
        printf("Opcao inexistente.\n");
    }
}

//para a opção C
void estatisticaManual() {
    char nomeFich[100], comando[100], buffer[100], temCabecalho;
    int colunaEscolhida, numDados = 0;
    double valor, soma = 0, somaQuadrados = 0;

    printf("\nESTATISTICA MANUAL\n");
    printf("\nInsira o nome do ficheiro: ");
    scanf("%s", nomeFich);
    printf("\nInsira o nº da coluna a retirar: ");
    scanf("%d", &colunaEscolhida);
    printf("O ficheiro tem cabecalho (s/n):");
    scanf(" %c", &temCabecalho);

    //Uso o cut tal como na ficha 6 para extrair a coluna
    if (temCabecalho == 's' || temCabecalho == 'S') {
        sprintf(comando, "cut -d',' -f%d %s | tail -n +2", colunaEscolhida, nomeFich);
    } else {
        sprintf(comando, "cut -d',' -f%d %s",colunaEscolhida, nomeFich);
    }

    //uso uma função que faz um pipe entre o comando e o programa que o está a executar
    //permite ler sem guardar em um ficheiro temporário (devolve um ponteiro ou null)
    FILE *pipe = popen(comando, "r");

    if (pipe == NULL) {
        printf("Erro a extrair do ficheiro.\n");
        return;
    }

    //ler a saída do cut linha a linha diretamente da memória
    while (fgets(buffer, sizeof(buffer), pipe)) {
        valor = atof(buffer);
        
        soma += valor;
        somaQuadrados += (valor * valor);
        numDados++;
    }

    pclose(pipe);

    if (numDados > 0) {
        double media = soma / numDados;
        double variancia = (somaQuadrados / numDados) - (media * media);


        printf("\nRESULTADOS\n");
        printf("\nMedia:                %.2f", media);
        printf("\nVariancia:            %.2f\n", variancia);
    } else {
        printf("\nNao foi encontrado nenhum dado numerico.\n");
    }
}

//para a opção D
void estatisticaAwk() {
    char nomeFich[100], comando[300], temCabecalho;
    int colunaEscolhida;

    printf("\nESTATISTICA COM AWK\n");
    printf("\nInsira o nome do ficheiro: ");
    scanf("%s", nomeFich);
    printf("\nInsira o nº da coluna a retirar: ");
    scanf("%d", &colunaEscolhida);
    printf("O ficheiro tem cabecalho (s/n): ");
    scanf(" %c", &temCabecalho);

    printf("\nRESULTADOS (AWK)\n");

    if (temCabecalho == 's' || temCabecalho == 'S') {
        // Usa NR>1 para ignorar a primeira linha 
        sprintf(comando, "awk -F',' 'NR>1 {soma+=$%d; somaQuadrados+=$%d*$%d; numDados++} END {print \"Media:\", soma/numDados; print \"Variancia:\", (somaQuadrados/numDados)-(soma/numDados)^2}' %s", 
                colunaEscolhida, colunaEscolhida, colunaEscolhida, nomeFich);
    } else {
        sprintf(comando, "awk -F',' '{soma+=$%d; somaQuadrados+=$%d*$%d; numDados++} END {print \"Media:\", soma/numDados; print \"Variancia:\", (somaQuadrados/numDados)-(soma/numDados)^2}' %s", 
                colunaEscolhida, colunaEscolhida, colunaEscolhida, nomeFich);
    }

    system(comando); 
}

void menuFicha1() {
    char opcao;

    do {
        printf("\nMENU FICHA 1\n");
        printf("\na) Listagem de Processos\n");
        printf("b) Consultar PIDs\n");
        printf("c) Estatistica Descritiva (Média e Variância)\n");
        printf("d) Estatistica Descritiva (Média e Variância) com awk\n");
        printf("e) Sair (Voltar ao menu principal)\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &opcao);

        switch(opcao) {
            case 'a': listarProcessos(); break;
            case 'b': consultarPIDs(); break;
            case 'c': estatisticaManual(); break;
            case 'd': estatisticaAwk(); break;
            case 'e': printf("A voltar ao menu principal..\n"); break;
            default: printf("Opcao inexistente.\n");
        }
    } while (opcao != 'e');

}

void monitorizarRecursos() {

    printf("\n[PID: %d] Monitorização de Recursos\n", getpid());

    pid_t pid = fork();    
    if(pid < 0) {
        printf("\n[PID: %d] Erro na criação de processo filho (fork).\n", getpid());
        return;
    } else if(pid == 0) {
        //filho
        system ("ps aux > log_processos.txt");
        printf("\n[PID: %d] O estado dos processos foi escrito no ficheiro 'log_processos.txt' a partir do processo filho.\n", getpid());
        exit(0);
    } else {
        //pai
        wait(NULL);
    }
}

void processarDados() {
    char nomeFich[100], comando[100];
    int colunaEscolhida;

    printf("\n[PID: %d] Processamento de Dados com Filtros\n", getpid());

    printf("\nInsira o nome do ficheiro: ");
    scanf("%s", nomeFich);
    printf("\nInsira o nº da coluna a retirar: ");
    scanf("%d", &colunaEscolhida);
    
    pid_t pid1 = fork();

    if(pid1 < 0) {
        printf("\n[PID: %d] Erro na criação do primeiro processo filho (fork).\n", getpid());
        return;
        
    //filho 1
    } else if(pid1 == 0) {
        sprintf(comando, "cut -d',' -f%d %s | tail -n +2 | sort -nu | tail -n 1 > resultados_estat.dat", colunaEscolhida, nomeFich);
        system(comando);
        printf("\n[PID: %d] O nº máximo da coluna nº %d no ficheiro '%s' guardado com sucesso em resultados_estat.dat a partir do 1º processo filho.\n", getpid(), colunaEscolhida, nomeFich);
        exit(0);
    } else {
        //pai
        pid_t pid2 = fork();

        if(pid2 < 0) {
            printf("\n[PID: %d] Erro na criação do segundo processo filho (fork).\n", getpid());
            wait(NULL); //esperar pelo filho 1
            return;

        //filho 2
      } else if (pid2 == 0) {
            printf("\n[PID: %d] A executar gnuplot...\n", getpid());
            sprintf(comando, "gnuplot -e \"plot '< cut -d, -f%d %s | tail -n +2' with lines; pause mouse;\"", colunaEscolhida, nomeFich);
            system(comando);
            exit(0);
      }
    }

    wait(NULL);
    wait(NULL);
}

void gerirTarefas() {
    
    printf("\n[PID: %d] Gestão de Tarefas\n", getpid());

    pid_t pid1 = fork();

    if(pid1 < 0) {
        printf("\n[PID: %d] Erro na criação do primeiro processo filho (fork).\n", getpid());
        return;
    } else if (pid1 == 0){
        system("gzip -f log_processos.txt && mv log_processos.txt.gz backup.gz");
        
        printf("[PID: %d] Backup criado com sucesso como 'backup.gz' a partir do 1º processo filho.\n", getpid());
        exit(0);
    }

    pid_t pid2 = fork();

    if (pid2 < 0) {
        printf("\n[PID: %d] Erro na criação do segundo processo filho (fork).\n", getpid());
        wait(NULL); 
        return;
    } else if (pid2 == 0) {
        system("rm -f resultados_estat.dat"); //forçar delete

        printf("[PID: %d] Ficheiros temporários eliminados com sucesso a partir do 2º processo filho.\n", getpid());
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    printf("\n[PID: %d] Os processos filho terminaram!\n", getpid());

    printf("[PID: %d] Pressione a tecla ENTER para voltar ao submenu...", getpid());
    getchar(); // sem este getchar o programa voltava diretamente ao submenu
    getchar();
}

void menuFicha2() {
    char opcao;

    do {
        printf("\nMENU FICHA 2\n");
        printf("\na) Monitorização de recursos\n");
        printf("b) Processamento de Dados com Filtros\n");
        printf("c) Gestão de Tarefas\n");
        printf("d) Sair (Voltar ao menu principal)\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &opcao);

        switch(opcao) {
            case 'a': monitorizarRecursos();  break;
            case 'b': processarDados(); break;
            case 'c': gerirTarefas(); break;
            case 'd': printf("A voltar ao menu principal..\n"); break;
            default: printf("Opcao inexistente.\n");
        }
    } while (opcao != 'd');
}

void terminarFilhos() {
    if (pidA > 0) kill(pidA, SIGKILL);
    if (pidB > 0) kill(pidB, SIGKILL);
    if (pidC > 0) kill(pidC, SIGKILL);

    printf("\nPROCESSO PAI [PID %d]: Sinal SIGKILL enviado aos filhos com PIDs A(%d), B(%d) e C(%d). Retornando para o menu principal. Pressione uma tecla para continuar...", getpid(), pidA, pidB, pidC);
    getchar(); 
    getchar();

    if (pidA > 0) waitpid(pidA, NULL, 0);
    if (pidB > 0) waitpid(pidB, NULL, 0);
    if (pidC > 0) waitpid(pidC, NULL, 0);

    pidA = 0;
    pidB = 0;
    pidC = 0;
}

void trataSinalPai(int sinal) {
    sinal_recebido = sinal;
}

void coordenarSinais() {
    
    //escutar as respostas dos filhos
    signal(SIGUSR1, trataSinalPai);
    signal(SIGUSR2, trataSinalPai);
    signal(SIGTERM, trataSinalPai);

    //ativar o filho A
    sinal_recebido = 0;
    printf("\nPROCESSO PAI [PID %d]: sinal SIGUSR1 enviado para o processo filho A [PID %d]\n", getpid(), pidA);
    kill(pidA, SIGUSR1);
    
    //bloquear o pai até que o filho A responda
    while (sinal_recebido != SIGUSR1) {
        pause(); 
    }
    printf("PROCESSO PAI [PID %d]: sinal SIGUSR1 recebido do processo filho A [PID %d]. Sinal SIGUSR2 será enviado de seguida ao processo filho B [PID %d] para verificação dos dados.\n", getpid(), pidA, pidB);
    
    //ativar o filho B
    sinal_recebido = 0;
    printf("PROCESSO PAI [PID %d]: Sinal SIGUSR2 enviado ao processo filho B [PID %d]\n", getpid(), pidB);
    kill(pidB, SIGUSR2);

    //bloquear o pai até que o filho B responda
    while (sinal_recebido != SIGUSR2) {
        pause();
    }
    printf("PROCESSO PAI [PID %d]: sinal SIGUSR2 recebido do processo filho B [PID %d]. Sinal SIGTERM será enviado ao processo filho C [PID %d] para geração gráfica.\n", getpid(), pidB, pidC);
    
    //ativar o filho C
    sinal_recebido = 0;
    printf("PROCESSO PAI [PID %d]: Sinal SIGTERM enviado ao processo filho C [PID %d]\n", getpid(), pidC);
    kill(pidC, SIGTERM);

    //bloquear o pai até que o filho C responda
    while (sinal_recebido != SIGTERM) {
        pause();
    }

    printf("PROCESSO PAI [PID %d]: Sinal SIGTERM recebido do processo filho C [PID %d]. Retornando para o submenu da ficha 3. Pressione uma tecla para continuar...\n", getpid(), pidC);

    getchar();
    getchar();
}

void trataSinalFilhoA(int sinal) {
    char nomeFich[100], comando[150], buffer[100], temCabecalho;
    int colunaEscolhida, numDados = 0;
    double valor, maximo = 0, minimo = 0;

    printf("\nPROCESSO FILHO A [PID %d]: sinal SIGUSR1 recebido!\n", getpid());

    printf("Introduza o nome do ficheiro de dados: ");
    scanf("%s", nomeFich);
    printf("O ficheiro possui cabeçalho (s/n)? ");
    scanf(" %c", &temCabecalho);
    printf("Introduza o número da coluna a analisar: ");
    scanf("%d", &colunaEscolhida);

    if (temCabecalho == 's' || temCabecalho == 'S') {
        sprintf(comando, "cut -d',' -f%d %s | tail -n +2", colunaEscolhida, nomeFich);
    } else {
        sprintf(comando, "cut -d',' -f%d %s", colunaEscolhida, nomeFich);
    }

    FILE *pipe = popen(comando, "r");
    FILE *f_bruto = fopen("ficha03_coluna_bruta.dat", "w");

    if (pipe == NULL || f_bruto == NULL) {
        printf("Erro ao processar ficheiros.\n");
        kill(getppid(), SIGUSR1); // Evita que o pai fique bloqueado se houver erro
        return;
    }

    //ler a saída do cut linha a linha diretamente da memória
    while (fgets(buffer, sizeof(buffer), pipe)) {
        fprintf(f_bruto, "%s", buffer);

        valor = atof(buffer);
        
        //lógica simples para descobrir o máximo e mínimo
        if (numDados == 0) {
            maximo = valor;
            minimo = valor;
        } else {
            if (valor > maximo) maximo = valor;
            if (valor < minimo) minimo = valor;
        }
        numDados++;
    }
    pclose(pipe);
    fclose(f_bruto);
    printf("PROCESSO FILHO A [PID %d]: ficheiro ficha03_coluna_bruta.dat atualizado com sucesso!\n", getpid());

    //guardar os extremos em modo append em um ficheiro
    FILE *f_extremos = fopen("ficha03_extremos.dat", "a");
    if (f_extremos != NULL) {
        fprintf(f_extremos, "Coluna: %d | Min: %.2f | Max: %.2f\n", colunaEscolhida, minimo, maximo);
        fclose(f_extremos);
    }
    printf("PROCESSO FILHO A [PID %d]: ficheiro ficha03_extremos.dat atualizado com sucesso!\n", getpid());

    printf("PROCESSO FILHO A [PID %d]: Operações realizadas com sucesso! Pressione uma tecla para continuar...", getpid());
    getchar();
    getchar();

    //devolve o sinal ao pai para continuar a coordenação de sinais
    kill(getppid(), SIGUSR1); 
}

void rotinaFilhoA() {

    signal(SIGUSR1, trataSinalFilhoA);

    while(1) {
        pause();
    }
}

void trataSinalFilhoB(int sinal) {
    char comando[150], buffer[100];

    printf("\nPROCESSO FILHO B [PID %d]: sinal SIGUSR2 recebido!\n", getpid());

    //apresentar o conteúdo do ficheiro ficha03_coluna_bruta.dat
    sprintf(comando, "cat ficha03_coluna_bruta.dat");
    FILE *pipe1 = popen(comando, "r");
    if (pipe1 != NULL) {
        while (fgets(buffer, sizeof(buffer), pipe1)) {
            printf("%s", buffer);
        }
        pclose(pipe1);
    }

    //apresentar o conteúdo do ficheiro ficha03_extremos.dat
    sprintf(comando, "cat ficha03_extremos.dat");
    FILE *pipe2 = popen(comando, "r");
    if (pipe2 != NULL) {
        while (fgets(buffer, sizeof(buffer), pipe2)) {
            printf("%s", buffer);
        }
        pclose(pipe2);
    }


    printf("PROCESSO FILHO B [PID %d]: Operações realizadas com sucesso! Pressione uma tecla para continuar...", getpid());
    getchar(); 
    getchar(); 

    //devolve o sinal ao pai para continuar a coordenação de sinais
    kill(getppid(), SIGUSR2);
}

void rotinaFilhoB() {

    signal(SIGUSR2, trataSinalFilhoB);

    printf("PROCESSO FILHO B [PID %d] : aguarda receção de sinal!\n", getpid());
    while(1) {
        pause();
    }
}

void trataSinalFilhoC(int sinal) {
    char comando[250];

    printf("\nPROCESSO FILHO C [PID %d]: Sinal SIGTERM recebido. Pressione uma tecla para continuar...", getpid());
    getchar();
    getchar();

    sprintf(comando, "gnuplot -e \"plot 'ficha03_coluna_bruta.dat' with lines; pause mouse;\"");
    system(comando);

    printf("PROCESSO FILHO C [PID %d]: Sinal SIGTERM enviado ao processo pai [PID %d]\n", getpid(), getppid());

    kill(getppid(), SIGTERM);
}

void rotinaFilhoC() {

    signal(SIGTERM, trataSinalFilhoC);

    printf("PROCESSO FILHO C [PID %d] : aguarda receção de sinal!\n", getpid());
    while(1) {
        pause(); 
    }
}

void limparFicheiros() {
    remove("ficha03_extremos.dat");
    remove("ficha03_coluna_bruta.dat");

    printf("\nPROCESSO PAI [PID %d]: Eliminação com sucesso dos ficheiros de dados (.dat). Pressione uma tecla para continuar...", getpid());
    
    getchar(); 
    getchar();
}

void menuFicha3() {
    char opcao;

    if (pidA == 0) {

        pidA = fork();
        if (pidA == 0) {
            rotinaFilhoA();
        }
        
        pidB = fork();
        if (pidB == 0) {
            rotinaFilhoB();
        }

        pidC = fork();
        if (pidC == 0) {
            rotinaFilhoC();
        }
        printf("\nPROCESSO PAI [PID %d]: criei os processos A (PID %d), B (PID %d) e C (PID %d). Por favor introduza a opção pretendida!\n", getpid(), pidA, pidB, pidC);
    }



    do {
        printf("\nMENU FICHA 3\n");
        printf("\na) Processamento Estatístico dos Extremos (Máximo/Mínimo)\n");
        printf("b) Limpeza de Ficheiros de Resultados\n");
        printf("c) Sair (Terminar Processos Filho)\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &opcao);

        switch(opcao) {
            case 'a': coordenarSinais(); break;
            case 'b': limparFicheiros(); break;
            case 'c': terminarFilhos(); break;
            default: printf("Opcao inexistente.\n");
        }
    } while (opcao != 'c');
}

void* rotinaAuditoriaA(void* arg) {
    char nomeFich[100], comando[150], buffer[100];
    int colunaEscolhida, repeticoes, numDados = 0;
    double valor, maximo = 0;
    
    printf("Introduza o nome do ficheiro de dados: ");
    scanf("%s", nomeFich);
    printf("Introduza o número da coluna a analisar: ");
    scanf("%d", &colunaEscolhida);
    printf("Introduza o número de repetições desejadas: ");
    scanf("%d", &repeticoes);

    sprintf(comando, "cut -d',' -f%d %s | tail -n +2", colunaEscolhida, nomeFich);
    FILE *pipe = popen(comando, "r");
    
    if (pipe == NULL) {
        printf("[PID: %d] Erro ao abrir o ficheiro/comando.\n", getpid());
        pthread_exit(NULL);
    }

    while (fgets(buffer, sizeof(buffer), pipe)) {
        valor = atof(buffer);
        if (numDados == 0) {
            maximo = valor;
        } else {
            if (valor > maximo) {
                maximo = valor;
            }
        }
        numDados++;
    }
    pclose(pipe);

    if (numDados == 0) {
        printf("[PID: %d] Nenhum dado encontrado na coluna indicada.\n", getpid());
        pthread_exit(NULL);
    }

    FILE *f_auditoria = fopen("auditoria_simples.txt", "w");
    if (f_auditoria == NULL) {
        printf("[PID: %d] Erro ao criar o ficheiro auditoria_simples.txt\n", getpid());
        pthread_exit(NULL);
    }

    for (int i = 0; i < repeticoes; i++) {
        fprintf(f_auditoria, "Auditoria Coluna [%d] | Valor Máximo: [%.2f]\n", colunaEscolhida, maximo);
    }
    fclose(f_auditoria);

    printf("\n[PID: %d] Ficheiro 'auditoria_simples.txt' gerado com sucesso com %d registos!\n", getpid(), repeticoes);
    
    pthread_exit(NULL); //termina a thread
}

void auditoriaEstatistica() {
    pthread_t t_id;

    //&t_id (onde guarda o ID), NULL (atributos padrão), rotina (função a executar), NULL (sem argumentos)
    if (pthread_create(&t_id, NULL, rotinaAuditoriaA, NULL) != 0) {
        printf("[PID: %d] Erro ao criar a thread.\n", getpid());
        return;
    }

    //esperar que a thread termine
    pthread_join(t_id, NULL);
    
    printf("[PID: %d] Thread de auditoria terminou. A voltar ao submenu...\n", getpid());
}


void* rotinaLogSemSemaforo(void* arg) {
    long indice = (long) arg; 
    int coluna = g_colunas[indice]; 
    char comando[150], buffer[100];
    int numDados = 0;
    double soma = 0, media = 0;
    pthread_t tid = pthread_self(); //id único da thread

    sprintf(comando, "cut -d',' -f%d %s | tail -n +2", coluna, g_nome_ficheiro);
    FILE *pipe = popen(comando, "r");
    if (pipe != NULL) {
        while (fgets(buffer, sizeof(buffer), pipe)) {
            soma += atof(buffer);
            numDados++;
        }
        pclose(pipe);
    }

    if (numDados > 0) {
        media = soma / numDados;
    }

    //escrita simultânea
    for (int i = 1; i <= g_repeticoes; i++) {
        FILE *f_auditoria = fopen("auditoria_concorrente.txt", "a");
        if (f_auditoria != NULL) {
            fprintf(f_auditoria, "Thread [%ld] | Coluna [%d] | Média: [%.2f] | Registo nº [%d]\n", 
                    tid, coluna, media, i);
            fclose(f_auditoria);
        }
    }

    pthread_exit(NULL);
}

void logsSemSemaforos() {
    pthread_t threads[3];

    //cria o ficheiro e informa o utilizador
    FILE *f_criar = fopen("auditoria_concorrente.txt", "w");
    if (f_criar == NULL) {
        printf("[PID: %d] Erro ao criar o ficheiro auditoria_concorrente.txt\n", getpid());
        return;
    }
    fclose(f_criar);
    printf("[PID: %d] O ficheiro auditoria_concorrente.txt foi criado com sucesso.\n", getpid());

    printf("[PID: %d] Introduza o nome do ficheiro de dados: ", getpid());
    scanf("%s", g_nome_ficheiro);
    printf("[PID: %d] Indique três colunas distintas (separadas por espaco): ", getpid());
    scanf("%d %d %d", &g_colunas[0], &g_colunas[1], &g_colunas[2]);
    do {
        printf("[PID: %d] Introduza o numero de repeticoes (obrigatoriamente elevado, min: 1000): ", getpid());
        scanf("%d", &g_repeticoes);
        
        if (g_repeticoes < 1000) {
            printf("[PID: %d] Erro: O numero de repeticoes deve ser elevado para garantir o teste concorrente.\n", getpid());
        }
    } while (g_repeticoes < 1000);

    //cria as três threads simultâneas passando o índice i (0, 1, 2)
    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, rotinaLogSemSemaforo, (void*)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* rotinaLogComSemaforo(void* arg) {
    long indice = (long) arg; 
    int coluna = g_colunas[indice]; 
    char comando[150], buffer[100];
    int numDados = 0;
    double soma = 0, media = 0;
    pthread_t tid = pthread_self(); 

    sprintf(comando, "cut -d',' -f%d %s | tail -n +2", coluna, g_nome_ficheiro);
    FILE *pipe = popen(comando, "r");
    if (pipe != NULL) {
        while (fgets(buffer, sizeof(buffer), pipe)) {
            soma += atof(buffer);
            numDados++;
        }
        pclose(pipe);
    }

    if (numDados > 0) {
        media = soma / numDados;
    }

    //bloqueio do mutex para garantir exclusão mútua na escrita
    pthread_mutex_lock(&g_mutex);

    for (int i = 1; i <= g_repeticoes; i++) {
        FILE *f_auditoria = fopen("auditoria_sincronizada.txt", "a");
        if (f_auditoria != NULL) {
            fprintf(f_auditoria, "Thread [%ld] | Coluna [%d] | Média: [%.2f] | Registo nº [%d]\n", 
                    tid, coluna, media, i);
            fclose(f_auditoria);
        }
    }

    //desbloqueio do mutex após concluir todas as repetições
    pthread_mutex_unlock(&g_mutex);

    pthread_exit(NULL);
}

void logsComSemaforos() {
    pthread_t threads[3];
    int res;

    //inicialização dinâmica do mutex conforme o livro
    res = pthread_mutex_init(&g_mutex, NULL);
    if (res != 0) {
        perror("Erro na inicializacao do Mutex");
        return;
    }
    
    FILE *f_criar = fopen("auditoria_sincronizada.txt", "w");
    if (f_criar == NULL) {
        printf("[PID: %d] Erro ao criar o ficheiro auditoria_sincronizada.txt\n", getpid());
        pthread_mutex_destroy(&g_mutex);
        return;
    }
    fclose(f_criar);
    printf("[PID: %d] O ficheiro auditoria_sincronizada.txt foi criado com sucesso.\n", getpid());

    printf("[PID: %d] Introduza o nome do ficheiro de dados: ", getpid());
    scanf("%s", g_nome_ficheiro);
    printf("[PID: %d] Indique três colunas distintas (separadas por espaco): ", getpid());
    scanf("%d %d %d", &g_colunas[0], &g_colunas[1], &g_colunas[2]);
    
    do {
        printf("[PID: %d] Introduza o numero de repeticoes (obrigatoriamente elevado, min: 1000): ", getpid());
        scanf("%d", &g_repeticoes);
        
        if (g_repeticoes < 1000) {
            printf("[PID: %d] Erro: O numero de repeticoes deve ser elevado para o teste.\n", getpid());
        }
    } while (g_repeticoes < 1000);

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, rotinaLogComSemaforo, (void*)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    //destruir o mutex depois de utilizar
    pthread_mutex_destroy(&g_mutex);
}

void menuFicha4() {
    char opcao;

    do {
        printf("\nMENU FICHA 4\n");
        printf("\na) Geração de Bloco de Auditoria Estatística\n");
        printf("b) Escrita Concorrente de Logs Partilhados (Sem Semáforos)\n");
        printf("c) Escrita Concorrente de Logs Partilhados (Com Semáforos)\n");
        printf("d) Sair (Voltar ao menu principal)\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &opcao);

        switch(opcao) {
            case 'a': auditoriaEstatistica(); break;
            case 'b': logsSemSemaforos(); break;
            case 'c': logsComSemaforos(); break;
            case 'd': printf("A voltar ao menu principal..\n"); break;
            default: printf("Opcao inexistente.\n");
        }
    } while (opcao != 'd');
}

int main() {
    int opcao;

    do {
        printf("\nMENU PRINCIPAL\n");
        printf("\n1) Ficha 1 - Processos\n");
        printf("2) Ficha 2 - Criacao de Processos\n");
        printf("3) Ficha 3 - Sinais (Signals)\n");
        printf("4) Ficha 4 - Threads e Sincronização\n");
        printf("5) Sair do Programa\n");
        printf("Escolha uma opcao: ");
        scanf(" %d", &opcao);

        switch(opcao) {
            case 1: menuFicha1(); break;
            case 2: menuFicha2(); break;
            case 3: menuFicha3(); break;
            case 4: menuFicha4(); break;
            case 5: printf("A sair..\n"); break;
            default: printf("Opcao inexistente.\n");
        }
    } while (opcao != 5);

    return 0;
}