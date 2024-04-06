## Contents

- [Contents](#contents)
- [Yocto Tips](#yocto-tips)
  - [Size Optimization](#size-optimization)
    - [Build Directory](#build-directory)
    - [`rm_work` class](#rm_work-class)

---
## Yocto Tips
Discussing various tips for yocto and issues that you could face with yocto.

---
### Size Optimization

> :grey_exclamation: Adding a meta layer ... doesn't increase size or anything! ... size increases based on the used recipes!

#### Build Directory
When creating multiple images for various architectures ... Don't seperate the **`download`**, **`sstate-cache`** and **`tmp`** directories ... as this will cause `re-downloading`/`re-unpacking`/`re-compiling` and `re-building` in general from **scratch**!

Instead, make these directories the same through all your projects:

``` bash
# File: local.conf

# Same location for these directories
# To save disk-space & time
DL_DIR ?= "/path/to/your/poky/build/downloads"
SSTATE_DIR ?= "/path/to/your/poky/build/sstate-cache"
TMPDIR = "/path/to/your/poky/build/tmp"
```

#### `rm_work` class
**Significantly** reduces the size of tmp directory ... The **`rm_work class`** supports **deletion of temporary workspace**, which can ease your hard drive demands during builds.

Once the build system generates the packages for a recipe, the work files for that recipe are no longer needed. However, by default, the build system preserves these files for inspection and possible debugging purposes.
``` bash
# File: local.conf

# Delete work files ... to save disk space as the build progresses
INHERIT += "rm_work"
```

If you are modifying and building source code out of the work directory for a recipe, enabling `rm_work` will potentially result in your changes to the source being lost. To exclude some recipes from having their work directories deleted by `rm_work`, you can add the names of the recipe or recipes you are working on to the **`RM_WORK_EXCLUDE`** variable,
``` bash
# File: local.conf

# Exclude some recipes from having their work directories deleted by rm_work
RM_WORK_EXCLUDE += "openssh"
```

> :exclamation: For **`/tmp/work`**, you do not need all the workfiles of all recipes. You can specify which ones you are interested in your development.



---

md5sum COPYING.MIT
> :grey_exclamation: **`SRC_URI[md5sum]`** used to also be commonly used, but it is **deprecated** and should be replaced by **`SRC_URI[sha256sum]`** when updating existing recipes.


> Note that in order to add do_install(){...} to a recipe, you need to add `LDFLAGS` to the do_compile() function.

``` bash
bitbake -c do_install enigma  # To run the do_install() task
```

Output is generated at `./deploy/rpm/aarch64/enigma-0.1-r0.aarch64.rpm` for example. *(depending on the architecture and the recipe name)*