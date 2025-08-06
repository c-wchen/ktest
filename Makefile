obj-m := kutest.o


KT_FILES ?= $(notdir $(patsubst %.c,%.o,$(wildcard $(PWD)/kt_*.c)))
$(info ==> ${KT_FILES})

kutest-y := ktest.o ktest_main.o ${KT_FILES} 


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