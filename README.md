[root@cnode130 ktest]# ./make-tests.sh


[root@cnode130 ktest]# make
touch "/root/workspace/ktest/build/Makefile"
make -C /usr/src/kernels/5.14.0-580.el9.x86_64 	 M=/root/workspace/ktest/build src=/root/workspace/ktest  modules
make[1]: Entering directory '/usr/src/kernels/5.14.0-580.el9.x86_64'
  CC [M]  /root/workspace/ktest/build/ktest_main.o
  LD [M]  /root/workspace/ktest/build/kutest.o
  MODPOST /root/workspace/ktest/build/Module.symvers
  LD [M]  /root/workspace/ktest/build/kutest.ko
  BTF [M] /root/workspace/ktest/build/kutest.ko
Skipping BTF generation for /root/workspace/ktest/build/kutest.ko due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/kernels/5.14.0-580.el9.x86_64'


[root@cnode130 ktest]# ll build/kutest.ko
-rw-r--r--. 1 root root 319200 Aug  5 13:37 build/kutest.ko

