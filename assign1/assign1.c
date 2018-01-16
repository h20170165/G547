#include<linux/init.h>
#include<linux/module.h>
int assign1_init(void)
{
    printk(KERN_ALERT "Alohomora\n\n");
    return 0;
}
int assign1_exit(void)
{
    printk(KERN_ALERT "Avada Kedavra\n\n");
    return 0;
}
module_init(assign1_init);
module_exit(assign1_exit);
