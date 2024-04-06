## Contents

- [Contents](#contents)
- [BitBake](#bitbake)
  - [BitBake Example](#bitbake-example)
    - [Setting Up the BitBake Environment](#setting-up-the-bitbake-environment)
    - [Create project example directory](#create-project-example-directory)
    - [Setting `BBPATH`](#setting-bbpath)
    - [Create `conf/bitbake.conf`](#create-confbitbakeconf)
    - [Create `classes/base.bbclass`](#create-classesbasebbclass)
    - [Create `Layer`](#create-layer)
    - [Create `Recipe`](#create-recipe)
    - [Create `conf/bblayers.conf`](#create-confbblayersconf)
    - [Run `bitbake` with a target](#run-bitbake-with-a-target)
  - [Create a package](#create-a-package)
    - [Create a `meta layer`](#create-a-meta-layer)
    - [Create a `recipe`](#create-a-recipe)
    - [Add the recipe to the layer](#add-the-recipe-to-the-layer)
    - [Build the recipe](#build-the-recipe)
  - [Recipe Structure](#recipe-structure)

---
## BitBake
BitBake is a generic `task execution engine` *(program written in python language)* that allows **`shell`** and **`Python`** tasks to be run efficiently and in parallel while working within complex inter-task dependency constraints ... BitBake interprets **metadata**, decides what tasks are required to run, and executes those tasks. Similar to GNU Make, BitBake controls how software is built. GNU Make achieves its control through "makefiles". BitBake uses "recipes".



- **BitBake** is the **task executor** for the OpenEmbedded build system.
- **BitBake** executes tasks according to provided **`metadata`** that builds up the tasks. Metadata contains recipes, and recipes contain:
  -  *(.bb)* files that define the tasks to be run and the dependencies between those tasks.
  -  *(.conf)* files that provide configuration information to BitBake.
  -  *(.bbclass)* files that provide common code and functionality to recipes.
  -  *(.inc)* files that provide additional metadata to recipes.
-  **BitBake** includes a **fetcher** library for obtaining source code from various places such as source control systems or websites.
-  **BitBake** includes a **parser** that reads the metadata and determines the tasks to be run.


---
### BitBake Example
After obtaining bitbake whether independently using bitbake repo. ... or by using poky repo. ... **we can create a simple example to understand how bitbake works.**

#### Setting Up the BitBake Environment
**The steps are explicitly illustrated here:** [bitbake-user-manual-hello-example](https://docs.yoctoproject.org/bitbake/bitbake-user-manual/bitbake-user-manual-hello.html)

First, you need to be sure that you can run BitBake. Set your working directory to where your local BitBake files are and run the following command: **`Terminal`**
``` bash
# Make sure your inside bitbake directory ... 
./bin/bitbake --version # --> BitBake Build Tool Core version 2.3.1

# The recommended method to run BitBake is from a directory of your choice. To be able to run BitBake from any directory, you need to add the executable binary to your binary to your shell’s environment PATH variable
echo $PATH    # By your current PATH variable

# Add the directory location for the BitBake binary to the PATH
export PATH=/path/to/poky/bitbake/bin:$PATH
# Example --> export PATH=~/Desktop/NTI/yocto/poky/bitbake/bin:$PATH
```

#### Create project example directory
Set up a directory for the *(like “Hello World”)* project at any location desired ... like under `home` directory
``` bash
mkdir ~/bitbtake_example1
cd ~/bitbtake_example1

bitbake # try running

# ERROR: The BBPATH variable is not set and bitbake did not find a conf/bblayers.conf file in the expected location. Maybe you accidentally invoked bitbake from the wrong directory?
```

#### Setting `BBPATH`
You can set `BBPATH` variable in the same manner that you set PATH earlier in the appendix.
``` bash
export BBPATH="$PWD"  # PWD --> your project directory

bitbake # try running
# ERROR: Unable to parse /home/scott-lenovo/bitbake/lib/bb/parse/__init__.py ...
# FileNotFoundError: [Errno 2] file conf/bitbake.conf not found in <projectdirectory>
```
> :exclamation: When specifying your project directory, **do not** use the tilde `(“~”)` character **as BitBake does not expand that character as the shell would.**

#### Create `conf/bitbake.conf`
The `conf/bitbake.conf` includes a number of configuration variables BitBake uses for **`metadata`** and **`recipe files`**. For this example, you need to create the file in your project directory and **define some key BitBake variables**
``` bash
# Make sure your inside bitbake directory ... 
mkdir conf

# Create bitbake.conf
cd conf
vim bitbake.conf
```
From within the **conf directory**, create **`bitbake.conf`** so that it contains the following:
``` bash
PN  = "${@bb.parse.vars_from_file(d.getVar('FILE', False),d)[0] or 'defaultpkgname'}"

TMPDIR  = "${TOPDIR}/tmp"
CACHE   = "${TMPDIR}/cache"
STAMP   = "${TMPDIR}/${PN}/stamps"
T       = "${TMPDIR}/${PN}/work"
B       = "${TMPDIR}/${PN}"
```
> :grey_exclamation: Without a value for **PN**, the variables **STAMP**, **T**, and **B**, prevent more than one recipe from working. You can fix this by either setting **PN** to have a value similar to what OpenEmbedded and BitBake use in the default bitbake.conf file

> :grey_exclamation: The **TMPDIR** variable establishes a directory that BitBake uses for build output and intermediate files other than the cached information used by the Setscene process

> :bulb: **You can always safely delete the tmp directory in order to rebuild a BitBake target. The build process creates the directory for you when you run BitBake.**

Attempt to run
``` bash
bitbake  # try running

# bb.parse.ParseError: ParseError in configuration INHERITs: Could not inherit file classes/base.bbclass
```

#### Create `classes/base.bbclass`
BitBake uses **class files** to provide common code and functionality. The minimally required class for BitBake is the **`classes/base.bbclass`** file. **The base class is implicitly inherited by every recipe**. BitBake looks for the class in the classes directory of the project.
``` bash
# Make sure your inside bitbake directory ... 
mkdir classes

# Move to the classes directory and then create the base.bbclass file by inserting this single line:
vim base.bbclass

# Then add a minimal task
addtask build

bitbake # try running
# Nothing to do. Use 'bitbake world' to build everything, or run 'bitbake --help' for usage information.
```

#### Create `Layer`
While it is not really necessary for such a small example, it is good practice to create a layer in which to keep your code separate from the general metadata used by BitBake.

You need a recipe file and a layer configuration file in your layer. The configuration file needs to be in the conf directory inside the layer
``` bash
# Make sure your inside bitbake directory ... 
mkdir mylayer
cd mylayer
mkdir conf

# Move to the conf directory and create a layer.conf
cd conf
vim layer.conf

# Then add the following configurations
BBPATH .= ":${LAYERDIR}"
BBFILES += "${LAYERDIR}/*.bb"
BBFILE_COLLECTIONS += "mylayer"
BBFILE_PATTERN_mylayer := "^${LAYERDIR_RE}/"
LAYERSERIES_CORENAMES = "hello_world_example"
LAYERSERIES_COMPAT_mylayer = "hello_world_example"
```

#### Create `Recipe`
Inside your layer at the top-level, create a recipe file named **`printhello.bb`** that has the following:
``` bash
DESCRIPTION = "Prints Hello World"
# Make sure it's the same name as for the file
PN = 'printhello'
PV = '1'

python do_build() {
   bb.plain("********************");
   bb.plain("*                  *");
   bb.plain("*  Hello, World!   *");
   bb.plain("*                  *");
   bb.plain("********************");
}
```

Attempt to run
``` bash
bitbake printhello # try running

# ERROR: no recipe files to build, check your BBPATH and BBFILES?
```

#### Create `conf/bblayers.conf`
`conf/bblayers.conf` file locates the layers needed for the project. **This file must reside in the conf directory of the project**

``` bash
# Make sure your inside bitbake directory ... 
cd conf
vim bblayers.conf
```

``` bash
BBLAYERS ?= " \
    /home/enigma/Desktop/NTI/yocto/bitbtake_example1 mylayer \
"
```

#### Run `bitbake` with a target
``` bash
bitbake printhello

# It should be executing the receipe task .. and printing!
```
> :exclamation: **After the first execution**, **re-running** bitbake printhello again **will not result in a BitBake run** that prints the same console output. The reason for this is that the first time the **printhello.bb recipe’s do_build task executes successfully**, **BitBake writes a `stamp file` for the task**. Thus, the next time you attempt to run the task using that same bitbake command, BitBake notices the stamp and therefore determines that **the task does not need to be re-run**. **If you delete the tmp directory or run bitbake -c clean printhello and then re-run the build, the “Hello, World!” message will be printed again**.

> :exclamation: Upon closing the current terminal, the saved PATH & BBPATH variables are reset!

---
### Create a package
First, make sure you run the **source environment script** to set up the environment variables for the build system. *`(oe-init-build-env)`*. Then to create a package, you need to **create a recipe** for the **package** you are interested in. And you can **create a new layer** with a basic structure using the following command:

#### Create a `meta layer`
``` bash
# Create a new layer with a basic structure (conf/ recipes/ README)
bitbake-layers create-layer meta-enigma
```
> :grey_exclamation: You can change layer & recipe name to whatever you want!

#### Create a `recipe`
**A basic recipe:**
1. Create a recipe directory in the **`meta-enigma/`** directory --> **`recipes-enigma/`**
2. Create a recipe file in the **`recipes-enigma/`** directory --> **`enigma_0.1.bb`**
3. Create a **`files`** directory in the **`enigma_0.1/`** directory --> **`files/`** ... then add your source code file to it
4. Edit the **`enigma_0.1.bb`** recipe file to include the following:
> :exclamation: This is a simple example of a recipe that compiles a C program and installs the resulting binary into the target’s **`/usr/bin`** directory
``` bash
SUMMARY = "bitbake-layers enigma recipe"
DESCRIPTION = "ItzEnigma Recipe"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://enigma.c"
S = "${WORKDIR}"

do_compile (){
	${CC} ${LDFLAGS} enigma.c -o ${S}/enigma
}

do_install (){
	install -d ${D}${bindir}
	install -m 0755 enigma ${D}${bindir}/
}

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*  ItzEnigma !!!   *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build
```
#### Add the recipe to the layer
Add the **`meta-layer`** to the **`BBLAYERS`** variable in the **`conf/bblayers.conf`** file. Then you can build and use the recipe.

**There are two ways:**
- Using **`bitbake-layers add-layer mylayer`** command
- Manually adding the layer to the **`BBLAYERS`** variable in the **`conf/bblayers.conf`** file.

> :grey_exclamation: If the meta-layer is already added to your build, then no need to add it again.

#### Build the recipe
You can build this specific recipe using the following command:
``` bash
bitbake [your_recipe_name]
# Example --> bitbake enigma

# To clean the recipe
bitbake -c clean enigma
```
> :grey_exclamation: If you add the recipe to your image **`local.conf`** file (`IMAGE_INSTALL:append = " enigma"`), the recipe will be **built** and **installed** into the image when you build the image.

> :exclamation: **`bitbake`** will **automatically** run the **`do_compile()`** and **`do_install()`** ... if you have rm_work enabled, you might not be able to see the output binary file in the **`tmp/work`** directory.

---
### Recipe Structure
``` bash
SUMMARY = ""      # Summary of the package
DESCRIPTION = ""  # Description of the package
HOMEPAGE = ""     # Homepage of the package
LICENSE = ""      # License of the package (e.g. MIT)
SECTION = ""      # Section of the package
DEPENDS = ""      # Dependencies of the package
LIC_FILES_CHKSUM = ""   # Checksum of the license file (e.g. md5sum)
# file://${COREBASE}/meta/COPYING.MIT md5=3da9cfbcb788c80a0384361b4de20420

SRC_URI = ""      # Source URI of the package (e.g. git://github.com/username/project.git)
# Or for a local file (file:// )  BUT, make sure you have files/ directory in your recipe
# https://strace.io/files/${PV}/strace-${PV}.tar.xz --> meta/recipes-devtools/strace/strace_5.5

S = "${WORKDIR}"  # Source directory
# WORKDIR is the directory where the source code is unpacked and built. --> /path/to/poky/build/tmp/work/aarch64-poky-linux/enigma/0.1-r0

# Add the do_compile() task to compile the source code
# Add the do_install() task to install the compiled binary into the target’s /usr/bin directory
# (Optional) Add the do_display_banner() task to display a banner before the do_build() task
```
- **`bindir`** represent a configuration variable that added in bitbake which represent that the binary will be installed in `/usr/bin`
- **`sbindir`** represent that the binaries will be installed in `/usr/sbin`

