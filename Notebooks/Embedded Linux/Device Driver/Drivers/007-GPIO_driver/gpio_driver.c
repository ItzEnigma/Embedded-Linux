/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  gpio_driver.C
 *
 *  Description: This file contains the implementation of the functions for the gpio_driver.h file and the main kernel
 *               module functions (init and exit), and file operations functions definitions.
 *
 *********************************************************************************************************************/
#include "gpio_driver.h"

/************************************* GLOBAL VARIABLES *************************************/

/* pseudo device's memory */
char buffer_dio1[BUFFER_SIZE];
char buffer_dio2[BUFFER_SIZE];

/* ---------- Driver/Devices ---------- */
static pcDriver_private_data_t pcdrv_data = {
    .no_of_devices = NO_OF_DIOS_USED,
    .pcdev_data = {
        [0] = {/* GPIO0 --> Output */
               .name = "dio0",
               .id = 0,
               .perms = RDWR,
               .buffer = buffer_dio1,
               .size = BUFFER_SIZE},
        [1] = {/* GPIO1 --> Output */
               .name = "dio1",
               .id = 1,
               .perms = RDWR,
               .buffer = buffer_dio2,
               .size = BUFFER_SIZE},
        [2] = {/* GPIO2 --> Output */
               .name = "dio2",
               .id = 2,
               .perms = RDWR,
               .buffer = buffer_dio1,
               .size = BUFFER_SIZE},
        [3] = {/* GPIO3 --> Input */
               .name = "dio3",
               .id = 3,
               .perms = RDONLY,
               .buffer = buffer_dio2,
               .size = BUFFER_SIZE},
        [4] = {/* GPIO4 --> Input */
               .name = "dio4",
               .id = 4,
               .perms = RDONLY,
               .buffer = buffer_dio1,
               .size = BUFFER_SIZE},
        [5] = {/* GPIO5 --> Input */
               .name = "dio5",
               .id = 5,
               .perms = RDONLY,
               .buffer = buffer_dio2,
               .size = BUFFER_SIZE},
        [6] = {/* GPIO6 --> Output */
               .name = "dio6",
               .id = 6,
               .perms = RDWR,
               .buffer = buffer_dio1,
               .size = BUFFER_SIZE},
        [7] = {/* GPIO7 --> Output */
               .name = "dio7",
               .id = 7,
               .perms = RDWR,
               .buffer = buffer_dio2,
               .size = BUFFER_SIZE},
        [8] = {/* GPIO8 --> Output */
               .name = "dio8",
               .id = 8,
               .perms = RDWR,
               .buffer = buffer_dio1,
               .size = BUFFER_SIZE},
        [9] = {/* GPIO9 --> Output */
               .name = "dio9",
               .id = 9,
               .perms = RDWR,
               .buffer = buffer_dio2,
               .size = BUFFER_SIZE}}};
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

    if (dev_perm == RDWR)
    {
        return 0;
    }

    /* Read only */
    if ((dev_perm == RDONLY) && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
    {
        return 0;
    }

    /* Write only */
    if ((dev_perm == WRONLY) && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
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
    if (r_err)
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
    pcDevice_private_data_t *local_pcdev_data = (pcDevice_private_data_t *)filp->private_data;
    size_t dev_size = local_pcdev_data->size;

    // Check if offset is beyond the end of the device buffer
    if (*f_pos >= dev_size)
        return 0;

    pr_info("[%s] - Current file position = %lld\n", __func__, *f_pos);

    if (size + *f_pos > dev_size) // Check if the size is bigger than the device buffer
    {
        size = dev_size - *f_pos;
    }

    /* Get GPIO value */
    local_pcdev_data->buffer[0] = gpio_get_value(GPIO_START_PIN + local_pcdev_data->id) + '0';
    local_pcdev_data->buffer[1] = '\n';

    if (copy_to_user(buffer, local_pcdev_data->buffer + (*f_pos), size))
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
    pcDevice_private_data_t *local_pcdev_data = (pcDevice_private_data_t *)filp->private_data;
    size_t dev_size = local_pcdev_data->size;

    pr_info("[%s] - Current file position = %lld\n", __func__, *f_pos);

    // Check if offset is beyond the end of the device buffer
    if (*f_pos >= dev_size)
        return -ENOSPC; // No space left on device

    if (size + *f_pos > dev_size) // Check if the size is bigger than the device buffer
    {
        size = dev_size - *f_pos;
    }

    // Check if the file is opened with O_APPEND flag
    if (filp->f_flags & O_APPEND)
    {
        *f_pos = local_pcdev_data->g_offset; // Set offset to the end of the device buffer
    }
    else // clear the device buffer (important for rewriting)
    {
        memset(local_pcdev_data->buffer, 0, dev_size); // Clear the device buffer
    }

    /* ------------------------ DEVICE SPECIFIC ------------------------ */
    if (copy_from_user(local_pcdev_data->buffer + (*f_pos), buffer, 1))
        goto Copy_Error;

    switch (local_pcdev_data->buffer[0])
    {
    case '0':
        gpio_set_value(GPIO_START_PIN + local_pcdev_data->id, 0);
        break;
    case '1':
        gpio_set_value(GPIO_START_PIN + local_pcdev_data->id, 1);
        break;
    default:
        pr_err("Invalid data\n");
        return -EINVAL;
    }
    /* ----------------------------------------------------------------- */
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
static int __init GPIO_init(void)
{
    /* 1 - Dynamically allocate a device number
     * 2 - Create a class for the driver under /sys/class
     * 3 - Initialize and add the cdev structure with the file operations for all devices
     * 4 - Add the device and populate the sysfs with device info.
     * 5 - GPIO Request (initialization)
     * 6 - GPIO Direction (input/output)
     */

    int r_err = 0;
    int i;
    int j;

    /* 1 - device number - base number - number of devices - name */
    r_err = alloc_chrdev_region(&pcdrv_data.dev_num, 0, NO_OF_DIOS_USED, "enigma_dios");
    if (r_err < 0)
    {
        pr_err("Failed allocating device number\n");
        goto failure_out;
    }

    /* 2 - Create a class for the driver under /sys/class */
    pcdrv_data.class_pcd = class_create(THIS_MODULE, "enigma_dio_class");
    if (IS_ERR(pcdrv_data.class_pcd))
    {
        pr_err("Failed creating class\n");
        r_err = PTR_ERR(pcdrv_data.class_pcd);
        goto unreg_chrdev;
    }

    /* 3 - Initialize and add the cdev structure with the file operations for all devices */
    for (i = 0; i < NO_OF_DIOS_USED; i++)
    {
        pr_info("Device [%s] number %d:%d\n", pcdrv_data.pcdev_data[i].name, MAJOR(pcdrv_data.dev_num + i), MINOR(pcdrv_data.dev_num + i));

        /* Initialize the cdev structure with the file operations */
        cdev_init(&pcdrv_data.pcdev_data[i].cDev, &fops);

        pcdrv_data.pcdev_data[i].cDev.owner = THIS_MODULE;
        /* Add the device to the system
         * cdev structure - device number - number of devices */
        r_err = cdev_add(&pcdrv_data.pcdev_data[i].cDev, pcdrv_data.dev_num + i, 1);
        if (r_err < 0)
        {
            pr_err("Failed adding Device[%d] cdev\n", pcdrv_data.dev_num + i);
            goto del_cdev;
        }

        /* 4 - Add the device and populate the sysfs with device info.
         * class - parent - device number - driver data - device name */
        pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, pcdrv_data.dev_num + i, NULL, pcdrv_data.pcdev_data[i].name);
        if (IS_ERR(pcdrv_data.device_pcd))
        {
            pr_err("Failed creating device[%d]\n", pcdrv_data.dev_num + i);
            r_err = PTR_ERR(pcdrv_data.device_pcd);
            goto del_cdev;
        }

        /* 5 - GPIO Request (initialization) */
        if (gpio_request(GPIO_START_PIN + i, "enigma_dio") < 0)
        {
            pr_err("Failed requesting GPIO[%d]\n", GPIO_START_PIN + i);
            r_err = -1;
            goto del_cdev;
        }

        /* 6 - GPIO Direction (input/output) */
        if (pcdrv_data.pcdev_data[i].perms == RDONLY)
        {
            if (gpio_direction_input(GPIO_START_PIN + pcdrv_data.pcdev_data[i].id) < 0)
            {
                pr_err("Failed setting GPIO[%d] direction\n", GPIO_START_PIN + i);
                r_err = -1;
                goto gpio_free;
            }
        }
        else
        {
            if (gpio_direction_output(GPIO_START_PIN + pcdrv_data.pcdev_data[i].id, 0) < 0)
            {
                pr_err("Failed setting GPIO[%d] direction\n", GPIO_START_PIN + i);
                r_err = -1;
                goto gpio_free;
            }
        }
    }

    pr_info("Module insertion was successful\n");
    return r_err;

/* ---------- Error handling ---------- */
gpio_free:
    j = i;
    for (; j >= 0; j--)
    {
        gpio_free(GPIO_START_PIN + j);
    }
del_cdev:
    for (; i >= 0; i--)
    {
        device_destroy(pcdrv_data.class_pcd, pcdrv_data.dev_num + i);
        cdev_del(&pcdrv_data.pcdev_data[i].cDev);
    }
    class_destroy(pcdrv_data.class_pcd);
unreg_chrdev:
    unregister_chrdev_region(pcdrv_data.dev_num, NO_OF_DIOS_USED);
failure_out:
    pr_info("Module insertion failed\n");
    return r_err;
}

// This is the exit function that is called when the module is removed
static void __exit GPIO_exit(void)
{
    /* Start with:
     * resetting GPIO values
     * freeing GPIO pins
     * removing device
     * removing class
     * removing cdev
     * unregistering device number
     */
    int i;

    for (i = 0; i < NO_OF_DIOS_USED; i++)
    {
        gpio_set_value(GPIO_START_PIN + i, 0);
        gpio_free(GPIO_START_PIN + i);
        device_destroy(pcdrv_data.class_pcd, pcdrv_data.dev_num + i);
        cdev_del(&pcdrv_data.pcdev_data[i].cDev);
    }

    class_destroy(pcdrv_data.class_pcd);

    unregister_chrdev_region(pcdrv_data.dev_num, NO_OF_DIOS_USED);

    pr_info("Module removal was successful\n");
}

// These are the module init and exit functions
module_init(GPIO_init);
module_exit(GPIO_exit);
/***********************************************************************************************/

/************************************* MODULE INFORMATION *************************************/
// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enigma");
MODULE_DESCRIPTION("My GPIO Character Driver");