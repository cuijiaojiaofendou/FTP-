/*
 * =====================================================================================
 *
 *       Filename:  diary.c
 *
 *    Description:  系统日志，记录客户端的操作
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 21时56分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  cuijiaojiao, cuijiaojiaofendou@gmail.com
 *        Company:  Class 1204 of Software
 *
 * =====================================================================================
 */

#include "tranfic.h"
#include <time.h>

//锁的设置或释放函数
int lock_set(int fd, struct flock *lock)
{
    if(fcntl(fd, F_SETLK, lock) == 0)
    {
        if(lock->l_type == F_RDLCK)  //读锁
	{
	    printf("set read lock, pid : %d\n", getpid());
	} else if(lock->l_type == F_WRLCK) {
	    
	    printf("set write lock, pid:%d\n", getpid());
	} else if(lock->l_type == F_UNLCK) {       //解锁
	
	    printf("release lock, pid :%d\n", getpid());
	}
    } else {
        
	 perror("lock operation fail\n");
	 return -1;
    }

    return 0;
}

//测试锁，只有当测试发现参数lock 指定的锁能被设置时，返回0
int lock_test(int fd, struct flock *lock)
{
    if(fcntl(fd, F_GETLK, lock) == 0)
    {
        if(lock->l_type == F_UNLCK)
	{
	    printf("lock can be set in fd\n");
	    return 0;
	} else {
	 
	    if(lock->l_type == F_RDLCK)
	    {
	        printf("can't set lock, read lock has been set by :%d\n", lock->l_pid);
	    } else if(lock->l_type == F_WRLCK) {
	    
		printf("can't set lock, write lock has been set by :%d\n", lock->l_pid);
	    }

	    return -2;
	}
    } else {
    
	perror("get incompatible locks fails\n");
	return -1;
    }
}

void diary(char com_word[], char cli_ip[])
{
     //struct stat *buf;
     char 	*buftime;
     time_t     ptr;
     int 	fd, ret;
     struct flock    lock;

     if((fd = open("system_log.txt", O_CREAT|O_APPEND|O_RDWR, 0644)) == -1)
     {
         perror("open");
     }

     memset(&lock, 0 , sizeof(struct flock));
     lock.l_start = SEEK_SET;
     lock.l_whence = 0;
     lock.l_len = 0;

     //设置写锁
     lock.l_type = F_WRLCK;
     if(lock_test(fd, &lock) == 0)    //测试可以设置锁
     {
         lock.l_type = F_WRLCK;
	 lock_set(fd, &lock);
     }
     
     time(&ptr);
     buftime = ctime(&ptr);     //对时间的解析

     //将命令，ip,文件变动时间写入文件里
     if(write(fd, com_word, strlen(com_word)) < 0)
     {
         perror("write");
	 exit(1);
     }

     printf("%s\n", com_word);

     if(write(fd, cli_ip, strlen(cli_ip)) < 0)
     {
          my_err("write", __LINE__);
     }
     
     printf("%s\n", cli_ip);
     if(write(fd, buftime, strlen(buftime)) < 0)
     {
         my_err("write", __LINE__);
     }

     printf("buftime is :%s\n", buftime);


     //释放锁
     lock.l_type = F_UNLCK;
     lock_set(fd, &lock);

     close(fd);
}
