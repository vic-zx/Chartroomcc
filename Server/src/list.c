#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

Link init_list()
{
    Link head = (Link)malloc(sizeof(Node));
    if(head == NULL)
    {
        printf("list malloc error.\n");
    }
    head->next = NULL;
    return head;
}

void insert_head_list(Link head, Link newnode)
{
    newnode->next = head->next;
    head->next = newnode;
}

int get_account(int c_fd, Link head)
{
    Link tmp = head->next;
    while(tmp != NULL && tmp->c_fd != c_fd)
    {
        tmp = tmp->next;
    }
    if(tmp == NULL)
    {
        return -1;
    }
    else
    {
        return tmp->account;
    }
}

int is_online(int account, Link head)
{
    Link tmp = head->next;
    while(tmp != NULL)
    {
        if(tmp->account == account)
        {
            return 0;
        }
        tmp = tmp->next;
    }
    return 1;
}

int get_c_fd(int account, Link head)
{
    Link tmp = head->next;
    while(tmp != NULL && tmp->account != account)
    {
        tmp = tmp->next;
    }
    if(tmp == NULL)
    {
        return -1;
    }
    else
    {
        return tmp->c_fd;
    }
}

int delete_node(int c_fd, Link head)
{
    Link tmp = head;
    Link d_node;

    while(tmp->next != NULL && tmp->next->c_fd != c_fd)
    {
        tmp = tmp->next;
    }
    if(tmp->next == NULL)
    {
        return -1;
    }
    else
    {
        d_node = tmp->next;
        tmp->next =tmp->next->next;
        free(d_node);
        d_node = NULL;
        return 0;
    }
    
}
