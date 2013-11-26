/*

18-342 Lab4
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 25, 2013
 */

/*Implementation of custom SWI Handler*/


#include <bits/fileno.h>
#include <bits/errno.h>
#include <bits/swi.h>

#include <types.h>
#include <assert.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

#include <exports.h>
#include <syscall.h>
#include <kernel_consts.h>
#include <lock.h>

// Special Values
#define EOT 				4
#define BACKSPACE 			8
#define DELETE 				127
#define NEW_LINE 			10
#define CARRIAGE_RETURN		13





extern volatile unsigned long kernel_up_time;

int C_SWI_Handler(int swi_num, unsigned* param)
{
	//param[0] - fd/ exit code
	//param[1] - string
	//param[2] - length
	// char* buf = (char*) param[1];
	//unsigned int i = 0;
	//unsigned long sleep_counter=0;

	switch (swi_num)
	{
		

		// Read syscall
		case READ_SWI:
			
			param[0]=read_syscall(param[0],(void*)param[1],(size_t)param[2]);
			// printf("ret=%d\n",i );

		break;

		//Write sycall
		case WRITE_SWI:
		
			param[0]=write_syscall(param[0],(void*)param[1],param[2]);
		break;

		case TIME_SWI:
			param[0] = time_syscall();
			
		break;

		case SLEEP_SWI:

		sleep_syscall(param[0]);
		param[0] = 0;

		break;

		case CREATE_SWI:

		param[0]=task_create((task_t *)param[0],(size_t)param[1]);

		//printf("Coming to create swi in C_SWI_Handler\n");

		break;

		case MUTEX_CREATE:
		disable_interrupts();
		param[0]=mutex_create();

		break;

		case MUTEX_LOCK:
		disable_interrupts();
		param[0]=mutex_lock((int) param[0]);

		break;

		case MUTEX_UNLOCK:


		param[0]=mutex_unlock((int) param[0]);

		break;

		case EVENT_WAIT:


		param[0]=event_wait((int) param[0]);

		break;


		default:
			//For invalid SWI number
			puts("Invalid SWI no.\n");
			param[0]=  -0xBADC0DE;
		break;


	}
	
	return param[0];
	
}
