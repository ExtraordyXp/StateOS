#include "test.h"

static_SIG(sig3, 0);

static unsigned sent;

static void proc3()
{
	unsigned event;

	event = sig_wait(sig3, sigAll);              assert(event == sent);
 	        sig_give(sig2, event);
	        tsk_stop();
}

static void proc2()
{
	unsigned event;
		                                         assert_dead(tsk3);
	        tsk_startFrom(tsk3, proc3);          assert_ready(tsk3);
	event = sig_wait(sig2, sigAll);              assert(event == sent);
 	        sig_give(sig3, event);
	event = sig_wait(sig2, sigAll);              assert(event == sent);
 	        sig_give(sig1, event);
	event = tsk_join(tsk3);                      assert_success(event);
	        tsk_stop();
}

static void proc1()
{
	unsigned event;
		                                         assert_dead(tsk2);
	        tsk_startFrom(tsk2, proc2);          assert_ready(tsk2);
	event = sig_wait(sig1, sigAll);              assert(event == sent);
 	        sig_give(sig2, event);
	event = sig_wait(sig1, sigAll);              assert(event == sent);
 	        sig_give(&sig0, event);
	event = tsk_join(tsk2);                      assert_success(event);
	        tsk_stop();
}

static void proc0()
{
	unsigned event;
		                                         assert_dead(tsk1);
	        tsk_startFrom(tsk1, proc1);          assert_ready(tsk1);
	event = sig_wait(&sig0, sigAll);             assert(event == sent);
 	        sig_give(sig1, sent);
	event = sig_wait(&sig0, sigAll);             assert(event == sent);
	event = tsk_join(tsk1);                      assert_success(event);
	        tsk_stop();
}

static void test()
{
	unsigned event;
		                                         assert_dead(&tsk0);
	        tsk_startFrom(&tsk0, proc0);         assert_ready(&tsk0);
	        tsk_yield();
	        tsk_yield();
 	        sig_give(&sig0, sent = rand() % SIG_LIMIT);
	event = tsk_join(&tsk0);                     assert_success(event);
}

extern "C"
void test_signal_2()
{
	TEST_Notify();
	TEST_Call();
}
