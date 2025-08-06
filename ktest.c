#include <linux/string.h>
#include <linux/math.h>
#include <linux/slab.h>
#include "ktest.h"

/*-------------------------------------------------------------------------*
 * CuStr
 *-------------------------------------------------------------------------*/

char *ktest_str_alloc(int size)
{
    char *new_str = (char*) kmalloc(sizeof(char) * (size), GFP_KERNEL);
    return new_str;
}

void ktest_str_free(const char *str)
{
    kfree(str);
}

char *ktest_str_copy(const char* old)
{
    int len = strlen(old);
    char *new_str = ktest_str_alloc(len + 1);
    strcpy(new_str, old);
    return new_str;
}

/*-------------------------------------------------------------------------*
 * ktest_string
 *-------------------------------------------------------------------------*/

void ktest_string_init(ktest_string* str)
{
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char*) kmalloc(sizeof(char) * str->size, GFP_KERNEL);
    str->buffer[0] = '\0';
}

ktest_string *ktest_string_new(void)
{
    ktest_string* str = (ktest_string*) kmalloc(sizeof(ktest_string), GFP_KERNEL);
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char*) kmalloc(sizeof(char) * str->size, GFP_KERNEL);
    str->buffer[0] = '\0';
    return str;
}

void ktest_string_delete(ktest_string *str)
{
    if (NULL == str) {
        return;
    }
    if (str->buffer) {
        kfree(str->buffer);
    }
    kfree(str);
}

void ktest_string_resize(ktest_string* str, int new_size)
{
    str->buffer = (char*) krealloc(str->buffer, sizeof(char) * new_size, GFP_KERNEL);
    str->size = new_size;
}

void ktest_string_append(ktest_string* str, const char* text)
{
    int length;

    if (text == NULL) {
        text = "NULL";
    }

    length = strlen(text);
    if (str->length + length + 1 >= str->size) {
        ktest_string_resize(str, str->length + length + 1 + STRING_INC);
    }
    str->length += length;
    strcat(str->buffer, text);
}

void ktest_string_append_clear(ktest_string* str, char ch)
{
    char text[2];
    text[0] = ch;
    text[1] = '\0';
    ktest_string_append(str, text);
}

void ktest_string_append_format(ktest_string* str, const char* format, ...)
{
    va_list argp;
    char buf[HUGE_STRING_LEN];
    va_start(argp, format);
    vsprintf(buf, format, argp);
    va_end(argp);
    ktest_string_append(str, buf);
}

void ktest_string_insert(ktest_string* str, const char* text, int pos)
{
    int length = strlen(text);
    if (pos > str->length) {
        pos = str->length;
    }
    if (str->length + length + 1 >= str->size) {
        ktest_string_resize(str, str->length + length + 1 + STRING_INC);
    }
    memmove(str->buffer + pos + length, str->buffer + pos, (str->length - pos) + 1);
    str->length += length;
    memcpy(str->buffer + pos, text, length);
}

/*-------------------------------------------------------------------------*
 * ktest
 *-------------------------------------------------------------------------*/

void ktest_init(ktest* t, const char* name, test_func function)
{
    t->name = ktest_str_copy(name);
    t->failed = 0;
    t->ran = 0;
    t->message = NULL;
    t->function = function;
}

ktest *ktest_new(const char* name, test_func function)
{
    ktest* tc = NULL;
	KTEST_ALLOC_PTR(tc);
    ktest_init(tc, name, function);
    return tc;
}

void ktest_delete(ktest* t)
{
    ktest_str_free(t->name);
    KTEST_FREE(t->message);
    KTEST_FREE(t);
    return;
}

void ktest_run(ktest* tc)
{
    LOG_PRINT("===========> running %s <===========", tc->name);

    tc->ran = 1;
    tc->function(tc);
}

static void ktest_fail_internal(ktest* tc, const char* file, int line, ktest_string* string)
{
    char buf[HUGE_STRING_LEN];

    sprintf(buf, "%s:%d: ", file, line);
    ktest_string_insert(string, buf, 0);

    tc->failed = 1;
    tc->message = string->buffer;
}

