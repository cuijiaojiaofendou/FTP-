/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  客户端各函数块的实现
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 16时42分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  cuijiaojiao, cuijiaojiaofendou@gmail.com
 *        Company:  Class 1204 of Software
 *
 * =====================================================================================
 */


#include "tranfic.h"
#include "client.h"


 //函数对连接的处理
int conn_ect(char a[] , char b[])
{
    int 	serv_port, conn_fd;

    struct sockaddr_in    serv_addr;

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    serv_addr.sin_family = AF_INET;
    serv_port  = atoi(a);
    
 //   printf("%s\n", a);
    if(serv_port < 0 || serv_port > 65535)
    {
        printf("The port is error\n");
    } else {
        
	serv_addr.sin_port = htons(serv_port);

	if(inet_aton(b, &serv_addr.sin_addr) == 0)
	{
	    printf("invalid server ip address\n");
	    exit(0);
	} else {
	    
            conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	    printf("conn_fd success!\n");
	}
    }

 //   printf("qqqqq@@@@@\n");
    if(conn_fd < 0)
    {
        my_err("socket", __LINE__);
    }

    if(connect(conn_fd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr)) < 0)
    {
        my_err("connect", __LINE__);
    }

    return conn_fd;
}



