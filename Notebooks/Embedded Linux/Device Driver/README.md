# Contents

- [Contents](#contents)
  - [Device Driver](#device-driver)
  - [Linux Kernel Module _(LKM)_](#linux-kernel-module-lkm)
    - [Kernel Headers](#kernel-headers)
    - [Simple Example](#simple-example)
    - [Makefile Content](#makefile-content)
      - [Variables](#variables)
      - [Targets](#targets)
    - [Module Construction](#module-construction)
    - [`insmod` \& `rmmod`](#insmod--rmmod)
      - [`insmod` Parameters](#insmod-parameters)
      - [Permissions MACROS](#permissions-macros)
  - [Driver File](#driver-file)
    - [File Operations](#file-operations)
    - [Proc FS](#proc-fs)
    - [Character Device](#character-device)
      - [Major \& Minor Numbers](#major--minor-numbers)
        - [Major Number](#major-number)
        - [Minor Number](#minor-number)
      - [Example of Registering \& Unregistering a Device](#example-of-registering--unregistering-a-device)
      - [Adding a Device File _(Major \& Minor)_](#adding-a-device-file-major--minor)
    - [Read/Write Functions](#readwrite-functions)
    - [Permissions](#permissions)
  - [Error Handling](#error-handling)

## Device Driver

**The main interface between the kernel and user space is the set of `system calls`** ... About `400 system calls` that provide the main kernel services. Linux makes system and kernel information available in user space through
pseudo filesystems, sometimes also called virtual filesystems.

> :grey_exclamation: These pseudo filesystems are not stored on disk, but are in RAM.

dtb _(device tree binary)_ --> is introduced in the newer versions of the kernel (after v2) ... before, the kernel was dependent on the hardware .. meaning that kernel access hardware on its source code, meaning that the kernel was hardware dependent

**Static Drivers**, cannot be unloaded form RAM

dts _(device tree source)_ describes where the hardware resource is located like saying its address _(like a register file)_ ... whereas the driver, is the code implementation itself.

If you try to access a wrong address, this could

> :white_check_mark: **Elixir Bootlin** --> It's a source code cross-referencer inspired by `LXR 3`. It's written in Python and its main purpose is to index every release of a `C/C++` project **(like the Linux kernel)** while keeping a minimal footprint.

## Linux Kernel Module _(LKM)_

Every kernel module just **registers** itself in order to serve future requests and its initialization function terminates immediately.

- The purpose of the module’s **initialization** function is to prepare for later invocation of module’s functions
- The module’s **exit** function gets invoked just before the module is `unloaded`.

> :grey_exclamation: **Incidentally, the ability to unload a module is one of the features of modularization that you’ll most appreciate**
>
> :exclamation: **a kernel fault kills the current process at least, if not the whole system**

---

### Kernel Headers

Kernel headers **`<linux/module.h>`** are not accessible in `user-space` ... they're in the **Kernel Header Section**

**Code Section** contains the prototype functions

When implementing your own device driver, u will be using the kernel header and functions implemented by the kernel.

The files in RAM which are to be mounted are called **_Pseudofiles_**:

- `proc`
- `sys`
- `dev`

> **`uname`** --> _print system information_ --- **`uname -r`** --> _print --kernel-release_

---

### Simple Example

```c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
  printk(KERN_ALERT "Hello, world\n");
  return 0;
}
static void hello_exit(void)
{
  printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
```

- **`(MODULE_LICENSE)`** is used to tell the kernel that this module bears a free license; **without such a declaration, the kernel complains when the module is loaded**
- The string **`KERN_ALERT`** is the priority of the message. We’ve specified a high priority in this module, because a message with the default priority might not show up anywhere useful,

> :exclamation: `void` is a **MUST** for the `init` & `exit` functions

---

### Makefile Content

#### Variables

- **`obj.x`** --> object file _[X]_ can refer to various things:
  - **`n`: no compile _(not needed in general)_**
  - **`m`: compile as a dynamic driver module**
  - **`y`: compile static module _(zimage)_**
- **`BDIR`** --> **_(could be any name)_** points to the compiled kernel modules _(build)_

#### Targets

namings can be changed.

- **`build:`** --> _build the module_
- **`obj.m`** --> _compile the module_
- **`obj.m`** -->

> make -c --> calls the makefile of a different build

---

### Module Construction

Init contains initialization functions, and exit contains the exit functions.

> :exclamation: **Don't call init of another driver, inside your init !!** --> **that's why we make it `static`!**

- Make **`--init`** to load your init function into **init RAM section _(.init)_**, to get removed from RAM after execution
  ... **and the same can be applied for variables _(.init.data)_**

- Make **`--exit`** to tell the kernel to remove this function if it's static module or keep it if it's dynamic module ... **hence, making the driver generic**

- `module_init()` --> to initialize the module passing your init function
- `module_exit()` --> to exit the module passing your exit function

> :exclamation: You have to add _`'\n'`_ at the end if printk() ... as of pressing enter to show!

---

### `insmod` & `rmmod`

| Command            | Description                                                 |
| ------------------ | ----------------------------------------------------------- |
| `insmod`           | to insert the module into the kernel                        |
| `rmmod`            | to remove the module from the kernel                        |
| `lsmod`            | to list the modules in the kernel                           |
| `modinfo`          | to show the information of the module                       |
| `modprobe`         | to insert the module into the kernel with its dependencies  |
| `sudo mknod`       | to create a device file                                     |
| `dmesg`            | to show the kernel messages                                 |
| `ls /dev`          | to list the devices                                         |
| `ls /sys`          | to list the system devices _(Everything related to kernel)_ |
| `ls /proc`         | to list the process devices                                 |
| `ls /sys/class`    | to list the class devices                                   |
| `strace -c`        | to show the system calls                                    |
| `strace "command"` | to show the system calls of a specific command              |

---

#### `insmod` Parameters

:grey_exclamation: You can pass parameters to the module using `insmod` command ... **like `insmod mymodule.ko myparam=5`** ... but we have to add **`#include <linux/moduleparam.h>`** library.

```c
#include <linux/moduleparam.h>
int myparam = 0;    // default value
// myparam --> the name of the parameter
// int --> the type of the parameter
// 0 --> the permission of the parameter
module_param(myparam, int, 0);  // to pass the parameter
MODULE_PARM_DESC(myparam, "This is my parameter");  // to describe the parameter
/* PARAM_DESC will be seen in the modinfo */
```

#### Permissions MACROS

There are MACROs for the permissions for simplicity **which are defined inside the `linux/stat.h` library**:

```c
S_IRWXU   // --> read, write, execute permissions for the user (00700)
S_IRUSR   // --> read permission for the user     (00400)
S_IWUSR   // --> write permission for the user    (00200)
S_IXUSR   // --> execute permission for the user  (00100)
/*  -------------------------------------------------------------  */
S_IRWXG   // --> read, write, execute permissions for the group (00070)
S_IRGRP   // --> read permission for the group     (00040)
S_IWGRP   // --> write permission for the group    (00020)
S_IXGRP   // --> execute permission for the group  (00010)
/*  -------------------------------------------------------------  */
S_IRWXO   // --> read, write, execute permissions for others (00007)
S_IROTH   // --> read permission for others     (00004)
S_IWOTH   // --> write permission for others    (00002)
S_IXOTH   // --> execute permission for others  (00001)
```

- **`S`** short for `stat`
- **`I`** short for `inode`

---

## Driver File

### File Operations

Each driver has a set of file operations that it supports. These operations are defined in a structure called `file_operations` corresponding to each directory the file will be created inside _(like `/dev` directory or `/sys` directory)_

The structure contains pointers to the functions that will be called when the file is opened, closed, read, or written ...etc.

```c
// THIS_MODULE --> is a macro that points to the module that is currently running
struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = device_open,
  .release = device_release,
  .read = device_read,
  .write = device_write
};
```

> :grey_exclamation: You will have to implement your own functions for the file operations ... like `device_open`, `device_release`, `device_read`, `device_write` ... etc.

---

### Proc FS

`user` can be considered as the shared buffer between the kernel and the user. ... **but it isn't preferred to use it as a shared buffer, because it's not secure.** The more perffered way:

---

### Character Device

Char devices are accessed through names in the filesystem. Those names are called special files or device files _conventionally located in the /dev directory_

my devices & drivers are allocated in `/dev` or `/sys` directory

---

#### Major & Minor Numbers

The following example shows a snippet of the major and minor numbers for some devices:

| Major Number | Minor Number | Device Name |
| :----------: | :----------: | :---------: |
|      1       |      3       |  /dev/null  |
|      1       |      5       |  /dev/zero  |
|      4       |      1       |  /dev/tty1  |
|      4       |      64      | /dev/ttys0  |
|      4       |      65      | /dev/ttyS1  |

To be able to create a device file. You **MUST** include and use #include <linux/fs.h> library _(File Structure)_ ... and you have to use the following functions

---

##### Major Number

The major number identifies the **`driver`** **associated with the device** ... `/dev/null` and `/dev/zero` are both managed by **driver 1**

> :grey_exclamation: `/proc/devices` --> contains all major numbers for the connected devices

##### Minor Number

The minor number is used by the kernel to determine exactly which device is being referred to.

---

#### Example of Registering & Unregistering a Device

```c
#define DEVICE_NAME "chardev"
int retValue;   // to store the return value of the functions
// ---------------
/* Init Module */
// ---------------
/* ---------- Static allocation of the major number ---------- */
// major --> the major number of the device
// DEVICE_NAME --> the name of the device
// fops --> the file operations structure (read, write, open, close, ...etc.)
retValue = register_chrdev(major, DEVICE_NAME, &fops); // returns 0 if successful

/* ---------- Dynamic allocation of the major number ---------- */
// device_number --> structure for the device number
// 0 --> the first minor number
// 1 --> the number of devices
// DEVICE_NAME --> the name of the device
alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME); // returns 0 if successful


// ---------------
/* Exit */
// ---------------
unregister_chrdev(major, DEVICE_NAME); // Remove statically device

unregister_chrdev_region(device_number, 1); // Remove dynamically device
```

> :exclamation: When trying to reserve a major number **that is already reserved by another driver**, the `register_chrdev()` function will return unsuccessful.

---

#### Adding a Device File _(Major & Minor)_

The following example adds a device file to the `sys` directory & `dev` directory, **steps:**

- Create the **`dev` device number struct** & **`cdev` struct** and **file operations struct**
- Dynamically allocate the major number
- Define the driver type _(character, block, ...etc.)_ **(We're using character device `cdev`)**
- Add the minor devices.
- Generate the device file class.

```c

```

> :exclamation: `class_create()` returns a **pointer** to the class structure ... and `device_create()` returns a **pointer** to the device structure.
>
> :exclamation: You must **_remove/unregister/destroy_** the device file & class file in **an opposite direction from the creation**.

---

These reduce /var size ... as logging (infinitely) can **significantly** fill up the disk space
`rm /var/log/syslog.log` --> to remove the log file
`rm /var/log/kern.log` --> to remove the log file

> **`dev` & `sys` & `proc`** are all in RAM ... so, they're not stored in the disk.

### Read/Write Functions

We've to construct the read/write functions which will be passed to the file operations structure.

- `copy_to_user()` --> to copy from kernel to user
- `copy_from_user()` --> to copy from user to kernel

```c
static int device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
  // Read the data from the device
  // buffer --> the buffer to store the data
  // length --> the length of the data
  // offset --> the offset of the data
  return 0;
}

file_operations fops = {
  .owner = THIS_MODULE,
  .open = device_open,
  .release = device_release,
  .read = device_read,
  .write = device_write
};
```

```c

```

---

### Permissions

The `struct file` is a structure that represents an open file in the kernel. It is used to store information about the file, such as the file descriptor, the file operations, and the file position ...etc.

You can change and check on the file permissions using the `struct file` structure.

```c
// f_mode contains read/write permissions
if( file->f_mode & FMODE_READ ) {;
    printk(KERN_INFO "File is readable\n");
}
```

---

if the `open()` function **returns 0** --> it means that the file is opened successfully and goes to call the corresponding read or write ... **but if it returns `error`**, then it means that **the file is not opened successfully and read/write won't be called.**

---

## Error Handling

It's a good practice to handle errors in the kernel module ... as the kernel is a critcal part of the system ... and _bugs/errors and segmenation_ faults can lead to **system crashes and critical problems**.

In kernel modules, **`goto` `labels`** is usually used to handle errors ... as it's the **most efficient** way to handle errors in the kernel. However, it's not recommended to use `goto` in the user-space code.

```c
static int __init chardev_init(void)
{
  // Register the device
  retVal = alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME);
  if (retVal < 0) {
    printk(KERN_ALERT "Failed to allocate a major number\n");
    goto CHAR_ERROR;
  }

  // Define & Add the device
  cdev_init(&cdev, &fops);
  cdev.owner = THIS_MODULE;
  retVal = cdev_add(&cdev, device_number, 1);
  if (retVal < 0) {
    printk(KERN_ALERT "Failed to add the device\n");
    goto CHAR_DEV_ERROR;
  }

/* And so on .... */

  return 0;

// Label
CHAR_DEV_ERROR:
  cdev_del(&cdev);
CHAR_ERROR:
  unregister_chrdev_region(device_number, 1);
  return -1;
}

```

---

## Adding Kernel Module Through `Yocto`

To add a kernel module to the `Yocto` build, you have to make a recipe for it ... and add it to the `meta-layer` of the `Yocto` build. The recipe should contain the following:

> :grey_exclamation: You can find a reference kernel module recipe example in the `meta-skeleton` layer under recipes-kernel directory.

- The source code of the module
- The `Makefile` of the module
- The `bb` file of the module
  - The `bb` file should contain the following:
    - License of the module
    - The `SRC_URI` of the module
    - The `S` variable which points to the source code directory
    - inherit the `module` class
    - `RPROVIDES_${PN} = "${PN}-kernel-module"` ... to provide the kernel module

> :exclamation: Make sure to add the `linux` recipe example to your `meta-layer` to be able to build the kernel module.

You can check the `007-GPIO_Driver` for more information about the source file _(GPIO)_ and adding it using yocto to Raspberry Pi.
