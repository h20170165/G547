#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#define ADC_SET_CHANNEL _IOW(0,0,unsigned long)
int main(int argc,char *argv[])
{
    int fd;
    int ret_val;
    char *buffer;
    buffer=(char*)malloc(sizeof(char)*2);
    fd=open("/dev/adcdevice",0);
    if(fd<0)
    {
        printf("Device Open Failed\n");
        exit(-1);
    }
    if(argc==1)
    {
        ret_val=read(fd,buffer,2);
        if(ret_val>=0)
        printf("ADC Value: %d\n",((unsigned char)buffer[0])*4+((unsigned char)buffer[1])/64);
        else
        printf("Device Read Failed\n");
    }
    if(argc==2 && argv[1][0]>='0' && argv[1][0]<='7')
    {
        ret_val=ioctl(fd,ADC_SET_CHANNEL,argv[1][0]-48);
        if(ret_val>=0)
        printf("Channel set to %d\n",argv[1][0]-48);
        else
        printf("Device IOCTL Failed\n");
    }
    close(fd);
    return 0;
}
