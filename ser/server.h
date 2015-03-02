/*
 * =====================================================================================
 *
 *       Filename:  server.h
 *
 *    Description:  服务端的函数声明
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 11时03分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  cuijiaojiao, cuijiaojiaofendou@gmail.com
 *        Company:  Class 1204 of Software
 *
 * =====================================================================================
 */


#ifndef  CUI
#define  CUI

    #define SERV_PORT   1212
    #define LISTENQ     12
    exit int ret;
    //错误函数的处理
    void  my_err(const char *err_string, int line);

    //实现ls的函数的实现
    void  sls_fun(int, char*, char*);

    //实现ls_R功能
    void  sls_R_fun(int, char*, char*);


    //上传功能的实现
    void  sup_fun(int, char*, char*);

    //查找功能的实现，找出/home/cuijiaojiao/test下的所有文件路径和文件名，并把他们存放到一个结构体数组里面
    void find_file(char*);

    //下载功能的实现
    void  sdown_fun(int, char*, char*);


    //日志文件
    void diary(char*, char*);

#endif
