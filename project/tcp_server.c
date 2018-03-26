/*************************************************************************
	> File Name: tcp_server.c
	> Author:XYK 
	> Mail: 
	> Created Time: Mon 19 Mar 2018 02:16:10 PM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
//#include"cam.h"
#include"m0_com.c"
void *thread_fun1(void *arg1);
tem_t tem;
hum_t hum;
char cmd;
int ret,ret1;
int connect_fd;
char recvbuf[100];
struct buffer
{
    char start[640*480];
    int length;
};
struct buffer *buffer;


    //传输温适度函数
    void *thread_fun1(void *arg1)
    {
        m0_com();
        while(1)
        {
            sleep(3);
        int tep=0;
        tep|=tem.ltem;
        tep|=tem.htem<<2;
        printf("%d\n",tep);
        //向客户端发送温湿度
        /*memset(&tem1,0,sizeof(tem1));
        memcpy(sendbuf,&tem1,sizeof(tem1));
        {
            ret_tem=write(arg1,sendbuf,sizeof(sendbuf));
            if(ret_tem < 0)
            {
                perror("send tem fail:");
                return ;
            }*/
        }
    }


int main()
{
    //创建套结字
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd < 0)
    {
        perror("create socket fail:");
        return -1;
    }
    //封装套结字
    struct sockaddr_in server_addr;
    server_addr.sin_family =AF_INET;
    server_addr.sin_port =htons(10000);
    server_addr.sin_addr.s_addr = inet_addr("192.168.2.153");

    //绑定套结字
    ret = bind(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if( ret < 0)
    {
        perror("bing fail:");
        return -1;
    }
    //监听套结字
    listen(sock_fd,6);
    printf("waiting for connnect....\n");

    //等待连接
    while(1)
    {
        connect_fd=accept(sock_fd,NULL,NULL);
        if(connect_fd < 0)
        {
            perror("connect fail:");
            return -1;
        }
        printf("new connect coming....\n");
   
    pthread_t  a_thread;
    

    //创建线程
    if(pthread_create(&a_thread,NULL,thread_fun1,(void*)&connect_fd) < 0)
        {
            perror("create thread1 fail:\n");
            return -1;
        }

    //接收客户端命令
    while(1)
    {
        memset(recvbuf,0,sizeof(recvbuf));
        ret1=read(connect_fd,recvbuf,1);
        if( ret1 <0 )
        {
            perror("read recvbuf fail:");
            return -1;
        }
        cmd=recvbuf[0];
    }

    }


    return 0;
}
