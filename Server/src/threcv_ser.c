#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threcv_ser.h"

extern Link online;

void *thread_recv(void *arg)
{
    int c_fd;
    int ret;

    Message *msg = (Message *)malloc(sizeof(Message));

    c_fd = *((int *)arg);

    while (1)
    {
        memset(msg, 0, sizeof(Message));
        if ((ret = recv(c_fd, msg, sizeof(Message), 0)) < 0)
        {
            perror("recv error:");
            exit(1);
        }
        if (ret == 0)
        {
            printf("%d is close.\n", c_fd);
            pthread_exit(NULL);
        }

        switch (msg->action)
        {
            case 1:
            {
                user_login_ser(c_fd, msg, online);
                break;
            }
            case 2:
            {
                user_register_ser(c_fd, msg);
                break;
            }
            case 5:
            {
                list_friend_ser(c_fd, msg, online);
                break;
            }
            case 6:
            {
                add_friend_ser(c_fd, msg, online);
                break;
            }
            case 7:
            {
                list_group_ser(c_fd, msg, online);
                break;
            }

            case 8:
            {
                add_group_ser(c_fd, msg, online);
                break;
            }

            case 9:
            {
                msg_friend_ser(c_fd, msg, online);
                break;
            }

            case 10:
            {
                msg_group_ser(c_fd, msg, online);
                break;
            }
            case 11:
            {
                user_quit_ser(c_fd, msg, online);
                break;
            }

        }
        usleep(3);
    }
    pthread_exit(NULL);
}