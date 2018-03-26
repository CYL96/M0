/*************************************************************************
	> File Name: m0.c
	> Author:LCY 
	> Mail: 
	> Created Time: Mon 19 Mar 2018 05:25:27 PM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<strings.h>
#include<termios.h> 
#include<sys/types.h>   
#include<sys/stat.h>
#include <sys/ioctl.h>
#include<fcntl.h>
#include <termios.h>
#include<poll.h>
int m0_open(int comport)
{
    char *dev[]={"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2"};
    int fd= open(dev[comport],O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd==-1)
    {
        perror("can't open port!");
        return -1;
    }
    if(fcntl(fd,F_SETFL,0)<0)
    {
        printf("fcntl filed\n");
        return -1;
    }
    printf("m0->Fcntl=%d\n",fcntl(fd, F_SETFL,0));
    if(0 == isatty(fd))  
    {  
        printf("Standard input is not a terminal device\n");  
        return -1;
    }  
    else
    {  
        printf("Isatty success!\n");  
    }                
    printf("m0->fd = %d\n",fd); 
    return fd;
}
int m0_close(int  fd)
{
    close(fd);
}
int m0_set(int fd)
{
    struct termios options;
    if(tcgetattr(fd,&options)!=0)
    {
        perror("tcgetattr");
        return -1;
    }
    cfsetispeed(&options,B115200);//setbps
    cfsetospeed(&options,B115200);
    //SET FLAG
    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;
    options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_iflag &= ~(ICRNL | INLCR);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    
    options.c_cflag &= ~CRTSCTS;//不使用流控制
    options.c_cflag &= ~CSIZE;//接受8bit
    options.c_cflag |= CS8;

    options.c_cflag &= ~PARENB;//设置无奇偶校验位
    options.c_iflag &= ~INPCK;

    options.c_cflag &= ~CSTOPB;
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 1;

    tcflush(fd,TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&options) != 0)    
    {  
        perror("com set error!\n");    
        return -1;   
    }  
    printf("m0 set success\n");
    return 0;
}
int m0_init(int comport)
{
    int fd =-1;
    if((fd=m0_open(comport))<0)
    {
        printf("m0_open filed\n");
        return -1;
    }

    if(m0_set(fd)<0)
    {
        printf("m0_set filed\n");
        return -1;
    }
    return fd;
}
int m0_send(int fd,void *p,int len,int timeout)
{
    static struct pollfd fds;
    fds.fd = fd;
    fds.events =POLLOUT;
    fds.revents=0;

    int ret =  poll(&fds , 1,timeout);
    if(0>=ret)
    {
        perror("poll");
        return -1;
    }
    if(fds.revents & POLLOUT)
    {
        return write(fd,p,len);
    }
    printf("unkonw err1.\r\n");
    return -1;
}
int m0_recv(int fd, void *p,int len ,int timeout)
{
    static struct pollfd fds;
    fds.fd =fd;
    fds.events=POLLIN;
    fds.revents=0;

    int ret = poll(&fds, 1, timeout);

    if(fds.revents & POLLIN)
    {
        return read(fd, p, len);
    }
}
