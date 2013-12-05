/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
	18-342 Lab4
	Group-34
	Sanjay K Jayasankar - sjayasan
	Shobhit Kukreti - skukreti
	Yashwant Vemuru - yvemuru
	Date: Nov 25, 2013
 */

#define DEBUG

#include <sched.h>

#ifdef DEBUG
#include <exports.h>

#endif


	//Look-up table for utilization
unsigned long u_UpperBound[62] = {
1000,828,780,757,743,735,729,724,721,718,
715,714,712,711,709,708,707,707,706,705,
705,704,704,703,703,702,702,702,701,701,
701,701,700,700,700,700,700,700,699,699,
699,699,699,699,699,698,698,698,698,698,
698,698,698,698,698,697,697,697,697,697,
697,697
};

void sort_tasks(task_t *tasks,size_t num_tasks); 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks, size_t num_tasks)
{
	

	task_t *a_tasks = *tasks;

	
	#ifdef UB_TEST
	size_t i,k;
	unsigned long calc_util;
	#endif

	sort_tasks(a_tasks,num_tasks);

	#ifdef UB_TEST
	for(k=1;k<=num_tasks;k++)
	{	
		calc_util=0;
		//Ci/Ti
		for(i=1;i<=k;i++) 
		{
			calc_util+=a_tasks[i-1].C*1000/a_tasks[i-1].T;
			printf("C=%lu T=%lu U=%lu\n",a_tasks[i-1].C,a_tasks[i-1].T,calc_util);

		}
		calc_util+=a_tasks[k-1].B*1000/a_tasks[k-1].T;
		printf("B=%lu T=%lu U=%lu\n",a_tasks[k-1].B,a_tasks[k-1].T,calc_util);
		if(calc_util>u_UpperBound[k-1])
		{
			return 0;
		}

	}
	#endif
	
	return 1; 
}
	

void sort_tasks(task_t *tasks,size_t num_tasks)
{
    unsigned int i,j;
    task_t tmp;
    for(i=0;i<num_tasks;i++)
    {
        for(j=0;j<num_tasks-1;j++)
        {
            if(tasks[j].T > tasks[j+1].T)
            {
                //Swap
                tmp.lambda=tasks[j].lambda;
                tmp.data=tasks[j].data;
                tmp.stack_pos=tasks[j].stack_pos;
                tmp.C=tasks[j].C;
                tmp.T=tasks[j].T;
                tmp.B=tasks[j].B;

                tasks[j].lambda=tasks[j+1].lambda;
                tasks[j].data=tasks[j+1].data;
                tasks[j].stack_pos=tasks[j+1].stack_pos;
                tasks[j].C=tasks[j+1].C;
                tasks[j].T=tasks[j+1].T;
                tasks[j].B=tasks[j+1].B;

                tasks[j+1].lambda=tmp.lambda;
                tasks[j+1].data=tmp.data;
                tasks[j+1].stack_pos=tmp.stack_pos;
                tasks[j+1].C=tmp.C;
                tasks[j+1].T=tmp.T;
                tasks[j+1].B=tmp.B;

            }
        }
    }
}


