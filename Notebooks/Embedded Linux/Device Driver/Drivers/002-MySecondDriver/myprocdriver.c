#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>

/* Don't add any spaces in the folder or file name !!! */

/************************************* FUNCTION DEFINITIONS *************************************/
// This function is called when the /proc/myprocenigma file is read (read operation)
ssize_t myRead(struct file *myFileName, char __user *buffer, size_t size, loff_t *offset)
{
    printk("My Proc Reading ... Enigma!\n");
    copy_to_user(buffer, "Hello from myprocenigma!\n", 26); // Copy the string to the user space (to terminal)
    return 0; // Return the number of bytes read from the file (usually 0)
}

// This function is called when the /proc/myprocenigma file is written (write operation)
ssize_t myWrite(struct file *myFileName, const char __user *buffer, size_t size, loff_t *offset)
{
    char myBuffer[24];      // Buffer to store the string from the user space
    printk("My Proc Writing ... Enigma!\n");
    copy_from_user(myBuffer, buffer, size); // Copy the string from the user space (from terminal)
    return size; // Return the number of bytes written to the file
}
/***********************************************************************************************/

/************************************* GLOBAL VARIABLES *************************************/
static struct proc_dir_entry *myProcDirEntry = NULL;

static struct proc_ops myProc_operations = {
    .proc_read = myRead,
    .proc_write = myWrite,
};
/***********************************************************************************************/

/************************************* MODULE *************************************/
// This is the init function that is called when the module is loaded
static int __init mydriver_init(void)
{
    // Create a new entry in /proc
    // 0660 is the permission (rw- rw- ---)
    // NULL is the parent directory
    // fileoperation is the file operation structure
    myProcDirEntry = proc_create("myprocenigma", 0660, NULL, &myProc_operations);

    printk("Hello from my proc driver! ~ Enigma\n");
    return 0;
}

// This is the exit function that is called when the module is removed
static void __exit mydriver_exit(void)
{
    proc_remove(myProcDirEntry);
    printk("Removing myprocenigma ... bye bye ~ Enigma!\n");
}

// These are the module init and exit functions
module_init(mydriver_init);
module_exit(mydriver_exit);
/***********************************************************************************************/

/************************************* MODULE INFORMATION *************************************/
// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enigma");
MODULE_DESCRIPTION("My second driver");