#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/radix-tree.h>
#include "ktest.h"

RADIX_TREE(rt_ex_root, GFP_KERNEL);

#define LOG_INFO(fmt, ...) printk(KERN_INFO fmt " [%s:%d]\n", ##__VA_ARGS__, __func__, __LINE__)

static void radix_tree_example_init(void)
{
    char *ptr = NULL;
    LOG_INFO("init.");

    if (radix_tree_empty(&rt_ex_root)) {
        LOG_INFO("rt_ex_root is empty");
    }

    radix_tree_insert(&rt_ex_root, 10UL, "x10");
    radix_tree_insert(&rt_ex_root, 11UL, "x11");
    radix_tree_insert(&rt_ex_root, 128UL, "x128");

    if (radix_tree_empty(&rt_ex_root)) {
        LOG_INFO("rt_ex_root is empty");
    }

    ptr = (char *)radix_tree_lookup(&rt_ex_root, 10);

    LOG_INFO("cur ptr = %p, content = %s", ptr, ptr);
}

static void radix_tree_example_exit(void)
{
    LOG_INFO("exit.");
    (void)radix_tree_delete(&rt_ex_root, 10UL);
    (void)radix_tree_delete(&rt_ex_root, 11UL);
    (void)radix_tree_delete(&rt_ex_root, 128UL);
    return;
}

void test_radix_tree(ktest* kt)
{
    radix_tree_example_init();
    radix_tree_example_exit();
}