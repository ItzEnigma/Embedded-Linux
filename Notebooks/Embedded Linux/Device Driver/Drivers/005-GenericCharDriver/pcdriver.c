/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  pcdriver.C
 *
 *  Description: This file contains the implementation of the functions for the pcdriver.h file and the main kernel
 *               module functions (init and exit), and file operations functions definitions.
 *
 *********************************************************************************************************************/
#include "pcdriver.h"

/************************************* GLOBAL VARIABLES *************************************/

/* pseudo device's memory */
char device_buffer_pcdev1[MEM_SIZE_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_PCDEV4];
char device_buffer_pcdev5[MEM_SIZE_PCDEV5];

/* ---------- Driver/Devices ---------- */
static pcDriver_private_data_t pcdrv_data = {
    .no_of_devices = NO_OF_DEVICES,
    .pcdev_data = {
        [0] = {
            .name = "enigma_dio1_val",
            .id = 0,
            .perms = RDONLY,
            .buffer = device_buffer_pcdev1,
            .size = MEM_SIZE_PCDEV1
        },
        [1] = {
            .name = "enigma_dio1_dir",
            .id = 1,
            .perms = RDWR,
            .buffer = device_buffer_pcdev2,
            .size = MEM_SIZE_PCDEV2
        },
        [2] = {
            .name = "enigma_char3",
            .id = 2,
            .perms = RDWR,
            .buffer = device_buffer_pcdev3,
            .size = MEM_SIZE_PCDEV3
        },
        [3] = {
            .name = "enigma_char4",
            .id = 3,
            .perms = RDWR,
            .buffer = device_buffer_pcdev4,
            .size = MEM_SIZE_PCDEV4
        },
        [4] = {
            .name = "enigma_char5",
            .id = 4,
            .perms = WRONLY,
            .buffer = device_buffer_pcdev5,
            .size = MEM_SIZE_PCDEV5
        }
    }
};
/* ------------------------------------ */
/* File operations structure:
 * Setting the file operations functions to the defined functions
*/
static struct file_operations fops = {
    .open = pcd_open,
    .release = pcd_release,
    .read = pcd_read,
    .write = pcd_write,
};

/***********************************************************************************************/


/************************************* FUNCTION DEFINITIONS *************************************/

/**
 * check_openPermissions() - This function checks the permissions of the device file
 * @brief: This function is called when the file is opened which if device required permissions meets the user permissions
 *         will return success, leading to calling either read() or write()
 * @param dev_perm: device permissions (RDONLY, WRONLY, RDWR)
 * @param access_mode: user file access mode (FMODE_READ, FMODE_WRITE)
 * @return 0 on success, -EPERM on failure
 */
