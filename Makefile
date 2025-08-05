obj-m := kutest.o

kutest-y := ktest.o ktest_main.o \
			kt_basic.o kt_btree.o kt_radix_tree.o 


CURRENT_PATH:=${PWD} 
LINUX_KERNEL_VER:=$(shell uname -r)
LINUX_KERNEL_PATH:=/usr/src/kernels/${LINUX_KERNEL_VER} 	

BUILD_PATH=${PWD}/build
BUILD_PATH_MAKEFILE=${PWD}/build/Makefile

CONFIG_MODULE_SIG=n                

all:${BUILD_PATH_MAKEFILE}
	make -C $(LINUX_KERNEL_PATH) M=$(BUILD_PATH) src=${CURRENT_PATH} modules
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(BUILD_PATH) src=${CURRENT_PATH} clean
	rm -rf ${BUILD_PATH}

$(BUILD_PATH):
	mkdir -p "$@"

$(BUILD_PATH_MAKEFILE): $(BUILD_PATH)
	touch "$@"