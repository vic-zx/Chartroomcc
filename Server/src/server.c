#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "threcv_ser.h"

#define LISTEN_NUM 10
#define PORT 33333

Link online;

int main(int argc, int *argv[])
{
    int s_fd;
    int c_fd;

    int opt;
    int cl_len;

    char buff[1024];

    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;

    online = init_list();

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        perror("socket error:");
        exit(1);
    }

    printf("socket success!\n");

    bzero(&s_addr, sizeof(struct sockaddr_in));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    opt = 1;
    setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(s_fd, (struct sockaddr *)(&s_addr), sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind error:");
        exit(1);
    }

    printf("bind success!\n");

    if (listen(s_fd, LISTEN_NUM) < 0)
    {
        perror("listen error:");
        exit(1);
    }

    printf("listen success!\n");

    while (1)
    {

        memset(buff, 0, sizeof(buff));

        bzero(&c_addr, sizeof(struct sockaddr_in));
        cl_len = sizeof(struct sockaddr_in);

        printf("accepting ........ \n");

        c_fd = accept(s_fd, (struct sockaddr *)(&c_addr), &cl_len);
        if (c_fd == -1)
        {
            perror("accept error:");
            exit(1);
        }

        printf("ip: %s  port: %d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, thread_recv, (void *)(&c_fd)) != 0)
        {
            perror("pthread_create error:");
            exit(1);
        }
    }

    return 0;
}