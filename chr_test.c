#include<stdio.h>
#include<unistd.h>
#include<linux/fcntl.h>
#include<linux/ioctl.h>
#define TOPSECIO 0XEC
#define TOPSEC_GET_VERSION _IO(TOPSECIO,0X1)


int main()
{
int fd=open("/dev/topsec_test0",O_RDWR);
int version=ioctl(fd,TOPSEC_GET_VERSION,0); 
printf("version:%d\n",version);
printf("fd:%d\n",fd); 
close(fd);
}
