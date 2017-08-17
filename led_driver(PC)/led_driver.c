#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

//指定的主设备号
#define MAJOR_NUM 250

//自己的字符设备
struct mycdev
{
    int len;
    unsigned char buffer[50];
    struct cdev cdev;
}dev_led;

MODULE_LICENSE("GPL");

//设备号
static dev_t dev_num = {0};


//设备类
struct class *cls;

//打开设备
static int dev_led_open(struct inode *inode, struct file *file)
{
    printk("dev_led_open success!\n");
    return 0;
}

//读设备
static ssize_t dev_led_read(struct file *file, char __user *ubuf, size_t size, loff_t *ppos)
{
    printk("dev_led_read success!\n");
    return 0;
}

//写设备
static ssize_t dev_led_write(struct file *file, const char __user *ubuf, size_t size, loff_t *ppos)
{
    printk("dev_led_write success!\n");
    return 0;
}



//设备操作函数接口
static const struct file_operations led_operations = {
    .owner = THIS_MODULE,
    .open = dev_led_open,
    .read = dev_led_read,
    .write = dev_led_write,
};


//模块入口
int __init dev_led_init(void)
{
      int i = 0;
     int ret;
     struct device *device;


     //设备号 : 主设备号(12bit) | 次设备号(20bit)
     dev_num = MKDEV(MAJOR_NUM, 0);

    //动态注册设备号
    ret = alloc_chrdev_region(&dev_num,0,1,"dev_led");
    if(ret < 0){
        printk("Fail to register_chrdev_region\n");
    }
    //创建设备类
    cls = class_create(THIS_MODULE, "dev_led");
    if(IS_ERR(cls)){
        ret = PTR_ERR(cls);
    }

    //初始化字符设备
    cdev_init(&dev_led.cdev,&led_operations);

    //添加设备到操作系统
    ret = cdev_add(&dev_led.cdev,dev_num,1);

    //导出设备信息到用户空间(/sys/class/类名/设备名)
    device = device_create(cls,NULL,dev_num ,NULL,"dev_led",0);

    if(IS_ERR(device)){
        ret = PTR_ERR(device);
        printk("Fail to device_create\n");
    }

    printk("Register dev_led to system,ok!\n");
    return 0;
}

void __exit dev_led_exit(void)
{
    //删除sysfs文件系统中的设备
    device_destroy(cls,dev_num );


    //删除系统中的设备类
    class_destroy(cls);

    //从系统删除添加的字符设备
    cdev_del(&dev_led.cdev);
    //释放申请的设备号
    unregister_chrdev_region(dev_num, 1);
    printk("Exit dev_led ok!\n");
    return;
}

module_init(dev_led_init);
module_exit(dev_led_exit);
