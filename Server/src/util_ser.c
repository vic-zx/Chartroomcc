#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util_ser.h"

void user_login_ser(int c_fd, Message *msg, Link online)
{
    int ret;
    int account;
    char passwd[20];
    Link newnode;

    account = msg->account;
    strcpy(passwd, msg->msg);

    ret = is_online(account, online);
    // printf("%d", ret);
    if(ret != 0)
    {

        ret = is_user_exist(account, passwd);
        if (ret == 0)
        {
            newnode = (Link)malloc(sizeof(Node));
            newnode->c_fd = c_fd;
            newnode->account = account;

            insert_head_list(online, newnode);


            Link tmp = online->next;
            while(tmp != NULL)
            {
                printf("%d:%d --> ", tmp->c_fd, tmp->account);
                tmp = tmp->next;
            }
            printf("^\n");

            msg->account = 0;
            strcpy(msg->msg, "login successful.");
        }
        else
        {
            msg->account = -1;
            strcpy(msg->msg, "login failed.");
        }
    }
    else
    {
        msg->account = -1;
        strcpy(msg->msg, "this account has been logged in.");
    }

    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }
}

void user_register_ser(int c_fd, Message *msg)
{
    int ret;
    int id;
    char nickname[20];
    char sex[10];
    char passwd[20];

    strcpy(nickname, msg->msg);
    strcpy(sex, msg->msg + 100);
    strcpy(passwd, msg->msg + 200);

    ret = add_user_record(&id, nickname, sex, passwd);

    if (ret == 0)
    {
        strcpy(msg->msg, "register successful.");
        msg->account = id;
    }
    else
    {   
        msg->account = -1;
        strcpy(msg->msg, "register failed.");
    }

    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }
}

void add_friend_ser(int c_fd, Message *msg, Link online)
{
    int account;
    int fri_account;
    int ret;

    fri_account = msg->account;
    account = get_account(c_fd, online);

    ret = add_friend_record(account, fri_account);
    if(ret == -1)
    {
        strcpy(msg->msg, "friend addition failed.");
    }
    if(ret == 1)
    {
        strcpy(msg->msg, "friend already exist.");
    }
    if(ret == 0)
    {
        strcpy(msg->msg, "friend added successfully.");
    }

    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }
}

void list_friend_ser(int c_fd, Message *msg, Link online)
{
    int account;
    char ** friends;
    int fri_len;
    int row, col;
    int ret;
    int i, j;
    int f_id;
    char nickname[20];
    int now_online;

    account = get_account(c_fd, online);
    ret = list_friends(account, &row, &col, &friends);
    if(ret == 0)
    {
        msg->account = row * col;

        fri_len = 0;

        for(i = 0; i < row; i++)
        {
            for(j = 1; j <= col; j++) 
            {
                strcpy(msg->msg+(fri_len*60), friends[col*i+j]);
                f_id = atoi(friends[col*i+j]);
                
                get_user_nickname(f_id, &nickname);

                strcpy(msg->msg+(fri_len*60+20), nickname);

                now_online = is_online(f_id, online);
                if(now_online == 0)
                {
                    strcpy(msg->msg+(fri_len*60+40), "online");
                }
                else
                {
                    strcpy(msg->msg+(fri_len*60+40), "not online");
                }
                
                fri_len++;
            }
        }
    }
    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }
}

void msg_friend_ser(int c_fd, Message *msg, Link online)
{
    int co_fd;
    int account;
    char nickname[20];
    char message[1024];

    account = get_account(c_fd, online);
    get_user_nickname(account, &nickname);

    sprintf(message, "%s(%d) send a message to you: \n\t%s", nickname, account, msg->msg);
    
    co_fd = get_c_fd(msg->account, online);

    if(co_fd != -1)
    {
        msg->action = 20;
        strcpy(msg->msg, message);
        if (send(co_fd, msg, sizeof(Message), 0) < 0)
        {
            perror("send error:");
            exit(1);
        }

        msg->action = 9;
        strcpy(msg->msg, "message sent successfully.");
        if (send(c_fd, msg, sizeof(Message), 0) < 0)
        {
            perror("send error:");
            exit(1);
        }
    }
    else
    {
        msg->action = 9;
        strcpy(msg->msg, "not online! message sent failed.");
        if (send(c_fd, msg, sizeof(Message), 0) < 0)
        {
            perror("send error:");
            exit(1);
        }
    }
    

}

