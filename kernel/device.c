/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <kernel_consts.h>
/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

 int init_time=0; // Needed to find the duration of sleep


// kernel_up_time needed from time.c to know the number of ticks for a device to sleep.

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
// Initialize all the devices and their tcb
	int m;
	for(m = 0; m < NUM_DEVICES; m++) 
	{
		devices[m].next_match = dev_freq[m];
		devices[m].sleep_queue = NULL;
	}

}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{

  // Find the calling task tcb and put it to sleep queue.
 // defined in ctx_switch.c
	tcb_t *current_tcb = get_cur_tcb();

	current_tcb->sleep_queue = devices[dev].sleep_queue;
	devices[dev].sleep_queue = current_tcb;
    // Function in CTX_SWITCH.c  removes the highest priority(currently running task) from 
    // run queue and does a context switch with next runnable task
	dispatch_sleep();
	
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */

// The function is called from c_irq_handler.c
 // The void irq_handler() function calls dev_update
void dev_update(unsigned long millis __attribute__((unused)))
{

	int i;
	tcb_t *tmp;;

// Find the task in the sleep queue which was sleeping
// if the sleep duration matches with the number of ticks, then wake it up and add to run queue.	
	for(i = 0; i < NUM_DEVICES; i++)
	{
	 	if(devices[i].next_match <= millis)
	 	{
			while(devices[i].sleep_queue != NULL)
			{
				tmp = devices[i].sleep_queue; // find the device which was sleeping
				runqueue_add(tmp,tmp->cur_prio); // add it to run queue
				devices[i].sleep_queue = tmp->sleep_queue; // remember the task which had caused event_wait.
				tmp->sleep_queue = NULL; // device is not asleep anymore.
			}

			// Check for integer overflow of the next_match counter.
			if((devices[i].next_match + dev_freq[i]) < devices[i].next_match) 
			{
				printf("overflow occured in next_match %d \n", i);
				
			}
			
			devices[i].next_match += dev_freq[i]; // update the next frequency value for sleeping
		}
	}

}

