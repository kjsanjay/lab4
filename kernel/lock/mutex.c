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
#include <kernel_consts.h>
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
	tcb_t* current_tcb=get_cur_tcb();
	mutex_t *mutex_ref=&gtMutex[mutex];
	
	if(mutex_ref->bAvailable==FALSE)
	{
		if(mutex < 0 || mutex > OS_NUM_MUTEX)
		{
			printf("Invalid Mutex\n");
			return -EINVAL;
		}
		//Check for deadlock. if currecnt task is already holding
		else if(mutex_ref->pHolding_Tcb==current_tcb)
		{
			return -EDEADLOCK;

		}
		else
		{
			if(mutex_ref->bLock==1)
			{	//Mutex is already locked
				//Remove from run queue & add to sleep of mutex
				//Wait for mutex to be available
				
					//Add task to mutexes sleep queue
					current_tcb->sleep_queue=mutex_ref->pSleep_queue;
					mutex_ref->pSleep_queue=current_tcb;
					
					//Removes task from runqueue & context s\w
					dispatch_sleep();
					//Returns only after context switched-in
					mutex_ref->bLock=1;
					mutex_ref->pHolding_Tcb=current_tcb;
					
					
				

			}
			else
			{	//Mutex available
				mutex_ref->bLock=1;
				mutex_ref->pHolding_Tcb=current_tcb;
			}
		}
	}
	else
	{ //User has not created the mutex
		printf("User has not created this mutex\n");
		return -EINVAL;

	}
	return 0;
}

int mutex_unlock(int mutex)
{
	printf("coming to mutex_unlock and value is: %d \n", mutex);
	
	return 0;
	//return 1; // fix this to return the correct value
}