void add_group_ser(int c_fd, Message *msg, Link online)
{
    int i;
    int account;
    char gname[20];
    char g_mem_id[20];
    int gm_id;

    account = msg->account;

    strcpy(gname, msg->msg);

    for(i = 0; i < account; i++)
    {
        strcpy(g_mem_id, msg->msg+40+i*50);
        gm_id = atoi(g_mem_id);
        add_group_record(gm_id, gname);
    }
    account = get_account(c_fd, online);
    add_group_record(account, gname);

    strcpy(msg->msg, "create group successfully.");
    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }
}

void list_group_ser(int c_fd, Message *msg, Link online)
{
    int account;
    int fri_len;
    int row, col;
    int ret;
    int i, j;
    int f_id; 
    char ** groups;

    account = get_account(c_fd, online);

    ret = list_groups(account, &row, &col, &groups);
    if(ret == 0)
    {
        msg->account = row * col;

        fri_len = 0;

        for(i = 0; i < row; i++)
        {
            for(j = 1; j <= col; j++) 
            {
                strcpy(msg->msg+(fri_len*40), groups[col*i+j]);
                f_id = atoi(groups[col*i+j]);
                
                fri_len++;
            }
        }
    }
    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }

}

void msg_group_ser(int c_fd, Message *msg, Link online)
{
    int co_fd;
    char gname[20];
    int account;
    char nickname[20];
    char message[1024];

    int fri_len;
    int row, col;
    int ret;
    int i, j;
    int toc_fd; 
    char ** group_mem;

    strcpy(gname, msg->msg);
    strcpy(message, msg->msg+50);

    account = get_account(c_fd, online);
    get_user_nickname(account, &nickname);

    sprintf(msg->msg, "%s(%d) send a message by %s: \n\t%s", nickname, account, gname, message);
    
    ret = get_group_member(gname, &row, &col, &group_mem);

    if(ret == 0)
    {
        msg->account = row * col;

        fri_len = 0;

        for(i = 0; i < row; i++)
        {
            for(j = 1; j <= col; j++) 
            {
                account = atoi(group_mem[col*i+j]);
                toc_fd =  get_c_fd(account, online);
                if(toc_fd != -1)
                {
                    msg->action = 21;
                    if (send(toc_fd, msg, sizeof(Message), 0) < 0)
                    {
                        perror("send error:");
                        exit(1);
                    }
                }
                
                fri_len++;
            }
        }
    }
    msg->action = 10;
    strcpy(msg->msg, "group message success.");
    if (send(c_fd, msg, sizeof(Message), 0) < 0)
    {
        perror("send error:");
        exit(1);
    }

}

void user_quit_ser(int c_fd, Message *msg, Link online)
{
    int ret;
    ret = delete_node(c_fd, online);

    Link tmp = online->next;
    while(tmp != NULL)
    {
        printf("%d:%d --> ", tmp->c_fd, tmp->account);
        tmp = tmp->next;
    }
    printf("^\n");

    if(ret != -1)
    {
        msg->account = 0;
        strcpy(msg->msg, "sign out successful.");
        if (send(c_fd, msg, sizeof(Message), 0) < 0)
        {
            perror("send error:");
            exit(1);
        }
    }
    else
    {
        msg->account = -1;
        strcpy(msg->msg, "sign out failed.");
        if (send(c_fd, msg, sizeof(Message), 0) < 0)
        {
            perror("send error:");
            exit(1);
        }
    }
    
}