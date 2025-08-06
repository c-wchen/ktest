#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/btree.h>
#include "ktest.h"

struct btree_head64 tbt;

void btree_example_init(void)
{
    LOG_INFO("init.");
    btree_init64(&tbt);

    btree_insert64(&tbt, 1UL, "X10", GFP_ATOMIC);
    btree_insert64(&tbt, 2UL, "X20", GFP_ATOMIC);
    btree_insert64(&tbt, 3UL, "X30", GFP_ATOMIC);
    btree_insert64(&tbt, 4UL, "X40", GFP_ATOMIC);
    btree_insert64(&tbt, 5UL, "X50", GFP_ATOMIC);

    unsigned long long key;
    char *val;
    btree_for_each_safe64(&tbt, key, val) {
        LOG_INFO("key: %llu, value: %s", key, val);
    }
    return;
}

void btree_example_exit(void)
{
    LOG_INFO("exit.");
    (void)btree_destroy64(&tbt);
    return;
}

void test_btree(ktest* kt)
{
    btree_example_init();
    btree_example_exit();
}