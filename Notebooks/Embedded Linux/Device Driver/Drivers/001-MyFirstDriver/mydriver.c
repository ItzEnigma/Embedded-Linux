#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>

/* Don't add any spaces in the folder or file name !!! */

// This is the init function that is called when the module is loaded
static int __init mydriver_init(void)
{

    // This is the file operation structure that is used to read and write to the /proc file
    static const proc_ops fileoperation = {
        // Contains function pointers to the functions that are called when the /proc file is read or written
        .proc_read = readReadFunction,
        .proc_write = writeReadFunction,
        //.proc_open = NULL,
        //.proc_release = NULL,
    };

    // Create a new entry in /proc
    // 0660 is the permission (rw- rw- ---)
    // NULL is the parent directory
    // fileoperation is the file operation structure
    proc_create("mydriver", 0660, NULL, NULL);

    printk("Hello, world - this is my first driver!\n");
    return 0;
}

// This is the exit function that is called when the module is removed
static void __exit mydriver_exit(void)
{
    printk("DeInit ... bye bye Enigma!\n");
}

// These are the module init and exit functions
module_init(mydriver_init);
module_exit(mydriver_exit);

// This is the module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enigma");
MODULE_DESCRIPTION("My first driver");