/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  客户端的函数声明
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 11时27分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  cuijiaojiao, cuijiaojiaofendou@gmail.com
 *        Company:  Class 1204 of Software
 *
 * =====================================================================================
 */


#ifndef CUI
#define CUI

    #include <limits.h>

    //自定义的错误处理函数
    void my_err(const char*, int);

    //客户端和服务端连接的功能模块
    int conn_ect(char *, char *);
    
    //ls功能的声明
    void ls_fun(int);

    //ls功能模块，第一个参数为连接描述符，第二个参数为命令
    void ls_R_fun(int, char*);

    void down_fun(int, char*);

    void up_fun(int, char *);

    //显示功能
    void wan();

#endif

