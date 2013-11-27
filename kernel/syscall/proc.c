/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
18-342 Lab4
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 25, 2013
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


// #define DEBUG


void sort_tasks(task_t *tasks,size_t num_tasks);


int check_task_validity(task_t* tasks,int num_tasks);





int task_create(task_t* tasks, size_t num_tasks)
{

    if(num_tasks > (OS_AVAIL_TASKS-1))
	{
         #ifdef DEBUG
            puts("Too many tasks\n");

        #endif
		return -EINVAL;
	}  

 
    if(check_task_validity(tasks,num_tasks)==0)
    {
        #ifdef DEBUG
            puts("Incorrect values passed to task create\n");
        #endif
        
        return -EFAULT;
    }

         #ifdef DEBUG
            puts("No Error");

        #endif
    
    //TODO: UB Test

    
    sort_tasks(tasks,num_tasks);
    
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
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}


/*Checks validity of task parameters */
int check_task_validity(task_t* tasks,int num_tasks)
{

    int i,j;

    #ifdef DEBUG
        puts("in check_task_validity\n");
        printf("\ntask:%p %p\n",tasks,&tasks );
    #endif

    

    if(tasks==NULL || 
    ((uintptr_t)tasks < (uintptr_t)USR_START_ADDR || 
        (uintptr_t)tasks >= (uintptr_t)USR_END_ADDR))
    {
        #ifdef DEBUG
        
        puts("Error in task address\n");
        #endif
        return 0;
    }


    for (i = 0; i < num_tasks; ++i)
    {

        if(tasks[i].lambda==NULL || 
            ((uintptr_t)tasks[i].lambda < (uintptr_t)USR_START_ADDR || 
                (uintptr_t)tasks[i].lambda >= (uintptr_t)USR_END_ADDR))
        {
            #ifdef DEBUG
            puts("Invalid_lambda");

            #endif
            return 0;
        }

                // validate stack_pos
        if(tasks[i].stack_pos==NULL || 
            ((uintptr_t)tasks[i].stack_pos < (uintptr_t)USR_START_ADDR || 
                (uintptr_t)tasks[i].stack_pos >= (uintptr_t)USR_END_ADDR))
        {
            #ifdef DEBUG
            puts("Invalid_stackpos");
            #endif
            return 0;
        }

    
    
        if(tasks[i].T==0)
        {
            #ifdef DEBUG
                puts("T=0 Error\n");
            #endif
            return 0;
        }
            

        if(tasks[i].C > tasks[i].T)
        {
            #ifdef DEBUG
                puts("C>T");
            #endif
            return 0;
        }
            

    }// end of for-loop

    for (i = 0; i < num_tasks-1; i++)
    {
        for(j=i+1;j<num_tasks;j++)
        {
            if(tasks[i].stack_pos==tasks[j].stack_pos)
            {
                #ifdef DEBUG
                    puts("Overlapping stacks!");
                #endif
                return 0;

            }


        }

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
                tasks[j+1].data=tmp.data;
                tasks[j+1].stack_pos=tmp.stack_pos;
                tasks[j+1].C=tmp.C;
                tasks[j+1].T=tmp.T;

            }
        }
    }
}

