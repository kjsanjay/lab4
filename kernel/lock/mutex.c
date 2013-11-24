/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <exports.h> // <= Remove before submission
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
	int i;
	for(i=0;i<OS_NUM_MUTEX;i++) {

		gtMutex[i].bAvailable = TRUE;
		gtMutex[i].pHolding_Tcb = NULL;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pSleep_queue = NULL;

	}


}

//Creats a mutex & returns its index

int mutex_create(void)
{
	int i;
	

	for(i=0;i<OS_NUM_MUTEX;i++)
	{
		if(gtMutex[i].bAvailable == TRUE) 
			break;
	}

	
	if(i == OS_NUM_MUTEX)
	{
		printf("Mutex limit exceeded\n");
		return -ENOMEM;
	}
		

	gtMutex[i].bAvailable = FALSE;
	return i;
	
}

int mutex_lock(int mutex)
{
	tcb_t* cur_task=get_cur_tcb();
	
	if(gtMutex[mutex].bAvailable==FALSE)
	{
		if(mutex < 0 || mutex > OS_NUM_MUTEX)
		{
			printf("Invalid Mutex\n");
			return -EINVAL;
		}
		//Check for deadlock. if currecnt task is already holding
		else if(gtMutex[mutex].pHolding_Tcb==cur_task)
		{
			return -EDEADLOCK;

		}
		else
		{
			if(gtMutex[mutex].bLock==1)
			{	//Mutex unavailable
				//Remove from run queue & add to sleep of mutex
				//Wait for mutex to be available
				while(gtMutex[mutex].bLock==1)
				{
					

					
				}

			}
			else
			{	//Mutex available
				gtMutex[mutex].bLock=1;
				gtMutex[mutex].pHolding_Tcb=cur_task();

			}
			



		}

	}
	else
	{ //User has not created the mutex
		printf("User has not created this mutex\n");
		return -EINVAL;

	}


	
	return 0;


	//printf("coming to mutex_lock in mutex.c and value is: %d\n", mutex);
	//return 1; // fix this to return the correct value
}

int mutex_unlock(int mutex)
{
	printf("coming to mutex_unlock and value is: %d \n", mutex);
	
	return 0;
	//return 1; // fix this to return the correct value
}

