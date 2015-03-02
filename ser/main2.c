/*
 * =====================================================================================
 *
 *       Filename:  main2.c
 *
 *    Description:  服务端的main函数
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 17时40分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  cuijiaojiao, cuijiaojiaofendou@gmail.com
 *        Company:  Class 1204 of Software
 *
 * =====================================================================================
 */

#include "tranfic.h"
#include "server.h"

#define SERV_PORT   1212
#define LISTENQ     12

int main(int argc, char *argv[])
{
    char 	som_word[10];
    char 	recv_buf[1024];
    char 	send_buf[1024];
    char        cli_ip[20];
    int 	sock_fd, conn_fd, ret;
    int 	cli_len, optval = 1;

    pid_t 	pid;

    struct sockaddr_in     serv_addr, cli_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(sock_fd < 0)
    {
	my_err("socket", __LINE__);
    }

    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
    {
	my_err("setsockopt", __LINE__);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
	my_err("bind", __LINE__);
    }

    if(listen(sock_fd, LISTENQ) < 0)
    {
	my_err("listen", __LINE__);
    }

    cli_len = sizeof(struct sockaddr_in);

    while(1)
    {
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
	
	//printf("%d\n", conn_fd);

	strcpy(cli_ip, (char *)inet_ntoa(cli_addr.sin_addr));
        if(conn_fd < 0)
	{
	    my_err("accept", __LINE__);
	}

	printf("----------------connect success----------------\n");

	if ((pid = fork()) == 0)                  
	{
            while(1)
	    {
		 ret = recv(conn_fd, recv_buf,sizeof(recv_buf), 0);

		 if(ret < 0)
		 {
		       my_err("recv", __LINE__);
		 }

		 recv_buf[ret] = '\0';
		 printf("@_@%s\n", recv_buf);

		 if(strcmp(recv_buf, "ls") == 0) {
		   
		      sls_fun(conn_fd, recv_buf, send_buf);
		      
		      diary("ls", cli_ip);
		 } else if(strncmp(recv_buf, "ls_R", 4) == 0) {
		   
		      sls_R_fun(conn_fd, recv_buf, send_buf);

//		      printf("!!!!!!!!!\n");
		      diary("ls_R", cli_ip);
		 } else if(strncmp(recv_buf, "upload", 6) == 0) {
		   
	               sup_fun(conn_fd, recv_buf, send_buf);
		       
		       diary("upload", cli_ip);
		 } else if(strncmp(recv_buf, "download", 8) == 0) {
		   
		       sdown_fun(conn_fd, recv_buf, send_buf);
		   
		       diary("download", cli_ip);
		 } else if(strcmp(recv_buf, "quit") == 0){

		       strcpy(som_word, "quit");

		       send(conn_fd, som_word, strlen(som_word), 0);
		      
                       diary("quit", cli_ip);
		       
		       break;

		  } else {
		  
		 //      send(conn_fd, recv_buf, strlen(recv_buf), 0);
		       printf("input error\n");
		       exit(0);

		  }

	       }
		
		close(conn_fd);
	
		exit(0);
	    } else {
	    
	        close(conn_fd);
	    }
	}

	return EXIT_SUCCESS;
}

    
