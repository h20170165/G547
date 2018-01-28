#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include<asm/uaccess.h>

#define SUCCESS 0
#define FAILURE -1
#define OPEN 1
#define CLOSED 0

#define ADC_SET_CHANNEL _IOW(0,0,unsigned long)

static dev_t adc;
static struct cdev c_dev;
static struct class *cls;

static int ret_buffer;/*stores called function return status as int*/
static int channel_no;/*stores channel number after init*/
static int ret_status;/*stores current function return status*/
static void *ptr_buffer;/*stores called function return status as ptr*/
static bool dev_status;/*stores device file status*/

static int adc_open(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Process: %s\n",__FUNCTION__);
    if(dev_status==OPEN)
    {
        ret_status=-EBUSY;
        goto EXIT_POINT;
    }
    dev_status=OPEN;
    ret_status=SUCCESS;
    EXIT_POINT:
    return ret_status;
}
static int adc_close(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Process: %s\n",__FUNCTION__);
    dev_status=CLOSED;
    return SUCCESS;
}
static ssize_t adc_read(struct file *f,char __user *buffer,size_t length,loff_t *offset)
{
    char return_buffer[2];
    get_random_bytes(return_buffer,2);
    printk(KERN_INFO "Process: %s\n",__FUNCTION__);
    put_user(return_buffer[0],buffer);
    put_user(return_buffer[1],buffer+1);
    return SUCCESS;
}
static long adc_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    printk(KERN_INFO "Process: %s\n",__FUNCTION__);
    switch(cmd)
    {
        case ADC_SET_CHANNEL:
        channel_no=arg;
    }
    printk(KERN_INFO "ADC Channel changed to %d\n",channel_no);
    return SUCCESS;
}
static struct file_operations fops=
{
    .owner=THIS_MODULE,
    .open=adc_open,
    .release=adc_close,
    .read=adc_read,
    .unlocked_ioctl=adc_ioctl
};
static int __init adc_init(void)
{
    ret_buffer=alloc_chrdev_region(&adc,0,1,"ADCDEV");
    if(ret_buffer<0)
    {
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    cls=class_create(THIS_MODULE,"adcclass");
    if(cls==NULL)
    {
        unregister_chrdev_region(adc,1);
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    ptr_buffer=device_create(cls,NULL,adc,NULL,"adcdevice");
    if(ptr_buffer==NULL)
    {
        class_destroy(cls);
        unregister_chrdev_region(adc,1);
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    cdev_init(&c_dev,&fops);
    ret_buffer=cdev_add(&c_dev,adc,1);
    if(ret_buffer<0)
    {
        device_destroy(cls,adc);
        class_destroy(cls);
        unregister_chrdev_region(adc,1);
        ret_status=FAILURE;
        goto EXIT_POINT;
    }
    printk(KERN_INFO "ADC driver registered! MAJOR: %d MINOR: %d\n",MAJOR(adc),MINOR(adc));
    ret_status=SUCCESS;
    channel_no=0;/*Channel set to 0 by default*/
    dev_status=CLOSED;/*Device closed by default*/
    EXIT_POINT:
    return ret_status;
}
static void __exit adc_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cls,adc);
    class_destroy(cls);
    unregister_chrdev_region(adc,1);
    printk(KERN_INFO "ADC driver unregisterd!\n");
}
module_init(adc_init);
module_exit(adc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarthi Chugh");
MODULE_DESCRIPTION("Mock ADC Device Driver");
