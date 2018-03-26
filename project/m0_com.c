/*************************************************************************
	> File Name: m0_com.c
	> Author:LCY 
	> Mail: 
	> Created Time: Mon 19 Mar 2018 08:42:22 PM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<strings.h>
#include<pthread.h>
#include"m0.c"
#include"m0_struct.c"
uint8_t idd;
static char lcmd='0';
extern tem_t tem;
extern hum_t hum;
extern char cmd;
void *m0_recv_data(void *arg)
{
printf("recv thread successed\n");
    int len=0,num=0;
    int fd=(int)arg;
    printf("recv from fd:%d\n",fd);
    uint8_t recvbuf[36];
    uint8_t *p;
    int err=0;
    struct pollfd pfd=
    {
	.fd=fd,
	.events=POLLIN,
	.revents=0,
    };
    while(1)
    {
	int ret = poll(&pfd, 1, -1);
	if(POLLIN & pfd.revents)
	{

        memset(recvbuf,0,36);
        p=recvbuf;
        num=m0_recv(fd,p,1,1000);
        if(num<0)
        {
            printf("recv filed!\n");
            err++;
            continue;
        }
        if(p[0] != 0XBB)
        {
            printf("  ");
            continue;
        }
        p++;
        len=35;
        while(len)
        {
            num = m0_recv(fd, p, len, -1);
            if(0 > num)
            {
                printf("recv fail.\n");
                return ;
            }
            len -=num;
            p+=num;
        }
        printf("recv:");
        int i;
        for(i = 0; i < 36; i++){
            printf("%.2X ", recvbuf[i]);
        }
        printf("\n");
        tem.ltem=recvbuf[4];
        tem.htem=recvbuf[5];
        hum.lhum=recvbuf[6];
        hum.hhum=recvbuf[7];
        idd=recvbuf[1];
	}
    }
return 0;
}
int led_on(int fd,char recvbuf[32])
{
    recvbuf[4] =0x00;
    int num=m0_send(fd,recvbuf,32,-1);
    if(32 != num)
    {
	printf("send fail.\n");
	return -1;
    }
    return 0;

}
int led_off(int fd,char recvbuf[32])
{
    recvbuf[4]= 0x01;
    m0_send(fd,recvbuf,32,-1);
    return 0;
}
int fan_on(int fd,char recvbuf[32])
{
    recvbuf[4] = 0x04;
    m0_send(fd,recvbuf,32,-1);
    return 0;
}
int fan_off(int fd,char recvbuf[32])
{
    recvbuf[4] = 0x08;
    m0_send(fd,recvbuf,32,-1);
    return 0;
}
int buz_on(int fd,char recvbuf[32])
{
    recvbuf[4] =0x02;
    m0_send(fd,recvbuf,32,-1);
    return 0;
}
int buz_off(int fd,char recvbuf[32])
{
    recvbuf[4] =0x03;
    m0_send(fd,recvbuf,32,-1);
    return 0;
}
int all_off(int fd,char recvbuf[32])
{
    led_off(fd,recvbuf);
    fan_off(fd,recvbuf);
    buz_off(fd,recvbuf);
    return 0;
}
void *m0_send_data(void *arg)
{
    uint8_t sendbuf[32]={0XDD,0x00,0x24,0x00,0x00};
    int fd = (int)arg;
    int i=0;
    while(1)
    {
     sendbuf[1]=idd;
    if(cmd!=lcmd)
    {
        lcmd=cmd;
    switch(cmd-'a')
    {
        case 0:led_on(fd,sendbuf);break;
        case 1:led_off(fd,sendbuf);break;
        case 2:fan_on(fd,sendbuf);break;
        case 3:fan_off(fd,sendbuf);break;
        case 4:buz_on(fd,sendbuf);break;
        case 5:buz_off(fd,sendbuf);break;
        case 6:all_off(fd,sendbuf);break;
        default:break;
    }
if(lcmd >= 'a' && lcmd <= 'g')
{
    printf("send:");
    for(i=0;i<32;i++)
    {
        printf("%.2x ",sendbuf[i]);
    }
    printf("\n");
    }}
    }

}
int m0_com()
{
    int fd= m0_init(0);
    if(fd<0)
    {
        perror("Serial Init Error");
        return -1;
    }
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,m0_recv_data,(void *)fd);
    pthread_create(&tid2,NULL,m0_send_data,(void *)fd);

    while(1)
    {
    }
return 0;

}


