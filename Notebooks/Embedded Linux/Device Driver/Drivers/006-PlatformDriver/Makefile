obj-m = platform_cdriver.o
obj-m += AXZ.o


BDIR = /lib/modules/$(shell uname -r)/build

# If multiple targets exist ... make will require a target to be specified
# If only one target is specified, make will use that target
# The default one is all
build:
	make -C $(BDIR) M=$(PWD) modules
