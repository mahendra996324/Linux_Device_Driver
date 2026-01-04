#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h> 
#include<linux/err.h>
#include<linux/device.h>
#include<linux/cdev.h>


/*
** Module Init function
*/
#define Dev_count 2
dev_t dev = 0;
static struct class *dev_class;
static struct cdev mahi;
static struct file_operations fops;
static int __init hello_world_init(void)
{
  printk(KERN_INFO "Welcome Mahendra");
   if((alloc_chrdev_region(&dev,0,Dev_count,"simple_char_driver")<0)){
    printk(KERN_INFO "failed to allocate minor number");
}
printk(KERN_INFO "Major=%d Minor=%d\n",MAJOR(dev),MINOR(dev));

cdev_init(&mahi,&fops);
printk("pass");
cdev_add(&mahi,dev,Dev_count);

dev_class=class_create("my_first_char_driver");

if(IS_ERR(dev_class)){
printk("canot create a struct class");
goto remove_class;
}

for (int i = 0; i < Dev_count; i++) {
    if(IS_ERR(device_create(dev_class, NULL,MKDEV(MAJOR(dev), i),NULL,"my_first_char_driver_%d", i))){
printk(KERN_INFO "cannot create the device");
goto remove_device;
}
}

remove_class:
            class_destroy(dev_class);
remove_device:
             for(int i=0; i<Dev_count;i++){
              device_destroy(dev_class,MKDEV(MAJOR(dev),i));
              }
              unregister_chrdev_region(dev,Dev_count); 
    return 0;
}
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
   unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("2:1.0");
