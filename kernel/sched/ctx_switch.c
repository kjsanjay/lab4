/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>
#include <sched.h>
#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */


/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	
	cur_tcb = idle;
	print_run_queue();
	//dispatch_nosave();
	printf("Coming to dispatch_init\n");
	dispatch_nosave();
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
	tcb_t* current_tcb;
	tcb_t* next_tcb;
	uint8_t curr_task_prio;
	uint8_t next_task_prio;

	curr_task_prio = get_cur_prio();

	next_task_prio = highest_prio();
	
	if(next_task_prio<curr_task_prio)
	{	//Higher prio
		current_tcb = get_cur_tcb();
		next_tcb = &system_tcb[next_task_prio];
		runqueue_add(current_tcb, curr_task_prio);
		

		cur_tcb=runqueue_remove(next_task_prio);

		ctx_switch_full((volatile void*) &next_tcb->context,
			(volatile void*) &current_tcb->context);
	}
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	tcb_t* current_tcb;
	tcb_t* next_tcb;
	uint8_t curr_task_prio;
	uint8_t next_task_prio;

	curr_task_prio = get_cur_prio();

	next_task_prio = highest_prio();

	
	if(next_task_prio>curr_task_prio)
	{
		if(curr_task_prio!=IDLE_PRIO)
		{
			current_tcb = get_cur_tcb();
			runqueue_add(current_tcb, curr_task_prio);

		}
		next_tcb = &system_tcb[next_task_prio];
		
		if(curr_task_prio!=IDLE_PRIO)
			cur_tcb=runqueue_remove(next_task_prio);
		else 
			cur_tcb=next_tcb;
		ctx_switch_half((volatile void*) &next_tcb->context);
	}

	


}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	
	tcb_t* current_tcb;
	tcb_t* next_tcb;
	uint8_t curr_task_prio;
	uint8_t next_task_prio;

	curr_task_prio = get_cur_prio();

	next_task_prio = highest_prio();
	
	
	
	current_tcb = get_cur_tcb();
	next_tcb = &system_tcb[next_task_prio];
	runqueue_add(current_tcb, curr_task_prio);
	
	cur_tcb=runqueue_remove(next_task_prio);

	ctx_switch_full((volatile void*) &next_tcb->context,
		(volatile void*) &current_tcb->context);

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
