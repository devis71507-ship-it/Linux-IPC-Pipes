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

