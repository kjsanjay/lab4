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

//#define DEBUG 0

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
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{



	
	return 1; // fix this; dummy return to prevent compiler warnings	
}
	


