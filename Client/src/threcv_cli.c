#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "threcv_cli.h"

extern int is_online;

void * thread_recv(void *arg)
{
    int c_fd;
    int ret;

    Message *msg = (Message *)malloc(sizeof(Message));

    c_fd = *((int *)arg);

    while(1)
    {
		memset(msg, 0, sizeof(Message));
        if((ret = recv(c_fd, msg, sizeof(Message), 0)) < 0)
        {
            perror("recv error:");
            exit(1);
        }		
        if(ret == 0)
        {
            printf("%d is close.\n", c_fd);
            pthread_exit(NULL);
        }
		
        switch(msg->action)
        {
            case 1:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(msg->account == 0)
                {
                    is_online = 1;
                }
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }
            case 2:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(msg->account != -1)
                {
                    printf("you account: %d\n", msg->account);
                }
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }
            
            case 5:
            {
                printf("\b\b");
                int i;
                char ac_or_nick[20];

                for(i = 0; i < msg->account; i++)
                {
                    strcpy(ac_or_nick, msg->msg+(i*60));
                    printf("%s\t", ac_or_nick);

                    strcpy(ac_or_nick, msg->msg+(i*60+20));
                    printf("%s\t", ac_or_nick);

                    strcpy(ac_or_nick, msg->msg+(i*60+40));
                    printf("(%s)\n", ac_or_nick);
                }

                if(msg->account == 0)
                {
                    printf("you don't have friends yet.\n");
                }

                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }
            case 6:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }

            case 7:
            {
                printf("\b\b");
                int i;
                char gname[20];

                for(i = 0; i < msg->account; i++)
                {
                    strcpy(gname, msg->msg+(i*40));
                    printf("%s\n", gname);
                }
                 if(msg->account == 0)
                {
                    printf("you don't have a group chat yet.\n");
                }
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }

            case 8:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }

            case 9:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }
            case 10:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }

            case 11:
            {
                printf("\b\b");
                printf("%s\n", msg->msg);
                if(msg->account == 0)
                {
                    is_online = 0;
                }
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }

            case 20:
            {
                printf("\b\b");
                printf("%s", msg->msg);
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }
            case 21:
            {
                printf("\b\b");
                printf("%s", msg->msg);
                if(is_online)
                {
                    printf("$ ");
                }
                else
                {
                    printf("> ");
                }
                fflush(stdout);
                break;
            }
            default:
            {
                break;
            }
        }
        usleep(3);
    }
	pthread_exit(NULL);
    
}