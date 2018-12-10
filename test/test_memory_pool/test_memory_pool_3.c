#include "test.h"

static_LST(lst3);
static_MEM(mem3, 1, sizeof(unsigned));

static unsigned sent = 0;
static unsigned received = 0;

static void proc1()
{
	void   * p;
	unsigned event;

 	event = lst_wait(lst3, &p);                  assert_success(event);
	        received = *(unsigned *)p;           assert(sent == received);
            mem_give(mem3, p);
	        tsk_stop();
}

static void proc2()
{
	void   * p;
	unsigned event;

 	event = mem_wait(mem3, &p);                  assert_success(event);
	        *(unsigned *)p = sent = rand();
	        lst_give(lst3, p);
	        tsk_stop();
}

static void test()
{
	unsigned event;
		                                         assert_dead(tsk1);
	        tsk_startFrom(tsk1, proc1);
		                                         assert_dead(tsk2);
	        tsk_startFrom(tsk2, proc2);
	event = tsk_join(tsk2);                      assert_success(event);
	event = tsk_join(tsk1);                      assert_success(event);
}

void test_memory_pool_3()
{
	int i;
	TEST_Notify();
	mem_bind(mem3);
	for (i = 0; i < PASS; i++)
		test();
}