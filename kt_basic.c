#include <linux/printk.h>
#include "ktest.h"

void test_add(ktest* kt) {
    printk(KERN_INFO "Test Add\n");
    int a = 12;
    EXPECT_INT_EQ(kt, a, 14);
    return;
}