# Contents

- [Contents](#contents)
  - [Building ARM Toolchain](#building-arm-toolchain)
    - [Downloading the CrossTool-NG](#downloading-the-crosstool-ng)
    - [Initialize the CrossTool-NG Environment](#initialize-the-crosstool-ng-environment)
    - [Building the Toolchain](#building-the-toolchain)
  - [Finalizing the Toolchain](#finalizing-the-toolchain)
    - [Binutils](#binutils)
    - [Sysroot](#sysroot)

---

## Building ARM Toolchain

> :exclamation: **IF you didn't initialize the CrossTool-NG from Previous _(Task 2)_ and build AVR toolchain ... then you need to do the following:**

---

### Downloading the CrossTool-NG

First clone the CrossTool-NG repo. ... then go to a valid working commit _(like: **25f6dae8**)_

### Initialize the CrossTool-NG Environment

```bash
./bootstrap                  # Build ct-ng
./configure --enable-local   # Configuring ct-ng ... (MAKE SURE python3 IS CHOSEN!!!)
```

> :exclamation: **If python3 is not chosen or showing errors and problems then**
>
> ```bash
> sudo apt-get install python3-dev    # Install python3 then check it's added to /bin/
> ls /bin/ | grep python              # Checking if it's added
> # Symlink Python ... if it already exists then it would show that it already exist
> sudo ln -s /usr/bin/python3.10 /usr/bin/python3       # Make sure to choose your python3 version
>
> # Then re-run configuring of ct-ng
> ./configure --enable-local
> ```

:arrow_forward: **Continuity of `ct-ng` commands for building toolchain**

### Building the Toolchain

**You are required to customize an arm toolchain for your project.**

Make sure it support:

- [ ] Musl library
- [ ] Make
- [ ] Strace

```bash
# Generate the ct-ng toolchain generator
make
sudo make install
./ct-ng <your-config>      # Example --> ./ct-ng arm-cortexa9

# For custom configurations or modifications on the toolchain
# Like adding new language (golang), debugging method, ...etc.
./ct-ng menuconfig

# You can do this between building one toolchain and another (Specially useful when build fails)
./ct-ng distclean          # It's optional, BUT if build fails ... then it's a MUST!

./ct-ng build              # When all set ... start generating your target toolchain
```

> :warning: You will face allot of libraries missing you need to get it by `_sudo apt install_`
>
> ```bash
> sudo apt install build-essential git autoconf bison flex texinfo help2man gawk \
> libtool-bin libncurses5-dev unzip
> ```

---

## Finalizing the Toolchain

:white_check_mark: Once the build is complete you will notice `x-tool` file created on your home directory (your specified path).

You will find the following directories:

- `bin`
- `lib`
- `include`
- `share`
- `arm-cortexa9_neon-linux-musleabihf`

---

### Binutils

Binary utilities are a set of programs _(tools)_ for compiling and linking applications and creating executable files/libraries. **It's substantial for the toolchain to work properly.** ... Some of the files included:

- `as` (Assembler)
- `ld` (Linker)
- `objcopy` (Object Copy)
- `objdump` (Object File Disassembler)
- `nm` (Symbol Table Viewer)
- `readelf` (ELF File Viewer)

> :white_check_mark: The `cross-toolchain` allow you to **compile** and **link** applications for the **target architecture**, from your host machine.

---

### Sysroot

Sysroot is a directory that contains the standard library and headers for the target system _(and all necessary files as specified during configuration)_ ... Some of the files included:

> :grey_exclamation: The sysroot is a **mirror** of the target system's directory structure and essential files (which will be populated at the rootfs of the target system)

- `/usr/lib` Necessary shared libraries _(.so)_ for **dynamically linking executables**, as well as static libraries _(.a)_ for statically linking.
- `/usr/include` Necessary header files for **compiling** applications.
- `/usr/bin` Necessary binaries for **executing** applications.
- `/lib` May include additional shared libraries or **kernel-essential** system libraries.

> :grey_exclamation: The sysroot should include all necessary files, libraries, and configuration settings required for the target system to run the software ... **and it should be a mirror to the directory structure and essential files present on the target system to ensure compatibility and proper functioning.**

---

> :exclamation: Note that the toolchain will be used for generating the **kernel**, **Bootloader**, **Rootfs** _(BusyBox)_, and **Applications** for the target system.
