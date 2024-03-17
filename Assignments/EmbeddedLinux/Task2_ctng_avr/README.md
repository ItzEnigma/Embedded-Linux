# Task 2 _(Building AVR Toolchain)_

### First clone the CrossTool-NG repo. ... then go to a valid working commit _(like: **25f6dae8**)_

### After Cloning CrossTool-NG ... run the following
``` bash
./bootstrap                  # Build ct-ng
./configure --enable-local   # Configuring ct-ng ... (MAKE SURE python3 IS CHOSEN!!!)
```
> ### :exclamation: **If python3 is not chosen or showing errors and problems then**
> ``` bash
> sudo apt-get install python3-dev    # Install python3 then check it's added to /bin/
> ls /bin/ | grep python              # Checking if it's added
> # Symlink Python ... if it already exists then it would show that it already exist
> sudo ln -s /usr/bin/python3.10 /usr/bin/python3       # Make sure to choose your python3 version
>
> # Then re-run configuring of ct-ng
> ./configure --enable-local
> 
> ```
#### :arrow_forward: Continuity of _ct-ng_ commands for building toolchain
``` bash
# Generate the ct-ng toolchain generator
make
sudo make install
./ct-ng <your-config>      # Example --> ./ct-ng avr

# For custom configurations or modifications on the toolchain
# Like adding new language (golang), debugging method, ...etc.
./ct-ng menuconfig

# You can do this between building one toolchain and another (Specially useful when build fails)
./ct-ng distclean          # It's optional, BUT if build fails ... then it's a MUST!    

./ct-ng build              # When all set ... start generating your target toolchain
```

> ### :warning: You will face allot of libraries missing you need to get it by **_sudo apt install_**
> ### :white_check_mark: Once the build is complete you will notice x-tool file created on your home directory
