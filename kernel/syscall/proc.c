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
#include <lock.h>
#include <kernel_consts.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>


void sort_tasks(task_t *tasks,size_t num_tasks);


int check_task_validity(task_t* tasks,int num_tasks);





int task_create(task_t* tasks, size_t num_tasks)
{

	if(num_tasks > (OS_AVAIL_TASKS-1))
	{
		printf("Too many tasks\n");
		return -EINVAL;
	}  

 
    if(check_task_validity(tasks,num_tasks)==0)
    {
        printf("Incorrect values passed to task create\n");
        return -EFAULT;
    }

    
    
    //TODO: UB Test

    
    sort_tasks(tasks,num_tasks);
    dev_init();
    allocate_tasks(&tasks,num_tasks);


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

int check_task_validity(task_t* tasks,int num_tasks)
{

    int i;
    for (i = 0; i < num_tasks; ++i)
    {
        if(tasks[i].lambda==NULL || 
            ((uintptr_t)tasks[i].lambda < (uintptr_t)USR_START_ADDR || 
                (uintptr_t)tasks[i].lambda >= (uintptr_t)USR_END_ADDR))
        {
            return 0;
        }

        if(tasks[i].data==0)
            return 0;

        if(tasks[i].stack_pos==NULL || 
            ((uintptr_t)tasks[i].stack_pos < (uintptr_t)USR_START_ADDR || 
                (uintptr_t)tasks[i].stack_pos >= (uintptr_t)USR_END_ADDR))
        {
            return 0;
        }

        if(tasks[i].C==0)
            return 0;

        if(tasks[i].T==0)
            return 0;

    }


    return 1;
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

