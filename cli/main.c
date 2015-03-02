/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  客户端的主函数
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 11时36分21秒
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

void my_scanf(char *string)
{
    while((*string = (char)getchar()) != '\n')
	  string ++;
    *string = '\0';
}


int main(int argc, char *argv[])
{
    int 	conn_fd, serv_port;
    int 	ret;
    char 	recv_buf[BUFSIZE];
    char 	com_word[1024];
    char 	buf[1024];
    
    struct sockaddr_in    serv_addr;

    if(argc != 3)
    {
        printf("Usage : [%s] [serv_port] [serv_address]\n", argv[0]);
	exit(0);
    }

//    printf("%s, %s\n", argv[1], argv[2]);

    conn_fd = conn_ect(argv[1], argv[2]);

    printf("%d\n", conn_fd);
    wan();

    while(1)
    {
        printf("ftp://$command:");

	my_scanf(com_word);

	printf("%s\n", com_word);

	if(strcmp(com_word, "ls") == 0)
	{
	    ls_fun(conn_fd);
	    continue;
	
	} else if(strncmp(com_word, "ls_R", 4) == 0) {
	  
	    ls_R_fun(conn_fd, com_word);
	    continue;
	
	} else if(strcmp(com_word, "pwd") == 0) {

	    getcwd(buf, 1024);
	    printf("%s\n", buf);

	} else if(strncmp(com_word, "upload", 6) == 0) {
	    
            up_fun(conn_fd, com_word);
	    continue;
	
	} else if(strncmp(com_word, "download", 8) == 0) {

	    down_fun(conn_fd, com_word);
	    continue;
	
	} else if(strcmp(com_word, "quit") == 0) {

	    send(conn_fd, com_word, strlen(com_word), 0);
	    break;
	
	} else {

//	    send(conn_fd, com_word, strlen(com_word), 0);
	    printf("input error,please try again!\n");
	    wan();
	    continue;

	}
    }

	return EXIT_SUCCESS;
}

