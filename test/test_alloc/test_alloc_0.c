#include "test.h"

#define SIZE 256

static void proc2()
{
	size_t heap = sys_heapSize();
	size_t len1 = rand() % (SIZE) + 1;
	size_t len2 = rand() % (SIZE) + 1;
	void * buf1 = malloc(len1);                  ASSERT(buf1);
	void * buf2 = malloc(len2);                  ASSERT(buf2);
	memset(buf1, 0xFF, len1);
	memset(buf2, 0xFF, len2);
	       len1 = rand() % (SIZE) + 1;
	       len2 = rand() % (SIZE) + 1;
	       buf1 = realloc(buf1, len1);           ASSERT(buf1);
	       buf2 = realloc(buf2, len2);           ASSERT(buf2);
	memset(buf1, 0xFF, len1);
	memset(buf2, 0xFF, len2);
	free(buf1);
	free(buf2);                                  ASSERT(heap==sys_heapSize());
	tsk_stop();
}

static void proc1()
{
	stk_t *stk;
	tsk_t *tsk;
	int result;

	size_t heap = sys_heapSize();
	tsk = malloc(sizeof(tsk_t));                 ASSERT(tsk);
	stk = malloc(OS_STACK_SIZE);                 ASSERT(stk);
	tsk_init(tsk, 2, proc2, stk, OS_STACK_SIZE); ASSERT_dead(tsk);
	result = tsk_join(tsk);                      ASSERT_success(result);
	free(tsk);
	free(stk);                                   ASSERT(heap==sys_heapSize());
	tsk_stop();
}

static void test()
{
	tsk_t *tsk;
	int result;

	tsk = tsk_new(1, proc1);                     ASSERT(tsk);
	result = tsk_join(tsk);                      ASSERT_success(result);
}

void test_alloc_0()
{
	TEST_Notify();
	TEST_Call();
}
