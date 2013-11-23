/*

18-342 Lab3
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 10, 2013
 */

/*Implementation of custom SWI Handler*/


#include <bits/fileno.h>
#include <bits/errno.h>
#include <bits/swi.h>
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


extern inline void restore_OldSWI(void);
extern unsigned int getKernTime();
extern void exitProgram(int);


extern unsigned int globalsp_svc;
extern volatile unsigned long kernel_up_time;

void C_SWI_Handler(int swi_num, unsigned* param)
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
		//	i=write_syscall(param[0],(void*)param[1],param[2]);
			param[0]=write_syscall(param[0],(void*)param[1],param[2]);
		break;

		case TIME_SWI:
			//i=time_syscall();
			param[0] = time_syscall();
			
			//printf("%d\n",(int)param[0] );			
		break;

		case SLEEP_SWI:

		 sleep_syscall(param[0]);
			

		break;

		case CREATE_SWI:

		task_create((task_t *)param[0],(size_t)param[1]);

		//printf("Coming to create swi in C_SWI_Handler\n");

		break;

		case MUTEX_CREATE:

		mutex_create();

		break;

		case MUTEX_LOCK:

		mutex_lock((int) param[0]);

		break;

		case MUTEX_UNLOCK:


		mutex_unlock((int) param[0]);

		break;



		default:
			//For invalid SWI number
			puts("Invalid SWI no.\n");
			param[0]=  -0xBADC0DE;
		break;


	}
	
	//printf("ret at end: %d\n",i);
	//	putchar(i);
	// param[0]=i;
	//printf("return value: %d", param[0]);
	return; //param[0];
	}
