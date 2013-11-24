/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>


 extern void sort_tasks(task_t *, size_t);
// extern void setup_task_context(task_t, tcb_t, uint8_t);
extern int assign_schedule(task_t **,size_t);
extern void allocate_tasks(task_t **, size_t);
extern void sched_init(task_t*);
extern uint8_t highest_prio();
extern tcb_t* runqueue_remove(uint8_t);
extern void print_run_queue();







int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
  int bound_check;
  int high_prio;
  tcb_t* removed_task;
  task_t idle_task;

	if(num_tasks > (OS_AVAIL_TASKS-1))
	{
		printf("Too many tasks\n");
		return -EINVAL;
	}  


  bound_check = valid_addr(tasks, (num_tasks * sizeof(task_t)), USR_START_ADDR, USR_END_ADDR);
  if(bound_check == 0) 
  {
      printf("incorrect address of tasks passed to task create\n");
      return -EFAULT;
  }
	
	//TODO: check_task_validity();


    //assign_schedule(&tasks,num_tasks);
    //sort_tasks(tasks,num_tasks);
    //TODO: UB Test

    mutex_init();
    
    allocate_tasks(&tasks,num_tasks);

    sched_init(&idle_task);
    dispatch_init(&system_tcb[IDLE_PRIO]);
    high_prio = highest_prio();

    printf("Highest priority is %d\n", (int)high_prio);

    removed_task = runqueue_remove(1);
    printf("The removed runqueue task is %d\n" , (int) removed_task);
	    
    print_run_queue();




    return 0;


}

int event_wait(unsigned int dev  __attribute__((unused)))
{
  if(dev >= NUM_DEVICES)
  {
    return -EINVAL;
  }
  
  dev_wait(dev);

  return 0;

}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
