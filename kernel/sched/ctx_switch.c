/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

extern uint8_t highest_prio();
extern tcb_t* runqueue_remove(uint8_t);

extern void ctx_switch_full(volatile void* next_ctx, volatile void* cur_ctx);
extern void ctx_switch_half(volatile void* next_ctx);






/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	

	printf("Coming to dispatch_init\n");
	
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{

}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	tcb_t* curr_tcb = get_cur_tcb();
	tcb_t* next_tcb;
	uint8_t curr_task_prio;
	uint8_t next_highest_prio;

	curr_task_prio = get_cur_prio();
	runqueue_remove(curr_task_prio);

	next_highest_prio = highest_prio();
	// next_tcb = run_list[next_highest_prio];

	ctx_switch_full((volatile void*) &next_tcb->context,(volatile void*) &curr_tcb->context);

}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio; //fix this; dummy return to prevent compiler warning
}
/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return (tcb_t *)cur_tcb ; //fix this; dummy return to prevent compiler warning
}
