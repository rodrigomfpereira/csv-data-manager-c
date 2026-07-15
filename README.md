# CSV Data Manager in C 

Operating Systems university project developed during the second year of the Computer Engineering degree.

> **Note:** The application's interface and console output are in Portuguese, as the project was originally developed for a Portuguese university course. It was a requirement for the whole code to be in a single file, otherwise I would have separated it.

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
[![GitHub release](https://img.shields.io/github/v/release/rodrigomfpereira/csv-data-manager-c)](https://github.com/rodrigomfpereira/csv-data-manager-c/releases/latest)

## Run

```bash
./main
```
