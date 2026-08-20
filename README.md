# Linux-IPC--Pipes
Linux-IPC-Pipes


# Ex03-Linux IPC - Pipes

# AIM:
To write a C program that illustrate communication between two process using unnamed and named pipes

# DESIGN STEPS:

### Step 1:

Navigate to any Linux environment installed on the system or installed inside a virtual environment like virtual box/vmware or online linux JSLinux (https://bellard.org/jslinux/vm.html?url=alpine-x86.cfg&mem=192) or docker.

### Step 2:

Write the C Program using Linux Process API - pipe(), fifo()

### Step 3:

Testing the C Program for the desired output. 

# PROGRAM:

## C Program that illustrate communication between two process using unnamed pipes using Linux API system calls

```
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void server(int, int);
void client(int, int);

int main()
{
    int p1[2], p2[2], pid;

    pipe(p1);
    pipe(p2);

    pid = fork();

    if(pid == 0)
    {
        close(p1[1]);
        close(p2[0]);
        server(p1[0], p2[1]);
        exit(0);
    }

    close(p1[0]);
    close(p2[1]);
    client(p1[1], p2[0]);

    wait(NULL);

    return 0;
}

void server(int rfd, int wfd)
{
    int n;
    char fname[2000];
    char buff[2000];

    n = read(rfd, fname, sizeof(fname));
    fname[n] = '\0';

    int fd = open(fname, O_RDONLY);

    if(fd < 0)
    {
        write(wfd, "can't open", 10);
    }
    else
    {
        n = read(fd, buff, sizeof(buff));
        write(wfd, buff, n);
        close(fd);
    }
}

void client(int wfd, int rfd)
{
    int n;
    char fname[2000];
    char buff[2000];

    printf("Enter file name: ");
    scanf("%s", fname);

    write(wfd, fname, strlen(fname)+1);

    n = read(rfd, buff, sizeof(buff));
    buff[n] = '\0';

    printf("\nContents of file:\n");
    printf("%s\n", buff);
}

```


## OUTPUT

<img width="809" height="336" alt="image" src="https://github.com/user-attachments/assets/2d8f1964-6ad0-4c09-a65e-19994063207b" />


## C Program that illustrate communication between two process using named pipes using Linux API system calls

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE "/tmp/my_fifo"
#define FILE_NAME "hello.txt"

void server();
void client();

int main()
{
    pid_t pid;

    // Create FIFO
    mkfifo(FIFO_FILE, 0666);

    pid = fork();

    if(pid > 0)
    {
        // Parent process
        sleep(1);
        server();
    }
    else if(pid == 0)
    {
        // Child process
        client();
    }
    else
    {
        printf("Fork Failed\n");
        exit(1);
    }

    return 0;
}

void server()
{
    int fifo_fd, file_fd;
    char buffer[1024];
    int n;

    file_fd = open(FILE_NAME, O_RDONLY);

    if(file_fd < 0)
    {
        printf("Cannot open hello.txt\n");
        exit(1);
    }

    fifo_fd = open(FIFO_FILE, O_WRONLY);

    if(fifo_fd < 0)
    {
        printf("Cannot open FIFO\n");
        exit(1);
    }

    while((n = read(file_fd, buffer, sizeof(buffer))) > 0)
    {
        write(fifo_fd, buffer, n);
    }

    close(file_fd);
    close(fifo_fd);
}

void client()
{
    int fifo_fd;
    char buffer[1024];
    int n;

    fifo_fd = open(FIFO_FILE, O_RDONLY);

    if(fifo_fd < 0)
    {
        printf("Cannot open FIFO\n");
        exit(1);
    }

    while((n = read(fifo_fd, buffer, sizeof(buffer))) > 0)
    {
        write(1, buffer, n);
    }

    close(fifo_fd);
}

```



## OUTPUT

<img width="384" height="304" alt="image" src="https://github.com/user-attachments/assets/bc8eacaf-8024-4731-9b55-aa4477551995" />

# RESULT:
The program is executed successfully.
