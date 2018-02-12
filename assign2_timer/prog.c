#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include<asm/io.h>

#define SUCCESS 0
#define FAILURE -1
#define TIMER0_CONTROL 0x43
#define TIMER0_CH0 0x40
#define TIMER0_CH1 0x41
#define TIMER0_CH2 0x42
#define CHANNEL_MASK 0xC0

#define TIMER0_SET_MODE _IOW(0,0,unsigned long)
#define TIMER0_SLEEP _IOW(0,1,unsigned long)

static dev_t timer0;
static struct cdev c_dev;
static struct class *cls;

static int ret_buffer;/*stores called function return status as int*/
static int ret_status;/*stores current function return status*/
static void *ptr_buffer;/*stores called function return status as ptr*/
static int port_address;/*stores timer port address*/

static int timer_open(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Timer0 Open()");
    return SUCCESS;
}
static int timer_close(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Timer0 Close()");
    return SUCCESS;
}
static ssize_t timer_read(struct file *f,char __user *buffer,size_t length,loff_t *offset)
{
    char return_buffer[2];
    printk(KERN_INFO "Timer0 Read()");
    return_buffer[0]=inb(port_address);
    return_buffer[1]=inb(port_address);
    put_user(return_buffer[0],buffer);
    put_user(return_buffer[1],buffer+1);
    return SUCCESS;
}
static inline void delay_ms(void)
{
int start_val,end_val;
start_val=inb(port_address);
start_val+=inb(port_address)*256;
while(1)
{
    end_val=inb(port_address);
    end_val+=inb(port_address)*256;
    if(end_val<=start_val && start_val-end_val>1193)
    break;
    if(end_val>start_val && start_val+65536-end_val>1193)
    break;
}
}
static inline void sleep(unsigned long millis)
{
    while(millis--)delay_ms();
}
static long timer_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    printk(KERN_INFO "Timer0 Ioctl()");
    switch(cmd)
    {
        case TIMER0_SET_MODE:
        outb(arg,TIMER0_CONTROL);
        switch(arg&CHANNEL_MASK)
        {
        case 0x00:port_address=TIMER0_CH0;break;
        case 0x40:port_address=TIMER0_CH1;break;
        case 0x80:port_address=TIMER0_CH2;break;
        case 0xC0:break;
        }
        printk(KERN_INFO "Timer0 Mode %02xH",(unsigned int)arg);
        break;
        
        case TIMER0_SLEEP:
        sleep(arg);
        printk(KERN_INFO "Timer0 Sleep %lu ms",arg);
        break;
    }
    return SUCCESS;
}
static struct file_operations fops=
{
    .owner=THIS_MODULE,
    .open=timer_open,
    .release=timer_close,
    .read=timer_read,
    .unlocked_ioctl=timer_ioctl
};
static int __init timer_init(void)
{
    ret_buffer=alloc_chrdev_region(&timer0,0,1,"TIMER0");
    if(ret_buffer<0)
    {
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    cls=class_create(THIS_MODULE,"timerclass");
    if(cls==NULL)
    {
        unregister_chrdev_region(timer0,1);
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    ptr_buffer=device_create(cls,NULL,timer0,NULL,"timerdevice");
    if(ptr_buffer==NULL)
    {
        class_destroy(cls);
        unregister_chrdev_region(timer0,1);
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    cdev_init(&c_dev,&fops);
    ret_buffer=cdev_add(&c_dev,timer0,1);
    if(ret_buffer<0)
    {
        device_destroy(cls,timer0);
        class_destroy(cls);
        unregister_chrdev_region(timer0,1);
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    printk(KERN_INFO "Timer0 driver registered! MAJOR: %d MINOR: %d\n",MAJOR(timer0),MINOR(timer0));
    port_address=TIMER0_CH0;
    ret_status=SUCCESS;
    EXIT_POINT:
    return ret_status;
}
static void __exit timer_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cls,timer0);
    class_destroy(cls);
    unregister_chrdev_region(timer0,1);
    printk(KERN_INFO "Timer0 driver unregisterd!\n");
}
module_init(timer_init);
module_exit(timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarthi Chugh");
MODULE_DESCRIPTION("Timer Read and Timer Sleep");
