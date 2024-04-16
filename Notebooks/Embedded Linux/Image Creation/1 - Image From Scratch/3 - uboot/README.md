## Contents

- [Contents](#contents)
- [U-Boot _(Bootloader)_](#u-boot-bootloader)
- [Setup U-Boot](#setup-u-boot)
  - [Download U-Boot](#download-u-boot)
  - [Configure U-Boot Machine](#configure-u-boot-machine)
  - [Vexpress Cortex A9 (Qemu)](#vexpress-cortex-a9-qemu)
  - [Configure U-Boot](#configure-u-boot)
  - [Build U-Boot](#build-u-boot)
- [SD Card](#sd-card)
    - [Create SD Image File](#create-sd-image-file)
    - [Create `Partition Table` for the SD Image File](#create-partition-table-for-the-sd-image-file)
    - [Attach `Loop Device`](#attach-loop-device)
    - [Format Partition Table](#format-partition-table)
    - [Mount Partitions](#mount-partitions)
    - [Completely erase the SD card](#completely-erase-the-sd-card)
- [QEMU](#qemu)
- [TFTP](#tftp)
- [U-Boot Scripting](#u-boot-scripting)
  - [Create uboot-script](#create-uboot-script)
  - [Convert To Uboot Image](#convert-to-uboot-image)
  - [Copy Image To Boot Partition](#copy-image-to-boot-partition)
  - [Load Script Into Uboot](#load-script-into-uboot)

---
## U-Boot _(Bootloader)_
U-Boot is the most popular boot loader in linux based embedded devices. It is released as open source under the GNU GPLv2 license. It supports a wide range of microprocessors like MIPS, ARM, PPC, Blackfin, AVR32 and x86.

---
## Setup U-Boot
### Download U-Boot
``` bash
git clone git@github.com:u-boot/u-boot.git
cd u-boot/
git checkout v2022.07        # You can go to a working commit just in case
```
### Configure U-Boot Machine
In this section we will **configure** u-boot for several Machine
``` bash
ls configs # In order to find the machine (target) supported by U-Boot
ls configs/ | grep [your machine]
```
> :grey_exclamation: We will be emulating target: vexpress_ca9x4_defconfig
### Vexpress Cortex A9 (Qemu)
In U-boot directory Assign this value
``` bash
# Set the Cross Compiler into the environment
# To be used by the u-boot
export CROSS_COMPILE=<Path To the Compiler>/arm-cortexa9_neon-linux-musleabihf-
export ARCH=arm
# Example: export CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-

# load the default configuration of ARM Vexpress Cortex A9
make vexpress_ca9x4_defconfig
```
### Configure U-Boot
In this part we need to configure some u-boot configuration for the specific board chosen up.
``` bash
make menuconfig
```
**The customer requirements can be like the following:**

- [ ] Support **editenv**.
- [ ] Support **bootd**.
- [ ] Store the environment variable inside file call **uboot.env**.
- [ ] Unset support of **Flash**
- [ ] Support **FAT file system**
- [ ] Configure the FAT interface to **mmc** _(**M**ulti **M**edia **C**ard)_
- [ ] Configure the partition where the fat is store to **0:1**    _(Device 0 : Partition 1 [boot partition])_

### Build U-Boot
After finishing configurations of the bootloader U-Boot ... it's time to build and make the bootloader to be used!
``` bash
make    # While inside the U-Boot Directory!
```
> **:exclamation: If you face any issues during make ... then you need to apt install the missing dependencies like the following:**
> ``` bash
> sudo apt-get install libssl-dev
> ```

> :white_check_mark: Once the build is complete you will notice u-boot & u-boot.bin files are generated inside u-boot
``` bash
# Try to check the type of the files and understand what it's showing!
file u-boot
file u-boot.bin
```
---
## SD Card
In this section we will **create** `SD card` image ... and **write** the `u-boot` to the SD Card

> :exclamation: **ONLY FOR WHO WORK WITH QEMU!**

#### Create SD Image File
In order to **Emulate** SD card to attached to Qemu, the following steps will be followed: **(ONLY FOR NON-PHYSICAL SD)**
``` bash
# Change to the directory where you want to create the SD card image file
cd ..

# Create a file with 1 GB filled with zeros
dd if=/dev/zero of=sd.img bs=1M count=1024
```

#### Create `Partition Table` for the SD Image File
``` bash
# for the VIRTUAL SD card
cfdisk sd.img

# for Physical SD card
cfdisk /dev/mmblck<x>   # Or sdX ... depending on how the SD card is recognized by the system

# cfdisk --> display or manipulate a disk partition table
```
**Then Configure the `Partition Table` and Apply Changes!**
| Partition Size | Partition Format | `Bootable` |
| :------------: | :--------------: | :--------: |
|     200 MB     |      FAT 16      |    Yes     |
|     500 MB     |      Linux       |     No     |
> :grey_exclamation: **Size of the partitions can be adjusted according to the requirements.**

#### Attach `Loop Device`
> :exclamation: **FOR Virtual usage ONLY**

Loop Device is a pseudo-device *(virtual)* that makes a computer file accessible as a block device ... To emulate the **`sd.img`** file as a **sd card** we need to attach it to **loop driver** to be as a block storage.

``` bash
# attach the sd.img to be treated as block storage
sudo losetup -f --show --partscan sd.img

# Running the upper command will show you
# Which loop device the sd.img is connected
```

#### Format Partition Table
As pre-configured from **`cfdisk`** command
- First partition is `FAT`
- Second partition is `Linux` *(ext4)*

We need to apply the format to the partitions ... by **adding the filesystem datastructure to the partitions.**
``` bash
# Get the loop device name
DISK=/dev/loop<x>     # x is the number of the loop device

# Formating the first partition as FAT
sudo mkfs.vfat -F 16 -n boot ${DISK}p1

# format the created partition by ext4
sudo mkfs.ext4 -L rootfs ${DISK}p2

# You can change the label of the partition to anything!
```

#### Mount Partitions
You can mount the partitions to the system to be able to write/see the content of the partitions.
``` bash
# DISK=/dev/loop<x>
# You can mount to any location you want (usually it's /media/<username>/<partition_name>)

sudo mount  ${DISK}p1 ~/testing_mount/boot
sudo mount  ${DISK}p2 ~/testing_mount/rootfs

# Just like mounting any other partition
```
> :white_check_mark: **`lsblk`** command can be used to list all block devices and partitions attached to the system.

> :exclamation: **Loop Device** is used because, the `sd.img` file contains **two partitions**, **and the system can't mount the partitions directly from the file.**

> :grey_exclamation: **`umount`** command is used to unmount the partitions from the system. ... loop device can be detached using **`losetup -d /dev/loop<x>`**

#### Completely erase the SD card
``` bash
### WARNING ####
sudo dd if=/dev/zero of=/dev/mmblk<x> bs=1M
# It can be sd<x> ... /dev/sd<x> ... depending on how the SD card is recognized by the system
```

---
## QEMU

---
## TFTP

---
## U-Boot Scripting
You can create the script by using any text editor *(vim)* to write the commands and configurations desired, and then create a **uboot script image** file using **`mkimage tool`**

### Create uboot-script 
Create the `uboot-script` with the desired configurations and environment variables needed ... with a final extension of *.txt*
``` bash
setenv bootargs "console=ttyAMA0,38400n8 root=/dev/mmcblk0p2 rootfstype=ext4 rw rootwait init=/sbin/init"

setenv imageFat "if mmc dev; then echo Device is working ...; else echo Device is not working!; fi"

setenv load_fromFat "fatload mmc 0:1 $kernel_addr_r zImage; fatload mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb; bootz $kernel_addr_r - $fdt_addr_r"

setenv load_fromTFTP "tftp $kernel_addr_r zImage; tftp $fdt_addr_r vexpress-v2p-ca9.dtb; bootz $kernel_addr_r - $fdt_addr_r"

setenv ipaddr 192.168.0.77
setenv serverip 192.168.0.1

setenv bootcmd "if run imageFat; then run load_fromFat; else run load_fromTFTP; fi"

bootz $kernel_addr_r - $fdt_addr_r"
```

### Convert To Uboot Image
``` bash
# File name is uboot_script.txt 
mkimage -T script -C none -n "uboot_script" -d uboot_script.txt uboot_script.img
```
- **-T** --> File Type
- **-C** --> Compression Type
- **-n** --> Image Name
- **-d** --> Input File then Output file

### Copy Image To Boot Partition
``` bash
# From: uboot_script.img location
# To: SD_Card boot partition
sudo cp .../uboot_script.img .../boot/
```

### Load Script Into Uboot
We have to set an `environment variable` to a specific **DRAM address**, to store the *yet to be loaded* `uboot_script`
``` bash
setenv uboot_scriptaddr 0x60000020

setenv bootcmd fatload mmc 0:1 $uboot_scriptaddr uboot_script.img; source $uboot_scriptaddr

# If the command is working without any syntax errors ... don't forget to saveenv ... to save the uboot_scriptaddr step and bootcmd step
```
> :white_check_mark: **`run bootcmd`**, If the image is loaded successfully .. you should see the image containing the uboot script written, being loaded *(with the commands and configurations written being loaded)* ... and since we added the `bootz` command ... the system would boot successfully!