## 为什么要有ktest
* 简化调试流程，有时需要试一下内核的数据结构是怎么用、宏的大小、...
* 提供出一种便捷的单元测试方法


## 怎么用ktest

### 规则

> 约定俗成，简化配置流程

测试文件名规则```kt_*.c```

测试用例规则```void test_xxx(ktest* kt)```

```c
// kt_basic.c
#include <linux/printk.h>
#include "ktest.h"

void test_add(ktest* kt)
{
    printk(KERN_INFO "Test Add\n");
    int a = 12;
    EXPECT_INT_EQ(kt, a, 14);
    return;
}
```

### 编译
1. make_tests.sh: 生成模块入口文件
2. make: 模块编译
3. ll build/kutest.ko: 查看生成模块

```bash
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
```



###  运行

插入模块：insmod build/kutest.ko

删除模块： rmmod build/kutest.ko

查看输出信息, 两种方式都可以
```bash
dmesg -w

cat /var/log/messages
```