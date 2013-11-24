/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>
#include <kernel.h>
#include <kernel_consts.h>
#include <exports.h>


unsigned long sleep_counter; 
extern volatile unsigned long kernel_up_time;


unsigned long time_syscall(void)
{
	return(kernel_up_time*OS_TIMER_RESOLUTION);
 //return 1; /* remove this line after adding your code here */	
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
	 		 sleep_counter=millis;
			 sleep_counter=kernel_up_time + (sleep_counter/OS_TIMER_RESOLUTION);
			
			 while(kernel_up_time < sleep_counter);
			 return;
}
