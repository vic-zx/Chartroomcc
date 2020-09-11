#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "threcv_cli.h"

#define PORT 33333
#define MAX_SIZE 1024

int is_online;

int main()
{
	int c_fd;

    char cmd[20];

    struct sockaddr_in s_addr;

    c_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(c_fd == -1)
    {
        perror("socket error:");
        exit(1);
    }

    printf("socket success!\n");

    bzero(&s_addr, sizeof(struct sockaddr_in));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(c_fd, (struct sockaddr *)(&s_addr), sizeof(struct sockaddr_in)) < 0)
    {
        perror("connection error:");
        exit(1);
    }

    printf("connect success!\n");

    pthread_t thread_id;

    if(pthread_create(&thread_id , NULL , thread_recv ,(void *)(&c_fd)) != 0)
    {
        perror("pthread_create error:");
        exit(1);
    }
    

    is_online = 0;

    while(1)
	{
        if(is_online)
        {
            printf("$ ");
        }
        else
        {
            printf("> ");
        }

        scanf("%s", cmd);

        if(strcmp(cmd, "help") == 0)
        {
            help_menu_cli(is_online);
        }
        if(is_online)
        {
            if(strcmp(cmd, "friend") == 0) //5
            {
                list_friend_cli(c_fd); 
            }
            if(strcmp(cmd, "addfri") == 0) //6
             {
                add_friend_cli(c_fd);
            }
            if(strcmp(cmd, "group") == 0) //7
            {
                list_group_cli(c_fd);
            }
            if(strcmp(cmd, "addgro") == 0) //8
            {
                add_group_cli(c_fd);
            }
            if(strcmp(cmd, "msgf") == 0) //9
            {
                msg_friend_cli(c_fd);
            }
            if(strcmp(cmd, "msgg") == 0) //10
            {
                msg_group_cli(c_fd);
            }
            if(strcmp(cmd, "quit") == 0) //11
            {
                user_quit_cli(c_fd);
            }

        }
        else
        {
            if(strcmp(cmd, "login") == 0)
            {
                user_login_cli(c_fd);
            }

            if(strcmp(cmd, "reg") == 0)
            {
                user_register_cli(c_fd);
                
            }
            if(strcmp(cmd, "exit") == 0)
            {
                shutdown(c_fd, SHUT_RDWR);
                exit(0);
            }
        }

	}
    
	shutdown(c_fd, SHUT_RDWR);

	return 0;
}