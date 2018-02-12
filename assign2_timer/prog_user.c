#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define TIMER0_SET_MODE _IOW(0,0,unsigned long)
#define TIMER0_SLEEP _IOW(0,1,unsigned long)

int main(int argc,char *argv[])
{
    int fd;
    int ret_val;
    char *buffer;
    unsigned long arg;
    buffer=malloc(2);
    fd=open("/dev/timerdevice",0);
    if(fd<0)
    {
        printf("Timer0 Open Failed\n");
        exit(-1);
    }
    if(argc==2 && !strcmp(argv[1],"READ"))
    {
        ret_val=read(fd,buffer,2);
        if(ret_val>=0)
        printf("Timer0 Value: %d\n",((unsigned char)buffer[0])+((unsigned char)buffer[1])*256);
        else
        printf("Timer0 Read Failed\n");
    }
    if(argc==2 && !strcmp(argv[1],"SETMODE"))
    {
    	printf("SETMODE $");
    	scanf("%lu",&arg);
        ret_val=ioctl(fd,TIMER0_SET_MODE,arg);
        if(ret_val>=0)
        printf("Timer0 mode set\n");
        else
        printf("Timer0 mode set Failed\n");
    }
    if(argc==2 && !strcmp(argv[1],"SLEEP"))
    {
    	printf("SLEEP $");
    	scanf("%lu",&arg);
        ret_val=ioctl(fd,TIMER0_SLEEP,arg);
        if(ret_val>=0)
        {
        printf("Timer0 woke up\n");
        }
        else
        printf("Timer0 can't sleep\n");
    }
    close(fd);
    free(buffer);
    return 0;
}
