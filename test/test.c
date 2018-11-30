#include "test.h"

#define       LOOP 7
#define       SIZE 64

static fun_t *test[SIZE];
static int    count = 0;

void test_add(fun_t *fun)
{
	assert(count < SIZE);
	test[count++] = fun;
}

static void test_init()
{
	TEST_Notify();
	LED_Init();
	srand(0);
}

static void test_fini()
{
	TEST_Notify();
	LEDs = 15;
}

int main()
{
	int i;

	test_init();
	
	TEST_AddUnit(test_alloc);
	TEST_AddUnit(test_spin_lock);
	TEST_AddUnit(test_once_flag);
	TEST_AddUnit(test_event);
	TEST_AddUnit(test_signal);
	TEST_AddUnit(test_flag);
	TEST_AddUnit(test_barrier);
	TEST_AddUnit(test_semaphore);
	TEST_AddUnit(test_mutex);
	TEST_AddUnit(test_fast_mutex);
	TEST_AddUnit(test_condition_variable);
	TEST_AddUnit(test_memory_pool);
	TEST_AddUnit(test_stream_buffer);
	TEST_AddUnit(test_message_buffer);
	TEST_AddUnit(test_mailbox_queue);
	TEST_AddUnit(test_event_queue);
	TEST_AddUnit(test_job_queue);
	TEST_AddUnit(test_timer);
	TEST_AddUnit(test_task);

	for (i = 0; i < count * LOOP; i++)
	{
		test[i      % count]();
		test[rand() % count]();
	}

	test_fini();

	tsk_stop();
}