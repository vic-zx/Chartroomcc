#ifndef _DBUTIL_H_
#define _DBUTIL_H_

int add_user_record(int *id, char *nickname, char *sex, char *passwd);
int is_user_exist(int account, char *passwd);
int add_friend_record(int uid, int fid);
int is_friend_exist(int uid, int fid);
int list_friends(int account, int *nrow, int *ncol,char *** friends);
int get_user_nickname(int account, char (*nickname)[]);
int add_group_record(int uid, char *gname);
int is_in_group(int uid, char *gname);
int list_groups(int uid, int *nrow, int *ncol,char *** friends);
int get_group_member(char *gname, int *nrow, int *ncol,char *** group_mem);

#endif