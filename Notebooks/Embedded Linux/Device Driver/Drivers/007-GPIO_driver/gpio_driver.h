/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  gpio_driver.h
 *
 *  Description: This file contains the header files and the function prototypes for the gpio_driver.c file.
 *
 *********************************************************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/**********************************************************************************************************************
 * DEFINITIONS
 *********************************************************************************************************************/

#define BUFFER_SIZE         2
#define NO_OF_DRIVERS       1
#define NO_OF_DIOS_USED     2
#define GPIO_START_PIN      5           // GPIO Pins are sequential starting from 5

/* Permission codes */
#define RDONLY 0x01
#define WRONLY 0X10
#define RDWR   0x11

/**********************************************************************************************************************
 * STRUCTURE TYPE DEFINITIONS
 *********************************************************************************************************************/
/* Device structure (Minor) */
typedef struct {
    const char* name;           // holds the device name
    unsigned id;                // holds the device id
    char* buffer;               // holds the device buffer
    int perms;                  // holds the permissions
    unsigned g_offset;          // holds the global offset
    size_t size;                // holds the size of the device buffer
    struct cdev cDev;

} pcDevice_private_data_t;

/* Driver structure (Major) */
typedef struct {
    int no_of_devices;              // Number of devices
    dev_t dev_num;                  // Device number
    struct class *class_pcd;       // will appear under `/sys/class`
    struct device *device_pcd;     // will appear under `/dev`
    pcDevice_private_data_t pcdev_data[NO_OF_DIOS_USED];

} pcDriver_private_data_t;

/**********************************************************************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**
 * check_openPermissions() - This function checks the permissions of the device file
 * @brief: This function is called when the file is opened which if device required permissions meets the user permissions
 *         will return success, leading to calling either read() or write()
 * @param dev_perm: device permissions (RDONLY, WRONLY, RDWR)
 * @param access_mode: user file access mode (FMODE_READ, FMODE_WRITE)
 * @return 0 on success, -EPERM on failure
 */
int check_openPermissions(int dev_perm, int access_mode);

/**
 * pcd_open() - This function shall be called when the file is opened
 * @brief: This function shall be called when the file is opened which if successful, will call either read() or write()
 * @param inode: pointer to the inode structure
 * @param filp: pointer to the file structure
 * @return 0 on success, -EPERM on failure
 */
int pcd_open(struct inode *inode, struct file *filp);

/**
 * pcd_read() - This function shall be called when the file is read
 * @brief: This function shall be called when the file is read which if successful, will read the device buffer
 * @param filp: pointer to the file structure
 * @buffer: pointer to the buffer
 * @param size: size of the buffer
 * @param f_pos: offset of the buffer
 * @return number of bytes read
 */
ssize_t pcd_read(struct file *filp, char __user *buffer, size_t size, loff_t *f_pos);

/**
 * pcd_write() - This function shall be called when the file is to be written
 * @brief: This function shall be called when the file is to be written which if successful, will write to the device buffer
 * @param filp: pointer to the file structure
 * @param buffer: pointer to the buffer
 * @param size: size of the buffer
 * @param f_pos: offset of the buffer
 * @return number of bytes written
 */
ssize_t pcd_write(struct file *filp, const char __user *buffer, size_t size, loff_t *f_pos);

/**
 * pcd_release() - This function shall be called when the file is closed
 * @brief: This function shall be called when the file is closed which if successful, will close the file
 * @param inode: pointer to the inode structure
 * @param filp: pointer to the file structure
 * @return 0 on success
 */
int pcd_release(struct inode *inode, struct file *filp);


#endif /* _GPIO_H_ */