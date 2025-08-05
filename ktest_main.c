

/* This is auto-generated code. Edit at your own peril. */
#include <linux/kernel.h>
#include <linux/module.h>
#include "ktest.h"


extern void test_add(ktest*);
extern void test_btree(ktest*);
extern void test_radix_tree(ktest*);


int run_all_tests(void)
{
    ktest_string *output = ktest_string_new();
    ktest_suit* suite = ktest_suit_new();


    SUITE_ADD_TEST(suite, test_add);
    SUITE_ADD_TEST(suite, test_btree);
    SUITE_ADD_TEST(suite, test_radix_tree);

    ktest_suit_run(suite);
    ktest_suit_details(suite, output);
    LOG_PRINT("%s", output->buffer);
    int rc = suite->fail_count == 0 ? 0 : 1;
    ktest_string_delete(output);
    ktest_suit_delete(suite);
    return rc;
}

static int __init ktest_module_init(void)
{
    LOG_INFO("init");
    return run_all_tests();
}
static void __exit ktest_module_exit(void)
{
    LOG_INFO("exit");
}

module_init(ktest_module_init)
module_exit(ktest_module_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wchen");
MODULE_DESCRIPTION("ktest");


