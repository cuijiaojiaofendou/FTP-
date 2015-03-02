/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  客户端个函数功能的实现
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 17时05分43秒
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

void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line :%d", line);
    perror(err_string);
    exit(0);
}


void wan()
{
    system("clear");

    printf("--------------------WELCOME--------------------\n");
    printf("\t\t\t 1.ls\n");
    printf("\t\t\t 2.pwd\n");
    printf("\t\t\t 3.ls_R\n");
    printf("\t\t\t 4.upload\n");
    printf("\t\t\t 5.download\n");
    printf("\t\t\t 6.quit\n");
    printf("\t\tPlease choiced:\n");
}


void ls_fun(int conn_fd)
{
    char 	word[] = "ls";
    char 	recv_buf[1024];
    int 	ret, k;

//    printf("qqqqqqq\n");
    if(( k = send(conn_fd, word, strlen(word), 0))< 0)
    {
	printf("%d\n", k);
        my_err("send", __LINE__);
    }

    //将命令ls 发送至服务器进行解析
    ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0);

    if(ret < 0)
    {
        my_err("recv", __LINE__);
    }

    if(ret == 0)  //没有接收到任何来自客户端的信息
    {
        exit(0);
    }

    recv_buf[ret] = '\0';

    printf("%s\n", recv_buf);
}


void ls_R_fun(int conn_fd, char word[])
{
    char     path[1024000];     //接收所有的来自服务端的路径
    
    int      ret;

    if((send(conn_fd, word, strlen(word), 0)) < 0)
    {
        my_err("send", __LINE__);
    }
    
    ret = recv(conn_fd, path, sizeof(path), 0);

    if(ret < 0)
    {
        my_err("recv", __LINE__);
    }

    path[ret] = '\0';

    printf("%s\n", path);
    
}


void down_fun(int conn_fd, char com_word[])   //实现文件的下载
{
    char     file[1024];
    char     path[256] = "/home/cuijiaojiao/new/chonglai/cli";
    char     filename[256];
    int      ret, fd;
    int 	w_ret;
  
    
    chdir(path);
    getcwd(path, 256);


    if((send(conn_fd, com_word, strlen(com_word), 0)) < 0)
    {
        my_err("recv", __LINE__);
    }
 


    else {

	    printf("存放下载文件的名字：\n");
	    my_scanf(filename);
	    printf("存入下载的文件名 %s\n", filename);    //新建一个空白文件，将下载到的东西存放在该文件里

	    if((fd = open (filename, O_RDWR|O_CREAT, S_IRWXU)) == -1)
            {
	        my_err("open", __LINE__);
       	    } else {
	
		printf("success!\n");
	    }


       do{
	    memset(file, 0, 1024);                            //清理缓存，避免阻塞
 	    ret = recv(conn_fd, file, 1024, 0);
             
	    printf("size is %d bits\n", ret);
	    
	    if(ret < 0)
	    {
	        my_err("recv", __LINE__);
	    }

	    file[ret] = '\0';

            if(strncmp(file, "---", 3) == 0)
	    {
		 printf("send over!\n");
		 memset(file, 0, 1024);
		 break;
	    }
	    
	    w_ret = write(fd, file, ret);

	    if(w_ret == -1)
	    {
	        my_err("write", __LINE__);
	    }

	    printf("write is %d\n", w_ret);
	    
       }while(1);

	    close(fd);
    }
}
   


void up_fun(int conn_fd, char com_word[])    //实现文件的上传
{
    int      i, j, k = 0;
    int      file_fd;
    int      r_ret, s_ret;
    char     filename[256];
    char     fa_buf[1024];
    char     path[] = "/home/cuijiaojiao/new/chonglai/cli";

    chdir(path);

    getcwd(path, 256);
    
    send(conn_fd, com_word, strlen(com_word), 0);

    for(i = 0; i < strlen(com_word); i++)
    {
        if(com_word[i] == ' ' && com_word[i + 1] != ' ')
	{
	    j = i + 1;
	    while(com_word[j] != ' ')
	    {
	        filename[k ++] = com_word[j];
		j++;
	    }
	    filename[k] = '\0';
	}
    }

    printf("%s\n", filename);
    
    if((file_fd = open(filename, O_RDWR, 0644)) == -1)
    {
        my_err("open", __LINE__);
    }

    do
    {
        memset(fa_buf, 0, 1024);
	r_ret = read(file_fd, fa_buf, 1024);
	printf("read is %d bits...\n", r_ret);

	if(r_ret == -1)
	{
	    my_err("read", __LINE__);
	}
	
	if(r_ret <= 0)
	{
	    memset(fa_buf, '-', 1024);
	    send(conn_fd, fa_buf, 1024, 0);

	    break;
	}

	s_ret = send(conn_fd, fa_buf, r_ret, 0);
	printf("send is %d bit...\n", s_ret);

    }while(1);

    close(file_fd);
}