int check_openPermissions(int dev_perm, int access_mode)
{
    /* Return:   0      --> allowed
     *          -EPERM  --> operation not permitted
     */

    if(dev_perm == RDWR)
    {
        return 0;
    }

    /* Read only */
    if((dev_perm == RDONLY) && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
    {
        return 0;
    }

    /* Write only */
    if((dev_perm == WRONLY) && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
    {
        return 0;
    }

    /* Operation not permitted */
    return -EPERM;
}

/**
 * pcd_open() - This function shall be called when the file is opened
 * @brief: This function shall be called when the file is opened which if successful, will call either read() or write()
 * @param inode: pointer to the inode structure
 * @param filp: pointer to the file structure
 * @return 0 on success, -EPERM on failure
 */
int pcd_open(struct inode *inode, struct file *filp)
{
    int minor_n;
    int r_err = 0;

    /* Get the minor number */
    minor_n = MINOR(inode->i_rdev);
    pr_info("[%s] - Minor number = %d\n", __func__, minor_n);
    
    /* Get device's private data */
    pcDevice_private_data_t *pcdev_data = container_of(inode->i_cdev, pcDevice_private_data_t, cDev);

    /* Save device's private data in filp */
    filp->private_data = pcdev_data;

    /* Check open permissions */
    r_err = check_openPermissions(pcdev_data->perms, filp->f_mode);
    if(r_err)
    {
        pr_info("[%s] - Operation not permitted\n", __func__);
        return r_err;
    }

    pr_info("[%s] - Device file opened\n", __func__);

    return r_err;
}

/**
 * pcd_read() - This function shall be called when the file is read
 * @brief: This function shall be called when the file is read which if successful, will read the device buffer
 * @param filp: pointer to the file structure
 * @buffer: pointer to the buffer
 * @param size: size of the buffer
 * @param f_pos: offset of the buffer
 * @return number of bytes read
 */
ssize_t pcd_read(struct file *filp, char __user *buffer, size_t size, loff_t *f_pos)
{
    pcDevice_private_data_t *local_pcdev_data = (pcDevice_private_data_t*)filp->private_data;
    size_t dev_size = local_pcdev_data->size;

    // Check if offset is beyond the end of the device buffer
    if (*f_pos >= dev_size) return 0;

    pr_info("[%s] - Current file position = %lld\n", __func__, *f_pos);
    
    
    if (size + *f_pos > dev_size) // Check if the size is bigger than the device buffer
    {
        size = dev_size - *f_pos;
    }

    if(copy_to_user(buffer, local_pcdev_data->buffer+(*f_pos), size))
    {
        pr_err("Failed to copy data to user\n");
        return -EFAULT;
    }

    *f_pos += size; // Update the offset

    pr_info("[%s] - Number of bytes successfully read = %ld\n", __func__, size);

    /* Return the number of bytes read from the file */
    return size;
}

/**
 * pcd_write() - This function shall be called when the file is to be written
 * @brief: This function shall be called when the file is to be written which if successful, will write to the device buffer
 * @param filp: pointer to the file structure
 * @param buffer: pointer to the buffer
 * @param size: size of the buffer
 * @param f_pos: offset of the buffer
 * @return number of bytes written
 */
ssize_t pcd_write(struct file *filp, const char __user *buffer, size_t size, loff_t *f_pos)
{
    pcDevice_private_data_t *local_pcdev_data = (pcDevice_private_data_t*)filp->private_data;
    size_t dev_size = local_pcdev_data->size;

    /* To store the value which is to be checked (for DIO Direction and Value) */
    char value[2];

    pr_info("[%s] - Current file position = %lld\n", __func__, *f_pos);

    // Check if offset is beyond the end of the device buffer
    if (*f_pos >= dev_size) return -ENOSPC; // No space left on device

    if (size + *f_pos > dev_size) // Check if the size is bigger than the device buffer
    {
        size = dev_size - *f_pos;
    }

    // Check if the file is opened with O_APPEND flag
    if (filp->f_flags & O_APPEND) {
        *f_pos = local_pcdev_data->g_offset; // Set offset to the end of the device buffer
    }
    else    // clear the device buffer (important for rewriting)
    {
        memset(local_pcdev_data->buffer, 0, dev_size); // Clear the device buffer
    }

    /* ------------------------ DEVICE SPECIFIC ------------------------ */
    
    if(local_pcdev_data->id == 0)   /* DIO1 Direction */
    {
        if( copy_from_user(value, buffer, 1) ) goto Copy_Error;
        
        if(value[0] == '0')  /* INPUT */
        {
            pr_info("[%s] - DIO1 Direction set to INPUT\n", __func__);
            pcdrv_data.pcdev_data[0].perms = RDONLY;
            // TODO: Add the GPIO direction setting
            goto Write_Value;
        }
        else if(value[0] == '1') /* OUTPUT */
        {
            pr_info("[%s] - DIO1 Direction set to OUTPUT\n", __func__);
            pcdrv_data.pcdev_data[0].perms = WRONLY;
            goto Write_Value;
        }
        else
        {
            pr_alert("[%s] - Invalid direction\n", __func__);
            return -EINVAL;
        }
    }
    else if(local_pcdev_data->id == 1)  /* DIO1 Value */
    {
        if( copy_from_user(value, buffer, 1) ) goto Copy_Error;

        if(value[0] != '0' && value[0] != '1')
        {
            pr_alert("[%s] - Invalid value\n", __func__);
            return -EINVAL;
        }
        else
        {
            pr_info("[%s] - DIO1 Value set to %c\n", __func__, *buffer);
            goto Write_Value;
        }
    }
    /* ----------------------------------------------------------------- */

    /* Write the value to the device buffer */
    Write_Value:
        if(copy_from_user(local_pcdev_data->buffer+(*f_pos), buffer, size))
        {
            pr_err("Failed to copy data from user\n");
            return -EFAULT;
        }

    *f_pos += size;                      // Update the offset
    local_pcdev_data->g_offset = *f_pos; // Update the global offset

    pr_info("[%s] - Number of bytes successfully written = %ld\n", __func__, size);

    return size; // Return the number of bytes written to the file

    Copy_Error:
        pr_err("Failed to copy data from user\n");
        return -EFAULT;
}

/**
 * pcd_release() - This function shall be called when the file is closed
 * @brief: This function shall be called when the file is closed which if successful, will close the file
 * @param inode: pointer to the inode structure
 * @param filp: pointer to the file structure
 * @return 0 on success
 */
int pcd_release(struct inode *inode, struct file *flip)
{
	pr_info("[%s] - Device file closed\n", __func__);

	return 0;
}

/***********************************************************************************************/

/************************************* MODULE *************************************/

// This is the init function that is called when the module is loaded
static int __init pcdriver_init(void)
{
    /* 1 - Dynamically allocate a device number
     * 2 - Create a class for the driver under /sys/class
     * 3 - Initialize and add the cdev structure with the file operations for all devices
     * 4 - Add the device and populate the sysfs with device info.
     */

    int r_err = 0;
    int i;

    /* 1 - device number - base number - number of devices - name */
    r_err = alloc_chrdev_region(&pcdrv_data.dev_num, 0, NO_OF_DEVICES, "enigma_pcdevs");
    if(r_err < 0)
    {
        pr_err("Failed allocating device number\n");
        goto failure_out;
    }

    /* 2 - Create a class for the driver under /sys/class */
    pcdrv_data.class_pcd = class_create("enigma_pcd_class");
    if(IS_ERR(pcdrv_data.class_pcd))
    {
        pr_err("Failed creating class\n");
        r_err = PTR_ERR(pcdrv_data.class_pcd);
        goto unreg_chrdev;
    }

    /* 3 - Initialize and add the cdev structure with the file operations for all devices */
    for(i = 0; i < NO_OF_DEVICES; i++)
    {
        pr_info("Device [%s] number %d:%d\n", pcdrv_data.pcdev_data[i].name, MAJOR(pcdrv_data.dev_num + i), MINOR(pcdrv_data.dev_num + i));

        /* Initialize the cdev structure with the file operations */
        cdev_init(&pcdrv_data.pcdev_data[i].cDev, &fops);

        pcdrv_data.pcdev_data[i].cDev.owner = THIS_MODULE;
        /* Add the device to the system
         * cdev structure - device number - number of devices */
        r_err = cdev_add(&pcdrv_data.pcdev_data[i].cDev, pcdrv_data.dev_num + i, 1);
        if(r_err < 0)
        {
            pr_err("Failed adding Device[%d] cdev\n", pcdrv_data.dev_num + i);
            goto del_cdev;
        }

        /* 4 - Add the device and populate the sysfs with device info.
         * class - parent - device number - driver data - device name */
        pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, pcdrv_data.dev_num + i, NULL, pcdrv_data.pcdev_data[i].name);
        if(IS_ERR(pcdrv_data.device_pcd))
        {
            pr_err("Failed creating device[%d]\n", pcdrv_data.dev_num + i);
            r_err = PTR_ERR(pcdrv_data.device_pcd);
            goto del_cdev;
        }
    }

    pr_info("Module insertion was successful\n");
    return r_err;

    /* ---------- Error handling ---------- */
    del_cdev:
        for( ; i >= 0 ; i--){
            device_destroy(pcdrv_data.class_pcd,pcdrv_data.dev_num+i);
            cdev_del(&pcdrv_data.pcdev_data[i].cDev);
	    }
        class_destroy(pcdrv_data.class_pcd);
    unreg_chrdev:
        unregister_chrdev_region(pcdrv_data.dev_num, NO_OF_DEVICES);
    failure_out:
        pr_info("Module insertion failed\n");
	    return r_err;
}

// This is the exit function that is called when the module is removed
static void __exit pcdriver_exit(void)
{
    /* Start with: 
     * removing device
     * removing class
     * removing cdev
     * unregistering device number
     */
    int i;

    for(i = 0; i < NO_OF_DEVICES; i++)
    {
        device_destroy(pcdrv_data.class_pcd, pcdrv_data.dev_num + i);
        cdev_del(&pcdrv_data.pcdev_data[i].cDev);
    }

    class_destroy(pcdrv_data.class_pcd);

    unregister_chrdev_region(pcdrv_data.dev_num, NO_OF_DEVICES);

    pr_info("Module removal was successful\n");
}

// These are the module init and exit functions
module_init(pcdriver_init);
module_exit(pcdriver_exit);
/***********************************************************************************************/

/************************************* MODULE INFORMATION *************************************/
// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enigma");
MODULE_DESCRIPTION("My Pseudo Character Driver");