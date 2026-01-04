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
static struct cdev etx_cdev;

static int my_open(struct inode *inode,struct file *filp){
//here we can get major and minior numbers usieng struct inode 
printk(KERN_INFO "inside open funtion MAJOR %d MINOR %d",MAJOR(dev),MINOR(dev));
printk(KERN_INFO "hello flip-f_pos %lld\n",filp->f_pos);
printk(KERN_INFO "hello flip-f_mode 0x%x\n",filp->f_mode);
printk(KERN_INFO "hello flip-f_flags 0x%x",filp->f_flags);

return 0 ;
}
static int my_release(struct inode *inode,struct file *filp){
pr_info("hello -file is closed now");
return 0;
}
static int read(struct file *filp,char __user *buf,size_t len,loff_t *off){
pr_info("Driver Read function");
return 0;
}

static struct file_operations fops={
.open=my_open,
.release=my_release,
.read=read,
};


static int __init hello_world_init(void)
{
  
 printk(KERN_INFO "Welcome Mahendra");
  
  if((alloc_chrdev_region(&dev,0,Dev_count,"simple_char_driver")<0)){
    printk(KERN_INFO "failed to allocate minor number");
}

//printk(KERN_INFO "Major=%d Minor=%d\n",MAJOR(dev),MINOR(dev));
cdev_init(&etx_cdev,&fops);

if((cdev_add(&etx_cdev,dev,Dev_count))<0){
pr_err("Canot create device in sys");
goto r_class;
}



dev_class=class_create("my_first_char_driver");

if(IS_ERR(dev_class)){
printk("canot create a struct class");
goto r_class;
}

//here we are creating number of device files the below will creted a two device file 

for (int i = 0; i < Dev_count; i++) {
    if(IS_ERR(device_create(dev_class, NULL,MKDEV(MAJOR(dev),MINOR(dev)+i),NULL,"my_first_char_driver_%d", i))){
printk(KERN_INFO "cannot create the device");
goto r_device;
}
}
printk("driver register succeful with sys");
  return 0;
r_device:
            class_destroy(dev_class);
r_class:
             
              unregister_chrdev_region(dev,Dev_count); 
              return -1;
  
}
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    for(int i=0; i<Dev_count;i++){
              device_destroy(dev_class,MKDEV(MAJOR(dev),MINOR(dev)+i));
              }
   class_destroy(dev_class);
   cdev_del(&etx_cdev);
   unregister_chrdev_region(dev, 1);
   
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("2:1.0");
