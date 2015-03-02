/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description: 服务端各功能的函数处理
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 17时36分08秒
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

void find_file(char *);

void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line :%d", line);
    perror(err_string);
    exit(1);
}

//ls命令的处理
void sls_fun(int connfd, char recv_buf[], char send_buf[]) 
{
    DIR*     dir;
    char     filebuf[102400];        //存放所有文件名的缓冲区大小
    char     dirbuf[256] = "/home/cuijaojiao/new/chonglai/ser";
    int      i = 0;
    
    struct dirent *ptr;

    chdir(dirbuf);
    getcwd(dirbuf, 256);
    printf("%s\n", dirbuf);

    if((dir = opendir(dirbuf)) == NULL)
    {
        perror("opendir");
	exit(0);
    }
    while((ptr = readdir(dir)) != NULL)
    {   
	if(ptr->d_name[0] != '.')
	{
           sprintf(filebuf + i, "%s\n",ptr->d_name);

	   i += (strlen (ptr->d_name) + 1);
	}

    }
    
    send (connfd, filebuf, strlen(filebuf), 0);
    
    closedir(dir);

}

//查找任意路径下对文件名路径的处理
void open_dir(char dir_buf[], char filename[])
{
	DIR      *dir;
	struct dirent  *entry;
	struct stat    statbuf;
	char 		path[256];

//	printf("%s\n",dir_buf);
        if((dir = opendir(dir_buf)) == NULL)
	{
	    perror("operdir");
	    exit(1);
	}
	chdir(dir_buf);
	
	while((entry = readdir(dir)) != NULL)
	{
	    if(lstat(entry->d_name, &statbuf) == -1)
	    {
	        perror(entry->d_name);
		exit(1);
	    }

	    if(S_ISDIR(statbuf.st_mode))
	    {
	        if(entry->d_name[0] == '.')
		{
		    continue;
		}
		
		strcat(filename,"/home/cuijiaojiao/");
		strcat(filename, entry->d_name);
		open_dir(entry->d_name, filename);
	    
	    } else {
	
		getcwd(path, 256);
		strcat(filename,path);
		strcat(filename, "/");
	        strcat(filename, entry->d_name);
		strcat(filename, "\n");
	    }
	}
	chdir("..");
	closedir(dir);
}


//ls_R函数的处理，对任意路径下都满足
void sls_R_fun(int conn_fd, char recv_buf[], char send_buf[])
{
    DIR      *dir;
    char     buf[PATH_MAX + 1];
    char     dirbuf[1024];
    char     filename[1024000];
    int      i, j = 0, k;
    struct dirent   *entry;
    struct stat     statbuf;
    for(i = 0; i < strlen(recv_buf); i++)
    {
        if(recv_buf[i] == '/')
	{
	    for(k = i; k < (strlen(recv_buf) + 1); k++, j++)
	    {
	        dirbuf[j] = recv_buf[k];
	    }
	}
    }

    chdir(dirbuf);
    getcwd(dirbuf, sizeof(dirbuf));
    printf("%s\n", dirbuf);

    open_dir(dirbuf, filename);

    if((send(conn_fd, filename, strlen(filename), 0)) < 0)
    {
        my_err("send", __LINE__);
    }

    chdir("/home/cuijiaojiao/new/chonglai/ser");
}



struct file
{
    char    filename[256];
    char    path[256];

}name[100];

