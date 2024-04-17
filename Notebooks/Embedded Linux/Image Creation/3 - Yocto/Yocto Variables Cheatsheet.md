# Contents

- [Contents](#contents)
  - [`local.conf` Variables](#localconf-variables)

---

## `local.conf` Variables

|       Variable        |             Description              |          Default Value          |
| :-------------------: | :----------------------------------: | :-----------------------------: |
| **BB_NUMBER_THREADS** | Number of parallel **BitBake** tasks |  `"${@bb.utils.cpu_count()}"`   |
|   **PARALLEL_MAKE**   |  Number of parallel **Make** tasks   | `"-j ${@bb.utils.cpu_count()}"` |
|      **DL_DIR**       |        **Download** directory        |     `"${TOPDIR}/downloads"`     |
|    **SSTATE_DIR**     |   **Shared State Cache** directory   |   `"${TOPDIR}/sstate-cache"`    |
|      **TMPDIR**       |      Build **Output** directory      |        `"${TOPDIR}/tmp"`        |
|      **MACHINE**      |         Target machine name          |           `"qemux86"`           |

> :grey_exclamation: The variable **`TOPDIR`** contains the full (`absolute`) path to the build environment
