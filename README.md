# CSV Data Manager in C (`csv-data-manager-c`)

Operating Systems university project developed during the second year of the Computer Engineering degree.

> **Note:** The application's interface and console output are in Portuguese, as the project was originally developed for a Portuguese university course.

This project is a concurrent CSV data processing application written in C for Linux. It demonstrates process management, multithreading, inter-process communication, and synchronization while reading, processing, and manipulating CSV files.

## Features

- **Process Management:** Dynamic process creation using `fork()` and parent/child process lifecycle management.
- **Signal Handling:** Communication and synchronization between processes using POSIX signals (`kill`, `sigaction`).
- **Multithreading:** Task parallelization using the `pthread` library.
- **Semaphore Synchronization:** Prevents race conditions and ensures safe access to shared resources.
- **CSV File Processing:** Reads, processes, and manipulates CSV data efficiently.
- **Concurrent Programming:** Combines processes and threads to perform data processing tasks concurrently.

## Requirements

This project was designed, developed, and tested on **Linux (Fedora)**. Since it relies on POSIX system calls, it is **not intended to run natively on Windows** without a compatibility layer such as WSL.

### Prerequisites

- GCC
- POSIX Threads (`-pthread`)

## Build

Compile the project with:

```bash
gcc main.c -o main -pthread
```

Or download the compiled binary instead: 
[![GitHub release](https://github.com/rodrigomfpereira/csv-data-manager-c/releases/tag/v1.0.0)
## Run

```bash
./main
```
