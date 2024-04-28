/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  AXZ.h
 *
 *  Description: This file contains the header files and the function prototypes for the AXZ.c file.
 *
 *********************************************************************************************************************/

#ifndef _AXZ_H_
#define _AXZ_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/device.h>

/**********************************************************************************************************************
 * DEFINITIONS
 *********************************************************************************************************************/
#define DEVICE_MAX_SIZE 128
#define NO_OF_DRIVERS   1

/**********************************************************************************************************************
 * STRUCTURE TYPE DEFINITIONS
 *********************************************************************************************************************/

#define AXZ_ID     0    /* Device ID */

/* Permission codes */
#define RDONLY 0x01
#define WRONLY 0X10
#define RDWR   0x11

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

/**********************************************************************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************************************************************/



#endif /* _AXZ_H_ */