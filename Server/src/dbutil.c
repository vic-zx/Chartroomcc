#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "dbutil.h"

#define DBPATH "/home/vic/note/chat001/Server/chat001.db"


int add_user_record(int *id, char *nickname, char *sex, char *passwd)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        sqlite3_close(db);
        exit(1);
    }

    sprintf(sql, "insert into chat_user values (NULL, \'%s\', \'%s\', \'%s\', datetime('now'))", nickname, sex, passwd);

    printf("%s\n", sql);

    ret = sqlite3_exec(db, sql, NULL, NULL, &error);
    if(ret != SQLITE_OK)
    {
        perror("insert error:");
        sqlite3_close(db);
        return -1;
    }        

    ret = sqlite3_get_table(db, "select last_insert_rowid() newid", &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    } 

    *id = atoi(result[1]);

    sqlite3_close(db);
    return 0;

}

int is_user_exist(int account, char *passwd)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        exit(1);
    }

    sprintf(sql, "select * from chat_user where id = %d and passwd = \'%s\' limit 1", account, passwd);


    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    }       
    sqlite3_close(db);

    if(row == 1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int add_friend_record(int uid, int fid)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    if(is_friend_exist(uid, fid) != 0)
    {
        ret = sqlite3_open(DBPATH, &db);
        if(ret != SQLITE_OK)
        {
            perror("open db error:");
            sqlite3_close(db);
            exit(1);
        }

        sqlite3_exec(db, "PRAGMA foreign_keys=ON;", NULL, NULL, &error);

        sprintf(sql, "insert into chat_friend values (NULL, %d, %d)", uid, fid);
        printf("%s\n", sql);
        
        ret = sqlite3_exec(db, sql, NULL, NULL, &error);
        if(ret != SQLITE_OK)
        {
            perror("insert error:");
            sqlite3_close(db);
            return -1;
        }

        sprintf(sql, "insert into chat_friend values (NULL, %d, %d)", fid, uid);
        printf("%s\n", sql);
        
        ret = sqlite3_exec(db, sql, NULL, NULL, &error);
        if(ret != SQLITE_OK)
        {
            perror("insert error:");
            sqlite3_close(db);
            return -1;
        }
        sqlite3_close(db);
    }
    else
    {
        return 1;
    }
    
    return 0;

}


int is_friend_exist(int uid, int fid)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        exit(1);
    }

    sprintf(sql, "select * from chat_friend where uid = %d and fid = %d limit 1", uid, fid);


    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    }       
    sqlite3_close(db);
    printf("%d\n", row); 

    if(row == 1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}



int list_friends(int account, int *nrow, int *ncol,char *** friends)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        sqlite3_close(db);
        exit(1);
    }

    sprintf(sql, "select fid from chat_friend where uid = %d", account);

    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    } 

    *friends = result;
    *nrow = row;
    *ncol = col;

    sqlite3_close(db);
    return 0;
}

int get_user_nickname(int account, char (*nickname)[])
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        sqlite3_close(db);
        exit(1);
    }

    sprintf(sql, "select nickname from chat_user where id = %d", account);

    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    } 

    strcpy(*nickname, result[col * 0 + 1]);

    sqlite3_close(db);
    return 0;
}

int add_group_record(int uid, char *gname)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    if(is_in_group(uid, gname) != 0)
    {
        ret = sqlite3_open(DBPATH, &db);
        if(ret != SQLITE_OK)
        {
            perror("open db error:");
            sqlite3_close(db);
            exit(1);
        }

        sqlite3_exec(db, "PRAGMA foreign_keys=ON;", NULL, NULL, &error);

        sprintf(sql, "insert into chat_group values (NULL, %d, \'%s\')", uid, gname);
        printf("%s\n", sql);
        
        ret = sqlite3_exec(db, sql, NULL, NULL, &error);
        if(ret != SQLITE_OK)
        {
            perror("insert error:");
            sqlite3_close(db);
            return -1;
        }
        sqlite3_close(db);
    }
    else
    {
        return 1;
    }
    
    return 0;
}


int is_in_group(int uid, char *gname)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        exit(1);
    }

    sprintf(sql, "select * from chat_group where uid = %d and gname = \'%s\' limit 1", uid, gname);


    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    }       
    sqlite3_close(db);

    if(row == 1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int list_groups(int uid, int *nrow, int *ncol,char *** friends)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        exit(1);
    }

    sprintf(sql, "select gname from chat_group where uid = %d ", uid);

    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    } 


    *friends = result;
    *nrow = row;
    *ncol = col;

    sqlite3_close(db);

    return 0;
}

int get_group_member(char *gname, int *nrow, int *ncol,char *** group_mem)
{
    sqlite3 *db = NULL;
    int ret;
    int row, col;
    char **result;
    char *error;
    char sql[100];

    ret = sqlite3_open(DBPATH, &db);
    if(ret != SQLITE_OK)
    {
        perror("open db error:");
        exit(1);
    }

    sprintf(sql, "select uid from chat_group where gname = \'%s\' ", gname);

    printf("%s\n", sql);

    ret = sqlite3_get_table(db, sql, &result, &row, &col, &error);
    if(ret != SQLITE_OK)
    {
        perror("get_table error:");
        sqlite3_close(db);
        return -1;
    } 


    *group_mem = result;
    *nrow = row;
    *ncol = col;

    sqlite3_close(db);

    return 0;
}
