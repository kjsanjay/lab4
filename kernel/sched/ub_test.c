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

//#define DEBUG

#include <sched.h>

#ifdef DEBUG
#include <exports.h>

#endif


 
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
	

	int i,k;
	unsigned long calc_util;
	task_t *a_tasks = *tasks;

	sort_tasks(a_tasks,num_tasks);


	for(k=1;k<=num_tasks;k++)
	{	
		calc_util=0;
		//Ci/Ti
		for(i=1;i<=k;i++) 
		{
			calc_util+=tasks[i-1].C*1000/tasks[i-1].T;

		}
		calc_util+=tasks[k-1].B*1000/tasks[k-1].T;
		if(calc_util>U_UpperBound[k-1])
		{
			puts("Schedule fails\n");
			return 0;
		}

	}
	return 1; 
}
	


