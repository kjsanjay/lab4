/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
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
void setup_task_context(task_t *task, tcb_t *tcb, uint8_t prio);
extern void runqueue_add(tcb_t*, uint8_t);
extern void runqueue_init();
extern void print_run_queue();




void sched_init(task_t* main_task  __attribute__((unused)))
{

	//set-up idle task
	tcb_t *idle_tcb =  &system_tcb[IDLE_PRIO];
	sched_context_t *idle_context;

	main_task->lambda =(task_fun_t) idle;
    main_task->data = NULL;
    main_task->stack_pos = system_tcb[IDLE_PRIO].kstack_high;
    main_task->C = 0;
    main_task->T = 0;

    idle_context=&idle_tcb->context;
 	idle_context->r4 = (uint32_t)main_task->lambda;
    idle_context->r5 = 0;
    idle_context->r6 = 0;
    idle_context->sp = (void *)(idle_tcb->kstack_high);
    idle_context->lr =  (void *)idle;

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
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	unsigned int i;
	task_t idle_task;
	//Initialize the runqueue
	//Clears run_list,run_bit & group_run_bits
	runqueue_init();
	printf("Runqueue has been initialized\n");



	task_t *a_tasks = *tasks;

	//tasks are sorted as per T or priority order
	sort_tasks(*tasks,num_tasks);

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

	
    

    sched_init(&idle_task);
    dispatch_init(&system_tcb[IDLE_PRIO]);

	print_run_queue();

}


void setup_task_context(task_t *task, tcb_t *tcb, uint8_t prio)
{	

	sched_context_t *context = &(tcb->context);
	tcb->native_prio = prio;
    tcb->cur_prio = prio;
    printf("setting up context with %p %p %p\n", task->lambda, task->data, task->stack_pos);
    context->r4 = (uint32_t)task->lambda;
    context->r5 = (uint32_t)task->data;
    context->r6 = (uint32_t)task->stack_pos;
    context->sp = (void *)(tcb->kstack_high);
    context->lr = launch_task;
    printf("The priority of this task is %u\n", (tcb->native_prio));
    //printf("after setting up context %u %u %u sp is %u \n", (tcb->context).r4, (tcb->context).r5, (tcb->context).r6, (tcb->context).sp);
    tcb->holds_lock = 0;
    tcb->sleep_queue = NULL;

}



void sort_tasks(task_t *tasks,size_t num_tasks)
{
	unsigned int i,j;
	task_t tmp;
	for(i=0;i<num_tasks;i++)
	{
		for(j=i;j<num_tasks-1;j++)
		{
			if(tasks[j].T > tasks[j+1].T)
			{
				//Swap
				tmp.lambda=tasks[j].lambda;
				tmp.data=tasks[j].data;
				tmp.stack_pos=tasks[j].stack_pos;
				tmp.C=tasks[j].C;
				tmp.T=tasks[j].T;

				tasks[j].lambda=tasks[j+1].lambda;
				tasks[j].data=tasks[j+1].data;
				tasks[j].stack_pos=tasks[j+1].stack_pos;
				tasks[j].C=tasks[j+1].C;
				tasks[j].T=tasks[j+1].T;


				tasks[j+1].lambda=tmp.lambda;
				tasks[j+1].data=tasks[j].data;
				tasks[j+1].stack_pos=tasks[j].stack_pos;
				tasks[j+1].C=tasks[j].C;
				tasks[j+1].T=tasks[j].T;

			}


		}


	}


}

