/****************************************************************************//**
*  \file       spi_driver.c
*
*  \details    Simple SPI driver to communicate with a custom Arduino
*
*  \author     kevv87, based on the work of EmbeTronicX
*
*******************************************************************************/
#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>  //copy_to/from_user()
#include <linux/err.h>


static struct spi_device *connected_spi_device;
dev_t dev = 0;
static struct cdev arduino_spi_cdev;
static struct class *dev_class;
static struct cdev arduino_spi_cdev;
uint8_t state = 0;

/*************** Driver functions **********************/
static int arduino_spi_open(struct inode *inode, struct file *file);
static int arduino_spi_release(struct inode *inode, struct file *file);
static ssize_t arduino_spi_read(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t arduino_spi_write(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/******************************************************/
 
//File operation structure 
static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = arduino_spi_read,
  .write          = arduino_spi_write,
  .open           = arduino_spi_open,
  .release        = arduino_spi_release,
};

static ssize_t arduino_spi_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  if ( copy_to_user(buf, &state, 1) ) {
    pr_err("ERROR: Not all the bytes have been copied to user\n");
  }
  
  pr_info("Executed read function!");
  return 0;
}

static int arduino_spi_release(struct inode *inode, struct file *file)
{
  pr_info("Device File Closed...!!!\n");
  return 0;
}

static int arduino_spi_open(struct inode *inode, struct file *file)
{
  pr_info("Device File Opened...!!!\n");
  return 0;
}

//Register information about your slave device
struct spi_board_info connected_spi_device_info = 
{
  .modalias     = "spi-arduino-driver",
  .max_speed_hz = 4000000,              // speed your device (slave) can handle
  .bus_num      = 1,          // SPI 1
  .chip_select  = 0,                    // Use 0 Chip select (GPIO 18)
  .mode         = SPI_MODE_0            // SPI mode 0
};

/****************************************************************************
 * Details : This function writes the 1-byte data to the slave device using SPI.
 ****************************************************************************/
int arduino_spi_write( struct file *filp,
		const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};

  if ( copy_from_user( rec_buf, buf, len) > 0 ) {
    pr_err("ERROR: Not all the bytes have been copied from user \n");
  }

  pr_info("Write function: GPIO_21 Set = %c\n", rec_buf[0]);
  
  if( connected_spi_device )
  {    
    if (spi_write( connected_spi_device, &rec_buf[0], 1) < 0) {
      pr_err("ERROR: Error while writing\n");
    }
  }
  
  return len;
}

/****************************************************************************
 * Details : This function Register and Initilize the SPI.
 ****************************************************************************/
static int __init arduino_spi_init(void)
{
  int     ret;
  struct  spi_master *master;
  
  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "arduino_spi_Dev")) <0){
    pr_err("Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
  /*Creating cdev structure*/
  cdev_init(&arduino_spi_cdev, &fops);
 
  /*Adding character device to the system*/
  if((cdev_add(&arduino_spi_cdev,dev,1)) < 0){
    pr_err("Cannot add the device to the system\n");
    goto r_del;
  }
 
  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"arduino_spi_class"))){
    pr_err("Cannot create the struct class\n");
    goto r_class;
  }
 
  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"arduino_spi_device"))){
    pr_err( "Cannot create the Device \n");
    goto r_device;
  }

  master = spi_busnum_to_master( connected_spi_device_info.bus_num );
  if( master == NULL )
  {
    pr_err("SPI Master not found.\n");
    return -ENODEV;
  }
   
  // create a new slave device, given the master and device info
  connected_spi_device = spi_new_device( master, &connected_spi_device_info );
  if( connected_spi_device == NULL ) 
  {
    pr_err("FAILED to create slave.\n");
    return -ENODEV;
  }
  
  // 8-bits in a word
  connected_spi_device->bits_per_word = 8;

  // setup the SPI slave device
  ret = spi_setup( connected_spi_device );
  if( ret )
  {
    pr_err("FAILED to setup slave.\n");
    spi_unregister_device( connected_spi_device );
    return -ENODEV;
  }
  
  pr_info("SPI driver Registered\n");
  return 0;

r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&arduino_spi_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  
  return -1;
}
 
/****************************************************************************
 * Name: etx_spi_exit
 *
 * Details : This function Unregister and DeInitilize the SPI.
 ****************************************************************************/
static void __exit arduino_spi_exit(void)
{ 
  if( connected_spi_device )
  {
    spi_unregister_device( connected_spi_device );    // Unregister the SPI slave
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&arduino_spi_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("SPI driver Unregistered\n");
  }
}
 
module_init(arduino_spi_init);
module_exit(arduino_spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kevv87 <kzeledon@protonmail.com>");
MODULE_DESCRIPTION("A simple device driver for arduino thru the SPI Slave Protocol Driver");
MODULE_VERSION("0.1");
