# Contents

- [Contents](#contents)
  - [BusyBox: The Swiss Army Knife of Embedded Linux](#busybox-the-swiss-army-knife-of-embedded-linux)
  - [Download BusyBox](#download-busybox)
  - [Initialize BusyBox](#initialize-busybox)
  - [Setup `Rootfs`](#setup-rootfs)
  - [NFS Connection](#nfs-connection)
    - [Install NFS](#install-nfs)
    - [Setup `Bootargs` in `U-Boot`](#setup-bootargs-in-u-boot)
  - [`Initramfs`](#initramfs)
    - [Making `initramfs`](#making-initramfs)

---

## BusyBox: The Swiss Army Knife of Embedded Linux

BusyBox combines tiny versions of many common UNIX utilities into a **single small executable**. It provides replacements for most of the utilities you usually find in GNU fileutils, shellutils, etc. The utilities in BusyBox generally have fewer options than their full-featured GNU cousins; however, the options that are included provide the expected functionality and behave very much like their GNU counterparts. **BusyBox provides a fairly complete environment for any small or embedded system**.

BusyBox has been written with **`size-optimization` and `limited resources` in mind**. It is also **`extremely modular` so you can easily include or exclude commands (or features) at compile time**. `This makes it easy to customize your embedded systems`. To create a working system, just add some device nodes in /dev, a few configuration files in /etc, and a Linux kernel.

> BusyBox is licensed under **_GPL v2_** !

---

## Download BusyBox

```bash
git clone https://github.com/mirror/busybox.git
cd busybox/
```

---

## Initialize BusyBox

**`Configure`** BusyBox and de/select the binaries needed! ... Then **`Compile`** BusyBox to generate all binaries.

```bash
# export your target's toolchain on the system
# choose the right compiler that correspond to your board
# and choose the correct architecture
export CROSS_COMPILE=${target_toolchain_location}/arm-cortexa9_neon-linux-musleabihf-
export ARCH=arm
# Example: export CROSS_COMPILE=~/x-tools/arm-cortexa9_neon-linux-musleabihf/bin/arm-cortexa9_neon-linux-musleabihf-

# Configure busybox to be static build from menuconfig
# Choose the binaries you want to include/exclude
make menuconfig

#build the busybox to get the minimal command
make

# generate the rootfs
make install

# this will create folder name _install has all binary
```

> :white_check_mark: After successfully finishing ... this will create folder name \_install has all binary

---

## Setup `Rootfs`

First you need to **initialize** the rootfs _(root file system)_ directory in home for example and set it up ... then this rootfs is to be copied to the SD Card Rootfs Partition.

> :warning: Make sure that **`owner`** of everything in rootfs directory is the **`root`** ... **NOT YOUR USER!**

```bash
# create directory rootfs (in home for example)
mkdir rootfs      # At home for example!
```

---

## NFS Connection

Network File System _(NFS)_ connection for mounting `Rootfs`

### Install NFS

```bash
# Install an NFS server on your host
sudo apt install nfs-kernel-server

# Add exported directory to `/etc/exports` file, with target ip as follows
/PathToDirectory/rootfs 192.168.0.100(rw,no_root_squash,no_subtree_check)
# Example --> /media/enigma/rootfs 192.168.0.77(rw,no_root_squash,no_subtree_check)

# Ask NFS server to apply this new configuration (reload this file)
sudo exportfs -r
```

---

### Setup `Bootargs` in `U-Boot`

```bash
setenv bootargs console=ttyXXX root=/dev/nfs ip=192.168.0.100:::::eth0 nfsroot=192.168.0.1:/home/fady/Documents/busybox/_install,nfsvers=3,tcp rw init=/sbin/init

# make sure the console tty represet the machine you working on

# Example --> setenv bootargs console=ttyAMA0 root=/dev/nfs ip=192.168.0.77:::::eth0 nfsroot=192.168.0.1:/media/enigma/rootfs,nfsvers=3,tcp rw init=/sbin/init
```

---

## `Initramfs`

Also called _`initrd`_ ... `initramfs` _(initial ramdisk filesystem)_ is a temporary, early root filesystem that is mounted before the real root filesystem becomes available during the Linux kernel's initialization process ... It is commonly used in the boot process to perform tasks such as loading essential kernel modules, configuring devices, and preparing the system for the transition to the actual root filesystem.

**Why?**

- to check **integrity** of the primary rootfs _(in sd for example)_ ... like when a new rootfs is being flashed.
- choose what's necessary to be mounted and used from rootfs initially.

> :exclamation: `initramfs` is removed once everything is done, and the **init process** takes control!

---

### Making `initramfs`

An initial RAM filesystem, or initramfs, is a compressed **cpio archive**. cpio is an old Unix archive format, similar to TAR and ZIP but **easier to decode and so requiring less code in the kernel**. You need to configure your kernel with CONFIG_BLK_DEV_INITRD to support initramfs.

> :exclamation: Make sure do not includes **kernel modules** in the initramfs as it will take much space.

```bash

```
