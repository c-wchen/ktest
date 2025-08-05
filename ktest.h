#ifndef CU_TEST_H
#define CU_TEST_H

#include <linux/stdarg.h>

#define LOG_INFO(fmt, ...)  printk(KERN_INFO fmt " [%s:%d]\n", ##__VA_ARGS__, __func__, __LINE__)
#define LOG_PRINT(fmt, ...) printk(KERN_INFO fmt "\n", ##__VA_ARGS__)

/* ktest_string */

char* ktest_str_alloc(int size);
char* ktest_str_copy(const char* old);

#define KTEST_ALLOC(TYPE)		((TYPE*) kmalloc(sizeof(TYPE), GFP_KERNEL))

#define KTEST_FREE(ptr)		  \
	do {                      \
		if (ptr) {            \
			kfree(ptr);       \
		}                     \
		(ptr) = NULL;         \
	} while (0)

#define HUGE_STRING_LEN	1024
#define STRING_MAX		256
#define STRING_INC		256

typedef struct
{
	int length;
	int size;
	char* buffer;
} ktest_string;

void ktest_string_init(ktest_string* str);
ktest_string* ktest_string_new(void);
void ktest_string_delete(ktest_string *str);
void ktest_string_read(ktest_string* str, const char* path);
void ktest_string_append(ktest_string* str, const char* text);
void ktest_string_append_clear(ktest_string* str, char ch);
void ktest_string_append_format(ktest_string* str, const char* format, ...);
void ktest_string_insert(ktest_string* str, const char* text, int pos);
void ktest_string_resize(ktest_string* str, int new_size);

/* ktest */

typedef struct ktest ktest;

typedef void (*test_func)(ktest *);

struct ktest
{
	const char* name;
	test_func function;
	int failed;
	int ran;
	const char* message;
};

void ktest_init(ktest* t, const char* name, test_func function);
ktest* ktest_new(const char* name, test_func function);
void ktest_delete(ktest *t);
void ktest_run(ktest* tc);

/* Internal versions of assert functions -- use the public versions */
void ktest_fail_line(ktest* tc, const char* file, int line, const char* message2, const char* message);
void ktest_expect_line(ktest* tc, const char* file, int line, const char* message, int condition);
void ktest_expect_streq_line_msg(ktest* tc,
	const char* file, int line, const char* message,
	const char* expected, const char* actual);
void ktest_expect_inteq_line_msg(ktest* tc,
	const char* file, int line, const char* message,
	int expected, int actual);
void ktest_expect_ptreq_line_msg(ktest* tc,
	const char* file, int line, const char* message,
	void* expected, void* actual);

/* public assert functions */

#define FAIL(tc, ms)                          ktest_fail_line(  (tc), __FILE__, __LINE__, NULL, (ms))
#define EXPECT_COND(tc, ms, cond)             ktest_expect_line((tc), __FILE__, __LINE__, (ms), (cond))
#define EXPECT_TRUE(tc, cond)                 ktest_expect_line((tc), __FILE__, __LINE__, "assert failed", (cond))
#define EXPECT_STR_EQ(tc, ex, ac)             ktest_expect_streq_line_msg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define EXPECT_STR_EQ_MSG(tc, ms, ex, ac)     ktest_expect_streq_line_msg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define EXPECT_INT_EQ(tc, ex, ac)             ktest_expect_inteq_line_msg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define EXPECT_INT_EQ_MSG(tc, ms, ex, ac)     ktest_expect_inteq_line_msg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define EXPECT_PTR_EQ(tc,ex,ac)               ktest_expect_ptreq_line_msg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define EXPECT_PTR_EQ_MSG(tc, ms, ex, ac)     ktest_expect_ptreq_line_msg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define EXPECT_PTR_NOT_NULL(tc, p)            ktest_expect_line((tc),__FILE__,__LINE__,"null pointer unexpected",(p != NULL))
#define EXPECT_PTR_NOT_NULL_MSG(tc, msg, p)   ktest_expect_line((tc),__FILE__,__LINE__,(msg),(p != NULL))

/* ktest_suit */

#define MAX_TEST_CASES	1024

#define SUITE_ADD_TEST(SUITE,TEST)	ktest_suit_add(SUITE, ktest_new(#TEST, TEST))

typedef struct
{
	int count;
	ktest* list[MAX_TEST_CASES];
	int fail_count;

} ktest_suit;


void ktest_suit_init(ktest_suit* test_suite);
ktest_suit* ktest_suit_new(void);
void ktest_suit_delete(ktest_suit *ts);
void ktest_suit_add(ktest_suit* test_suite, ktest *test_case);
void ktest_suit_add_from_suit(ktest_suit* test_suite, ktest_suit* test_suite2);
void ktest_suit_run(ktest_suit* test_suite);
void ktest_suit_summary(ktest_suit* test_suite, ktest_string* summary);
void ktest_suit_details(ktest_suit* test_suite, ktest_string* details);

#endif /* CU_TEST_H */
