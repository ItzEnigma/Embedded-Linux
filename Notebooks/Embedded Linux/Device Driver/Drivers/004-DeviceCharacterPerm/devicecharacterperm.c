#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


/************************************* DEFINITIONS *************************************/
#define DEVICE_MAX_SIZE 1024 

/************************************* GLOBAL VARIABLES *************************************/

// This is the device number //(will get dynamically allocated by alloc_chrdev_region)
static dev_t device_number;         // holds device number (major + minor)
/* Classes allow user space
 * to work with devices based on what they do, rather than how they are
 * connected or how they work
 */
static struct class *myclass;       
static struct device *mydevice;     
static struct cdev mycdev;          // holds the character device structure
/* The buffer used to store character for this module (device memory) */
static char device_buffer[DEVICE_MAX_SIZE] = "Hello World!\n"; 
/***********************************************************************************************/


/************************************* FUNCTION DEFINITIONS *************************************/
// This function is called when the file is read (read operation)
ssize_t myRead(struct file *myFileName, char __user *buffer, size_t size, loff_t *offset)
{
    int r_err = 0;

    // Check if offset is beyond the end of the device buffer
    if (*offset >= DEVICE_MAX_SIZE) return 0;

    printk(KERN_INFO "%s: [%s] the maximum count that can be written to screen: %ld\n", __FILE__, __func__, size);
    
    
    if (size + *offset > DEVICE_MAX_SIZE) // Check if the size is bigger than the device buffer
    {
        size = DEVICE_MAX_SIZE - *offset;
    }
    r_err = copy_to_user(buffer, &device_buffer[*offset], size);
    if(r_err)
    {
        printk(KERN_ALERT "Error copying to user\n");
        return -EFAULT;
    }
    *offset = size; // Update the offset
    return size; // Return the number of bytes read from the file (usually 0)
}

// This function is called when the file is written (write operation)
ssize_t myWrite(struct file *myFileName, const char __user *buffer, size_t size, loff_t *offset)
{
    int r_err = 0;

    static int g_offset = 0;

    // Check if offset is beyond the end of the device buffer
    if (*offset >= DEVICE_MAX_SIZE) return -ENOSPC; // No space left on device

    printk(KERN_INFO "%s: [%s] the count to write: %ld\n", __FILE__, __func__, size);

    if (size + *offset > DEVICE_MAX_SIZE) // Check if the size is bigger than the device buffer
    {
        size = DEVICE_MAX_SIZE - *offset;
    }
    // Check if the file is opened with O_APPEND flag
    if (myFileName->f_flags & O_APPEND) {
        *offset = g_offset; // Set offset to the end of the device buffer
    }
    else    // clear the device buffer (important for rewriting)
    {
        memset(device_buffer, 0, DEVICE_MAX_SIZE); // Clear the device buffer
    }

    r_err = copy_from_user(&device_buffer[*offset], buffer, size);
    if(r_err)
    {
        printk(KERN_ALERT "Error copying from user\n");
        return -EFAULT;
    }
    *offset += size; // Update the offset
    g_offset = *offset; // Update the global offset

    return size; // Return the number of bytes written to the file
}

static char *myDevNode(const struct device *dev, umode_t *mode)
{
    if(!mode)
    {
        return NULL;
    }
    *mode = 0400;
    return NULL;
}

static struct file_operations fops = {
    .read = myRead,
    .write = myWrite,
};

/***********************************************************************************************/

/************************************* MODULE *************************************/
// This is the init function that is called when the module is loaded
static int __init mydriver_init(void)
{
    int r_err = 0;
    // Allocate a device number (Takes a pointer to the cdev and allocates memory for it)
    // device_number is a capsulated number that contains the major and minor numbers
    // 0 --> base number (to start from)
    // 1 --> number of devices
    r_err = alloc_chrdev_region(&device_number, 0, 1, "mydeviceenigma_perm");

    if(r_err < 0)
    {
        printk(KERN_ALERT "Error allocating device number\n");
        return 1;
    }
    // Initialize the cdev structure with the file operations
    cdev_init(&mycdev, &fops);
    mycdev.owner = THIS_MODULE;
    // Add the device to the system
    // 1 --> number of devices
    cdev_add(&mycdev, device_number, 1);

    // Create a class for the device (this will create a directory in /sys/class)
    myclass = class_create("mydeviceenigma_perm");

    // Change device permissions
    myclass->devnode = myDevNode;

    // Create the device in /dev
    mydevice = device_create(myclass, NULL, device_number, NULL, "mydeviceenigma_perm");

    printk(KERN_INFO "Hello from my character device driver! ~ Enigma\n");
    return 0;
}

// This is the exit function that is called when the module is removed
static void __exit mydriver_exit(void)
{
    /* Start with: 
     * removing device
     * removing class
     * removing cdev
     * unregistering device number
     */

    // Remove the device from /dev
    device_destroy(myclass, device_number); // Remove the device from /dev

    // Remove the class from /sys/class
    class_destroy(myclass); // Remove

    // Delete the device from the system
    cdev_del(&mycdev);

    // Unregister the device number (remove)
    unregister_chrdev_region(device_number, 1);
    printk(KERN_INFO "Removing mydeviceenigma_perm ... bye bye ~ Enigma!\n");
}

// These are the module init and exit functions
module_init(mydriver_init);
module_exit(mydriver_exit);
/***********************************************************************************************/

/************************************* MODULE INFORMATION *************************************/
// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enigma");
MODULE_DESCRIPTION("My third driver");