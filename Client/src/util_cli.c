#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "util_cli.h"

void help_menu_cli(int is_online)
{
    if(is_online)
    {
        printf(
            "Options:\n"
            "\tfriend \t--View your friends list. \n"
            "\tgroup \t--View your own group list.\n"
            "\taddfri \t--Add friend\n"
            "\taddgro \t--Add group\n"
            "\tmsgf \t--Send a message to a friend\n"
            "\tmsgg \t--Send a message to a group\n"
            "\tquit \t--Sign out.\n"
        );
    }
    else
    {
        printf(
            "Options:\n"
            "\treg \t--Create an account. \n"
            "\tlogin \t--Login with an existing account.\n"
            "\texit \t--exit.\n"
        );
    }
}

void user_register_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    
    char nickname[20];
    char sex[10];
    char passwd[20];
    char confrim_passwd[20];

    int len;
	int ch;
	struct termios oldt,newt;

    memset(msg, 0, sizeof(Message));

    printf("nickname: ");
    scanf("%s", &nickname);

    printf("sex(male or female): ");
    scanf("%s", &sex);

    getchar();
    printf("passwd: ");

    len = 0;
    while(1)
    {
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &=~(ECHO | ICANON); 
        tcsetattr(STDIN_FILENO,TCSANOW,&newt); 
        ch = getchar();
        if(ch == '\n'){
            passwd[len] = '\0';
            tcsetattr(STDIN_FILENO,TCSANOW,&oldt); 
            break;
        }
        passwd[len] = ch;
        len++;
        tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
        printf("*");
    }
    printf("\n");

    printf("passwd confirm: ");
    len = 0;
    while(1)
    {
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &=~(ECHO | ICANON); 
        tcsetattr(STDIN_FILENO,TCSANOW,&newt); 
        ch = getchar();
        if(ch == '\n'){
            confrim_passwd[len] = '\0';
            tcsetattr(STDIN_FILENO,TCSANOW,&oldt); 
            break;
        }
        confrim_passwd[len] = ch;
        len++;
        tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
        printf("*");
    }
    printf("\n");
    fflush(stdout);
    
    if(!((strcmp(sex, "male")) && (strcmp(sex, "female"))) 
        && !(strcmp(passwd, confrim_passwd)))
    {
        msg->action = 2;
        strcpy(msg->msg, nickname);
        strcpy(msg->msg+100, sex);
        strcpy(msg->msg+200, passwd);
        if(send(c_fd, msg, sizeof(Message), 0) < 0)
        {
            perror("register error;");
            exit(1);
        }
    }
    else
    {
        printf("registration information error!\n");
    }
}

void user_login_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    int account;
    char passwd[20];
    
    int len;
    int ch;
	struct termios oldt,newt;

    memset(msg, 0, sizeof(Message));

    printf("account: ");
    scanf("%d", &account);

    getchar();
    printf("passwd: ");
    len = 0;
    while(1)
    {
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &=~(ECHO | ICANON); 
        tcsetattr(STDIN_FILENO,TCSANOW,&newt); 
        ch = getchar();
        if(ch == '\n'){
            passwd[len] = '\0';
            tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
            break;
        }
        passwd[len] = ch;
        len++;
        tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
        printf("*");
    }
    printf("\n");
    fflush(stdout);

    msg->action = 1;
    msg->account = account;
    strcpy(msg->msg, passwd);
    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("login error;");
        exit(1);
    }
}

void list_friend_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));

    memset(msg, 0, sizeof(Message));

    msg->action = 5;
    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("add friend error;");
        exit(1);
    }
     
}

void add_friend_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    int fri_account;

    memset(msg, 0, sizeof(Message));

    printf("friend account: ");
    scanf("%d", &fri_account);

    msg->action = 6;
    msg->account = fri_account;
    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("add friend error;");
        exit(1);
    }
}

void msg_friend_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    
    int account;
    char message[1024];

    memset(msg, 0, sizeof(Message));

    printf("friend_account: ");
    scanf("%d", &account);

    getchar();
    printf("massage: ");
    fgets(message, 1000, stdin);

    msg->action = 9;
    msg->account = account;
    strcpy(msg->msg, message);

    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("register error;");
        exit(1);
    }
}

void list_group_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    
    memset(msg, 0, sizeof(Message));

    msg->action = 7;
    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("list group error;");
        exit(1);
    }
}

void add_group_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    
    int i;
    int account;
    char gname[20];
    char g_mem_id[20];

    memset(msg, 0, sizeof(Message));

    printf("group name: ");
    scanf("%s", gname);

    strcpy(msg->msg, gname);

    printf("number of invitee: ");
    scanf("%d", &account);

    printf("invitee's account: ");
    for(i = 0; i < account; i++)
    {
        scanf("%s", g_mem_id);
        strcpy(msg->msg+40+i*50, g_mem_id);
    }
    msg->account = account;
    msg->action = 8;

    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("add group error;");
        exit(1);
    }
}

void msg_group_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));
    char gname[20];
    char message[1024];

    memset(msg, 0, sizeof(Message));

    printf("group_name: ");
    scanf("%s", gname);

    getchar();
    printf("message: ");
    fgets(message, 1000, stdin);

    msg->action = 10;
    strcpy(msg->msg, gname);
    strcpy(msg->msg+50, message);

    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("msg group error;");
        exit(1);
    }
}

void user_quit_cli(int c_fd)
{
    Message *msg = (Message *)malloc(sizeof(Message));

    memset(msg, 0, sizeof(Message));

    msg->action = 11;
    if(send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("sign out error;");
        exit(1);
    }

}


