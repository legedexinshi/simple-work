#include <iostream>
#include <thread>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include "my_pipe.h"

using namespace std;

int test1() {
	int pipe_fd[2];
	pid_t pid;
	char r_buf[100];
	char w_buf[4];
	char* p_wbuf;
	int r_num;
	int cmd;
	
	memset(r_buf,0,sizeof(r_buf));
	memset(w_buf,0,sizeof(r_buf));
	p_wbuf=w_buf;
	if(myPipe::pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	
	if((pid=fork())==0)
	{
		printf("\n");
		myPipe::close(pipe_fd[1]);
		sleep(3);//确保父进程关闭写端
	    r_num=myPipe::read(pipe_fd[0],r_buf,100);
		printf(	"read num is %d   the data read from the pipe is %d\n",r_num,atoi(r_buf));
		
		myPipe::close(pipe_fd[0]);
		exit(0);
	}
	else if(pid>0)
	{
		myPipe::close(pipe_fd[0]);//read
		strcpy(w_buf,"111");
		if(myPipe::write(pipe_fd[1],w_buf,4)!=-1)
			printf("parent write over\n");
		myPipe::close(pipe_fd[1]);//write
			printf("parent close fd[1] over\n");
	}
	return 0;
} 

int test2() {
	int pipe_fd[2];
	pid_t pid;
	char r_buf[4096];
	char w_buf[4096*2];
	int writenum;
	int rnum;
	memset(r_buf,0,sizeof(r_buf));	
	if(myPipe::pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	
	if((pid=fork())==0)
	{
		myPipe::close(pipe_fd[1]);
		while(1)
		{
			sleep(1);	
			rnum=myPipe::read(pipe_fd[0],r_buf,1000);
			printf("child: readnum is %d\n",rnum);
			if (rnum == 0) break;
		}
		myPipe::close(pipe_fd[0]);
		
		exit(0);
	}
	else if(pid>0)
	{
		myPipe::close(pipe_fd[0]);//write
		memset(r_buf,0,sizeof(r_buf));	
		if((writenum=myPipe::write(pipe_fd[1],w_buf,1024))==-1)
			printf("write to pipe error\n");
		else	
			printf("the bytes write to pipe is %d \n", writenum);
		writenum=myPipe::write(pipe_fd[1],w_buf,4096);
		close(pipe_fd[1]);
	}
	return 0;
}

int test_fifo() {
	int pipe_fd[2];
	pid_t pid;
	char r_buf[4096];
	char w_buf[4096];
	int writenum;
	int rnum;
	
	if((pid=fork())==0)
	{
		if (myPipe::pipe(pipe_fd, "path123") == -1) {
			printf("open pipe error\n");
			exit(0);
		}
		myPipe::close(pipe_fd[1]);
		while(1)
		{
			sleep(1);	
			rnum=myPipe::read(pipe_fd[0],r_buf,1000);
			printf("child: readnum is %d\n",rnum);
			if (rnum == 0) break;
		}
		myPipe::close(pipe_fd[0]);
		
		exit(0);
	}
	else if(pid>0)
	{
		if (myPipe::pipe(pipe_fd, "path123") == -1) {
			printf("open pipe error\n");
			exit(0);
		}
		myPipe::close(pipe_fd[0]);//write
		memset(r_buf,0,sizeof(r_buf));	
		if((writenum=myPipe::write(pipe_fd[1],w_buf,1024))==-1)
			printf("write to pipe error\n");
		else	
			printf("the bytes write to pipe is %d \n", writenum);
		writenum=myPipe::write(pipe_fd[1],w_buf,4096);
		close(pipe_fd[1]);
	}
	return 0;
}

int main() {
	//test1();
	//test2();
	test_fifo();
}
