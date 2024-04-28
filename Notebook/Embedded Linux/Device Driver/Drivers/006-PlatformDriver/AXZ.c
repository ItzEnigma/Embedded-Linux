/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  AXZ.C
 *
 *  Description: This file contains the implementation of the functions for the AXZ.h file and the main kernel
 *               module functions (init and exit).
 *
 *********************************************************************************************************************/
#include "AXZ.h"


/************************************* FUNCTION DEFINITIONS *************************************/

/* To fix the bug of the missing pcdev_release function in the kernel dmesg */
static void pcdev_release(struct device* dev)
{
}
/***********************************************************************************************/

/************************************* GLOBAL VARIABLES *************************************/

char device_buffer_pcdev1[DEVICE_MAX_SIZE];

/* ---------- Driver/Devices ---------- */
static pcDevice_private_data_t pcdev_data = {
    .name = "AXZ",
    .id = AXZ_ID,
    .buffer = device_buffer_pcdev1,
    .perms = RDWR,
    .size = DEVICE_MAX_SIZE
};

/* ---------- Platform Driver Data ---------- */
static struct platform_device pcdev_platform_data = {
    .name = "enigma-AXZ",
    .id = AXZ_ID,
    .dev = {
        .platform_data = &pcdev_data,
        .release = pcdev_release
    }
};
/***********************************************************************************************/

/************************************* MODULE *************************************/

// This is the init function that is called when the module is loaded
static int __init pcdriver_init(void)
{
    /* 1 - Dynamically allocate a device number
     * 2 - Create a class for the driver under /sys/class
     * 3 - Register the platform driver with the number of devices
     * 4 - Initialize and add the cdev structure with the file operations for all devices
     * 4 - Add the device and populate the sysfs with device info.
     */

    int r_err = 0;

    platform_device_register(&pcdev_platform_data);

    pr_info("Module insertion was successful\n");
    return r_err;
}

// This is the exit function that is called when the module is removed
static void __exit pcdriver_exit(void)
{
    /* Start with: 
     * unregistering the platform device
     */

    platform_device_unregister(&pcdev_platform_data);

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
MODULE_DESCRIPTION("AXZ Platform Device");