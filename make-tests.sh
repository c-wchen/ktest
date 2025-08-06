#!/bin/bash

# Auto generate single AllTests file for ktest.
# Searches through all *.c files in the current directory.
# Prints to stdout.
# Author: Asim Jalis
# Date: 01/08/2003

FILES=kt_*.c

if [ -e ktest_main.c ]; then
    rm -rf ktest_main.c
fi

#if test $# -eq 0 ; then FILES=*.c ; else FILES=$* ; fi

echo '

/* This is auto-generated code. Edit at your own peril. */
#include <linux/kernel.h>
#include <linux/module.h>
#include "ktest.h"

' >> ktest_main.c

cat $FILES | grep '^void test_' | 
    sed -e 's/(.*$//' \
        -e 's/$/(ktest*);/' \
        -e 's/^/extern /' >> ktest_main.c

echo \
'

int run_all_tests(void)
{
    ktest_string *output = ktest_string_new();
    ktest_suit* suite = ktest_suit_new();

' >> ktest_main.c

cat $FILES | grep '^void test_' | 
    sed -e 's/^void //' \
        -e 's/(.*$//' \
        -e 's/^/    SUITE_ADD_TEST(suite, /' \
        -e 's/$/);/' >> ktest_main.c

echo \
'
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
    run_all_tests();
    return 0;
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

' >> ktest_main.c