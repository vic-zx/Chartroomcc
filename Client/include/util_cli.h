#ifndef _UTIL_CLI_H_
#define _UTIL_CLI_H_

typedef struct message
{
    int action;
    int account;
    char msg[1024];
}Message;


void help_menu_cli(int is_online);
void user_login_cli(int c_fd);
void user_register_cli(int c_fd);
void list_friend_cli(int c_fd);
void add_friend_cli(int c_fd);
void list_group_cli(int c_fd);
void add_group_cli(int c_fd);
void msg_friend_cli(int c_fd);
void msg_group_cli(int c_fd);
void user_quit_cli(int c_fd);

#endif