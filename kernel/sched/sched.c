/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
	18-342 Lab4
	Group-34
	Sanjay K Jayasankar - sjayasan
	Shobhit Kukreti - skukreti
	Yashwant Vemuru - yvemuru
	Date: Nov 25, 2013
 */

#include <types.h>
#include <assert.h>
#include <task.h>
#include <kernel.h>
#include <config.h>
#include <sched.h>
#include "sched_i.h"
#include <kernel_consts.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>


tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
static void __attribute__((unused)) idle(void);
void sort_tasks(task_t *, size_t);



void sched_init(task_t* main_task)
{

	//set-up idle task
	tcb_t *idle_tcb =  &system_tcb[IDLE_PRIO];
	sched_context_t *idle_context;

	main_task->lambda =(task_fun_t) idle;
    main_task->data = NULL;
    main_task->stack_pos = 0;
    main_task->C = 0;
    main_task->T = 0;

    idle_context=&idle_tcb->context;
 	idle_context->r4 = (uint32_t)main_task->lambda;
    idle_context->r5 = (uint32_t)main_task->data;
    idle_context->r6 = (uint32_t)main_task->stack_pos;
    idle_context->r8 = (uint32_t)global_data;
    idle_context->sp = (void *)(idle_tcb->kstack_high);
    idle_context->lr = (void *)&idle;

	idle_tcb->native_prio = IDLE_PRIO;
    idle_tcb->cur_prio = IDLE_PRIO;
    idle_tcb->holds_lock = 0;
    idle_tcb->sleep_queue = NULL;

    runqueue_add(&system_tcb[IDLE_PRIO], IDLE_PRIO);
}




/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks, size_t num_tasks)
{
	unsigned int i;
	task_t idle_task;
	task_t *a_tasks = *tasks;
	//Initialize the runqueue
	//Clears run_list,run_bit & group_run_bits
	runqueue_init();
	// printf("Runqueue initialized\n");

	//Tasks will start from 0, edit the tcb.
	for(i=0;i<num_tasks;i++)
	{
		setup_task_context(&a_tasks[i], &system_tcb[i+1], i+1);

	}

	
	// Adding the alloted tasks onto runqueue
	// Start from 1 as opposed to 0, because of setup_task_context
	for(i=1;i<=num_tasks;i++)
	{
		runqueue_add(&system_tcb[i], system_tcb[i].native_prio);
	}

	
    // print_run_queue();

    // Creates idles task & adds to run queue
    sched_init(&idle_task); 

    dispatch_init(&system_tcb[IDLE_PRIO]);
    
	

}


void setup_task_context(task_t *task, tcb_t *tcb, uint8_t prio)
{	

	sched_context_t *task_context = &(tcb->context);
	tcb->native_prio = prio;
    tcb->cur_prio = prio;
    // printf("setting up context with %p %p %p\n", task->lambda, task->data, task->stack_pos);
    // printf("The priority of this task is %u\n", (tcb->native_prio));
    task_context->r4 = (uint32_t)task->lambda;
    task_context->r5 = (uint32_t)task->data;
    task_context->r6 = (uint32_t)task->stack_pos;
    task_context->r8 = (uint32_t)global_data;
    task_context->sp = (void *)(tcb->kstack_high);
    task_context->lr = launch_task;
    tcb->holds_lock = 0;
    tcb->sleep_queue = NULL;

}



