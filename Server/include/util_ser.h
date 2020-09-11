#ifndef _UTIL_SER_H_
#define _UTIL_SER_H_

#include "dbutil.h"
#include "list.h"

typedef struct message
{
    int action;
    int account;
    char msg[1024];
}Message;

void user_login_ser(int c_fd, Message *msg, Link online);
void user_register_ser(int c_fd, Message *msg);
void add_friend_ser(int c_fd, Message *msg, Link online);
void list_friend_ser(int c_fd, Message *msg, Link online);
void add_group_ser(int c_fd, Message *msg, Link online);
void list_group_ser(int c_fd, Message *msg, Link online);
void msg_friend_ser(int c_fd, Message *msg, Link online);
void msg_group_ser(int c_fd, Message *msg, Link online);
void user_quit_ser(int c_fd, Message *msg, Link online);

#endif