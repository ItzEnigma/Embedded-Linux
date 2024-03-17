# U-Boot _(Bootloader)_
U-Boot is the most popular boot loader in linux based embedded devices. It is released as open source under the GNU GPLv2 license. It supports a wide range of microprocessors like MIPS, ARM, PPC, Blackfin, AVR32 and x86.

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
> ### :grey_exclamation: We will be emulating target: vexpress_ca9x4_defconfig
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
### :white_check_mark: Once the build is complete you will notice u-boot & u-boot.bin files are generated inside u-boot
``` bash
# Try to check the type of the files and understand what it's showing!
file u-boot
file u-boot.bin
```
---
## SD Card

---
## QEMU

---
## TFTP

---
## U-Boot Scripting