void sdown_fun(int connfd, char recv_buf[], char send_buf[])
{
    char    dirbuf[100];
    char    bufs[] = "/home/cuijiaojiao/test";
    int     i = 0, j = 0, k;
    int     fd, len;
    int     r_ret;
    int     s_ret;
    char    read_buf[1024];
    char    file[1024];
    char    filename[256];
    char    buf[1024];

    for( i = 0; i < strlen(recv_buf) + 1; i++)
    {
        if(recv_buf[i] == ' ')
	{
	    for(k = i + 1; k < strlen(recv_buf); j++, k++)
            {
	        filename[j] = recv_buf[k];
	    }
	    filename[j] = '\0';
	}
    }

    printf("%s\n",filename);

    find_file(bufs);
    
    for(i = 0; i < 100; i ++)
    { 
        if(strcmp(name[i].filename, filename) == 0)
	{
	    printf("path is %s\n",name[i].path);
	    
	    chdir(name[i].path);
	    
	    if((fd = open(name[i].filename, O_RDWR, 0644)) == -1)
	    {
	        my_err("open", __LINE__);
	    }

	    do{
	    
		 memset(buf, 0, 1024);
	         r_ret = read(fd, buf, 1024);
	         printf("read is %d bit...\n", r_ret);

                 if (r_ret == -1)
		     my_err("read", __LINE__);
	    
		 if (r_ret <= 0)
	         {
	             memset(buf, '-', 1024);
		     send(connfd, buf, 1024, 0);
		     break;
	         }
	         s_ret = send(connfd, buf, r_ret, 0);
	         printf("send is %d bit \n", s_ret);
	    
	    }while(1);

	    close(fd);
	} 

    }
         memset(buf, 0, 1024);
         chdir("/home/cuijiaojiao/new/chonglai/ser");
}


//对下载路径中所有文件名的路径处理
int i=0;
void find_file(char dir_buf[])
{
	DIR            *dir;
	struct dirent  *entry;
	struct stat    statbuf;
	char 	       path[256];
	char 		newpath[256];
//	printf("%s\n",dir_buf);
        if((dir = opendir(dir_buf)) == NULL)
	{
	    perror("opendir");
	    exit(1);
	}
	chdir(dir_buf);
	printf("%s\n",dir_buf);
	
	while((entry = readdir(dir)) != NULL)
	{
	    if(lstat(entry->d_name, &statbuf) == -1)
	    {
	        perror(entry->d_name);
		exit(1);
	    }

	    if(S_ISDIR(statbuf.st_mode))
	    {

	        if(entry->d_name[0] == '.')
		{
		    continue;
		}
            } else {
		
		memset(path, 0, sizeof(path));
		getcwd(path, 256);
		strcpy(name[i].path,path);
//		strcat(filename, "/");
	        strcpy(name[i].filename, entry->d_name);
		i++;
//		printf("############\n");
	    }
	}
	chdir("..");
	closedir(dir);
//	printf("~~~~~~~~~~~~\n");
}




void sup_fun(int connfd, char recv_buf[], char send_buf[])
{
    char     filename[256];
    char     file[1024];
    char     path[256] = "/home/cuijiaojiao/test";

    int      w_ret, ret;
    int      rec_fd;
    int      i, j = 0, k;
    
    for( i = 0; i < strlen(recv_buf) + 1; i++)
    {
        if(recv_buf[i] == ' ')
	{
	    for(k = i + 1; k < strlen(recv_buf); j++, k++)
            {
	        filename[j] = recv_buf[k];
	    }
	    filename[j] = '\0';
	}
    }

    printf(" filename is %s\n", filename);

    chdir(path);

    if((rec_fd = open(filename, O_RDWR|O_CREAT, S_IRWXU)) == -1)
    {
        my_err("open", __LINE__);
    } else {
        printf("open success!\n");
    }

//    while(1)
    do
    {
        memset(file, 0, 1024);

	ret = recv(connfd, file, 1024, 0);

	printf("size is %d bites\n", ret);

	if(ret < 0)
	{
	    my_err("recv", __LINE__);
	}
	file[ret] = '\0';

	if(strncmp(file, "---", 3) == 0)
	{
	    printf("recive over !\n");
	    memset(file, 0, 1024);
	    break;
	}

	w_ret = write(rec_fd, file, ret);
	if(w_ret == -1)
	{
	    my_err("write", __LINE__);
	}

	printf("write is %d  bites\n", w_ret);
    }while(1);

    close(rec_fd);

    chdir("/home/cuijiaojiao/new/chonglai/ser");

}
