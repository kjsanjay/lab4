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
		gtMutex[i].pHolding_Tcb = 0x0;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pSleep_queue = 0x0;

	}


}

int mutex_create(void)
{
	int i;

	for(i=0;i<OS_NUM_MUTEX;i++){
		if(gtMutex[i].bAvailable == TRUE) break;
	}

	if(i == OS_NUM_MUTEX) return -ENOMEM;

	gtMutex[i].bAvailable = FALSE;
	return i;
	
}

int mutex_lock(int mutex  __attribute__((unused)))
{

	if(mutex < 0 || mutex > OS_NUM_MUTEX){
		printf("Invalid Mutex\n");
		return -EINVAL;
	}

	return 0;


	//printf("coming to mutex_lock in mutex.c and value is: %d\n", mutex);
	//return 1; // fix this to return the correct value
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	printf("coming to mutex_unlock and value is: %d \n", mutex);
	
	return 0;
	//return 1; // fix this to return the correct value
}