void ktest_fail_line(ktest* tc, const char* file, int line, const char* message2, const char* message)
{
    ktest_string string;

    ktest_string_init(&string);
    if (message2 != NULL) {
        ktest_string_append(&string, message2);
        ktest_string_append(&string, ": ");
    }
    ktest_string_append(&string, message);
    ktest_fail_internal(tc, file, line, &string);
}

void ktest_expect_line(ktest* tc, const char* file, int line, const char* message, int condition)
{
    if (condition) {
        return;
    }
    ktest_fail_line(tc, file, line, NULL, message);
}

void ktest_expect_streq_line_msg(ktest* tc, const char* file, int line, const char* message,
                                 const char *expected, const char *actual)
{
    ktest_string string;
    if ((expected == NULL && actual == NULL) ||
        (expected != NULL && actual != NULL &&
         strcmp(expected, actual) == 0)) {
        return;
    }

    ktest_string_init(&string);
    if (message != NULL) {
        ktest_string_append(&string, message);
        ktest_string_append(&string, ": ");
    }
    ktest_string_append(&string, "expected <");
    ktest_string_append(&string, expected);
    ktest_string_append(&string, "> but was <");
    ktest_string_append(&string, actual);
    ktest_string_append(&string, ">");
    ktest_fail_internal(tc, file, line, &string);
}

void ktest_expect_inteq_line_msg(ktest* tc, const char* file, int line, const char* message,
                                 int expected, int actual)
{
    char buf[STRING_MAX];
    if (expected == actual) {
        return;
    }
    sprintf(buf, "expected <%d> but was <%d>", expected, actual);
    ktest_fail_line(tc, file, line, message, buf);
}

void ktest_expect_ptreq_line_msg(ktest* tc, const char* file, int line, const char* message,
                                 void *expected, void *actual)
{
    char buf[STRING_MAX];
    if (expected == actual) {
        return;
    }
    sprintf(buf, "expected pointer <0x%p> but was <0x%p>", expected, actual);
    ktest_fail_line(tc, file, line, message, buf);
}


/*-------------------------------------------------------------------------*
 * ktest_suit
 *-------------------------------------------------------------------------*/

void ktest_suit_init(ktest_suit* test_suite)
{
    test_suite->count = 0;
    test_suite->fail_count = 0;
}

ktest_suit *ktest_suit_new(void)
{
    ktest_suit* test_suite = NULL;
    KTEST_ALLOC_PTR(test_suite);
    ktest_suit_init(test_suite);
    return test_suite;
}

void ktest_suit_delete(ktest_suit *ts)
{
    for (int i = 0 ; i < ts->count ; ++i) {
        ktest* test_case = ts->list[i];
        ktest_delete(test_case);
    }
    KTEST_FREE(ts);
}

void ktest_suit_add(ktest_suit* test_suite, ktest *test_case)
{
    test_suite->list[test_suite->count] = test_case;
    test_suite->count++;
}

void ktest_suit_add_from_suit(ktest_suit* test_suite, ktest_suit* test_suite2)
{
    int i;
    for (i = 0 ; i < test_suite2->count ; ++i) {
        ktest* test_case = test_suite2->list[i];
        ktest_suit_add(test_suite, test_case);
    }
}

void ktest_suit_run(ktest_suit* test_suite)
{
    int i;
    for (i = 0 ; i < test_suite->count ; ++i) {
        ktest* test_case = test_suite->list[i];
        ktest_run(test_case);
        if (test_case->failed) {
            test_suite->fail_count += 1;
        }
    }
}

void ktest_suit_details(ktest_suit* test_suite, ktest_string* details)
{
    int i;
    int fail_count = 0;

    ktest_string_append_format(details, "%d..%d\n", 1, test_suite->count);

    for (i = 0 ; i < test_suite->count ; ++i) {
        ktest* test_case = test_suite->list[i];

        if (test_case->failed) {
            fail_count++;
            ktest_string_append_format(details, "not ok %d - %s #%s\n",
                                       i + 1, test_case->name, test_case->message);
        } else {
            ktest_string_append_format(details, "ok %d - %s\n",
                                       i + 1, test_case->name);
        }
    }
}
