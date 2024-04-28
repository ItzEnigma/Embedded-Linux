/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  platform_cdriver.C
 *
 *  Description: This file contains the implementation of the functions for the platform_cdriver.h file and the main kernel
 *               module functions (init and exit), and file operations functions definitions.
 *
 *********************************************************************************************************************/
#include "platform_cdriver.h"

/************************************* GLOBAL VARIABLES *************************************/

/* ---------- Table of device IDs ---------- */
struct platform_device_id pcdrv_id_table[] = {
    [AXZ] = {
        .name = "enigma-AXZ",
        .driver_data = AXZ
    },
    [BXZ] = {
        .name = "enigma-BXZ",
        .driver_data = BXZ
    },
    [CXZ] = {
        .name = "enigma-CXZ",
        .driver_data = CXZ
    },
    [DXZ] = {
        .name = "enigma-DXZ",
        .driver_data = DXZ
    },
    {}
};

/* ---------- Platform Driver Data ---------- */
static struct platform_driver pcdrv_platform_data = {
    .probe = pcdrv_probe,
    .remove = pcdrv_remove,
    .id_table= pcdrv_id_table,
    .driver = {
        .name = "enigma_platform_driver"
    }
};


/* ---------- Driver/Devices ---------- */
static pcDriver_private_data_t pcdrv_data = {
    .no_of_devices = 0,
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
 * pcdrv_probe() - This function shall be called when the device is probed
 * @brief: This function shall be called when the device is probed which if successful, will probe the device
 * @param plat_dev: pointer to the platform device structure
 * @return 0 on success
 */
int pcdrv_probe(struct platform_device *plat_dev)
{
    int r_err;

    pcDevice_private_data_t *pcdev_data;
    
    /* Get the platform data */
    pcdev_data = dev_get_platdata(&plat_dev->dev);
    if(!pcdev_data)
    {
        pr_err("No platform data available\n");
        r_err = -EINVAL;
        goto probe_failOut;
    }

    /* 3 - Initialize and add the cdev structure with the file operations for all devices */
    cdev_init(&pcdev_data->cDev, &fops);
    pcdev_data->cDev.owner = THIS_MODULE;
    r_err = cdev_add(&pcdev_data->cDev, pcdrv_data.dev_num + pcdev_data->id, 1);
    if(r_err < 0)
    {
        pr_err("Failed adding Device[%d] cdev\n", pcdev_data->id);
        goto probe_failOut;
    }

    /* 4 - Add the device and populate the sysfs with device info.
     * class - parent - device number - driver data - device name */
    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, pcdrv_data.dev_num + pcdev_data->id, NULL, "enigma-%s%d", pcdev_data->name, pcdev_data->id);
    if(IS_ERR(pcdrv_data.device_pcd))
    {
        pr_err("Failed creating device[%d]\n", pcdev_data->id);
        r_err = PTR_ERR(pcdrv_data.device_pcd);
        goto probe_failcDev;
    }

    /* Save the device data in the driver's private data */
    pcdrv_data.pcdev_data[pcdrv_data.no_of_devices] = *pcdev_data;
    pcdrv_data.no_of_devices++;
    pr_info("Device[%d] detected\n", pcdev_data->id);
    return 0;


    /* ---------- Error handling ---------- */
    probe_failcDev:
        cdev_del(&pcdev_data->cDev);
    probe_failOut:
        pr_info("Device detection failed\n");
        return r_err;
}

/**
 * pcdrv_remove() - This function shall be called when the device is removed
 * @brief: This function shall be called when the device is removed which if successful, will remove the device
 * @param plat_dev: pointer to the platform device structure
 * @return 0 on success
 */
int pcdrv_remove(struct platform_device *plat_dev)
{
    pcDevice_private_data_t *pcdev_data;

    /* Get the platform data */
    pcdev_data = dev_get_platdata(&plat_dev->dev);

    /* Remove the device */
    device_destroy(pcdrv_data.class_pcd, pcdrv_data.dev_num + pcdev_data->id);

    /* Remove the cdev */
    cdev_del(&pcdev_data->cDev);

    pr_info("Device[%d] removed\n", pcdev_data->id);
    return 0;
}

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
    pr_info("[%s] - Device ID = %d\n", __func__, local_pcdev_data->id);
    goto Write_Value;
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
     * 3 - Register the platform driver with the number of devices
     * 4 - Initialize and add the cdev structure with the file operations for all devices
     */

    int r_err = 0;

    /* 1 - device number - base number - number of devices - name */
    r_err = alloc_chrdev_region(&pcdrv_data.dev_num, 0, NO_OF_DEVICES, "enigma_platform_pcdevs");
    if(r_err < 0)
    {
        pr_err("Failed allocating device number\n");
        goto failure_out;
    }
    
    /* 2 - Create a class for the driver under /sys/class */
    pcdrv_data.class_pcd = class_create("enigma_platformClass");
    if(IS_ERR(pcdrv_data.class_pcd))
    {
        pr_err("Failed creating class\n");
        r_err = PTR_ERR(pcdrv_data.class_pcd);
        goto unreg_chrdev;
    }
    platform_driver_register(&pcdrv_platform_data);

    pr_info("Module insertion was successful\n");
    return r_err;

    /* ---------- Error handling ---------- */
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
     * unregistering the platform driver
     * removing class
     * removing cdev
     * unregistering device number
     */

    platform_driver_unregister(&pcdrv_platform_data);

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
MODULE_DESCRIPTION("Platform Character Device Driver");