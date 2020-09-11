#ifndef _LIST_H_
#define _LIST_H_

typedef struct node
{
    int c_fd;
    int account;
    struct node *next;
}Node, *Link;

Link init_list();
void insert_head_list(Link head, Link newnode);
int get_account(int c_fd, Link head);
int is_online(int account, Link head);
int get_c_fd(int account, Link head);
int delete_node(int c_fd, Link head);

#endif