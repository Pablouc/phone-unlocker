/****************************************************************************//**
*  \file       spi_driver.c
*
*  \details    Simple SPI driver to communicate with a custom Arduino
*
*  \author     kevv87, based on the work of EmbeTronicX
*
*******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>

#include "ssd1306.h"

static struct spi_device *connected_spi_device;

//Register information about your slave device
struct spi_board_info connected_spi_device_info = 
{
  .modalias     = "spi-arduino-driver",
  .max_speed_hz = 4000000,              // speed your device (slave) can handle
  .bus_num      = SPI_BUS_NUM,          // SPI 1
  .chip_select  = 0,                    // Use 0 Chip select (GPIO 18)
  .mode         = SPI_MODE_0            // SPI mode 0
};

/****************************************************************************
 * Details : This function writes the 1-byte data to the slave device using SPI.
 ****************************************************************************/
int arduino_spi_write( uint8_t data )
{
  int     ret = -1;
  uint8_t rx  = 0x00;
  
  if( connected_spi_device )
  {    
    struct spi_transfer  tr = 
    {
      .tx_buf  = &data,
      .rx_buf = &rx,
      .len    = 1,
    };

    spi_sync_transfer( connected_spi_device, &tr, 1 );
  }
  
  return( ret );
}

/****************************************************************************
 * Details : This function Register and Initilize the SPI.
 ****************************************************************************/
static int __init arduino_spi_init(void)
{
  int     ret;
  struct  spi_master *master;
  
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
    spi_unregister_device( etx_spi_device );    // Unregister the SPI slave
    pr_info("SPI driver Unregistered\n");
  }
}
 
module_init(arduino_spi_init);
module_exit(arduino_spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kevv87 <kzeledon@protonmail.com>");
MODULE_DESCRIPTION("A simple device driver for arduino thru the SPI Slave Protocol Driver");
MODULE_VERSION("0.1");
