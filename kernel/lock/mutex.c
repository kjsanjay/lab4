/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 *	18-342 Lab4
	Group-34
	Sanjay K Jayasankar - sjayasan
	Shobhit Kukreti - skukreti
	Yashwant Vemuru - yvemuru
	Date: Nov 25, 2013 
 */

 // #define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <kernel_consts.h>

#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif


mutex_t gtMutex[OS_NUM_MUTEX];

void add_to_mutex_sleep(mutex_t *mut,tcb_t *current_tcb);

/* void mutex_init()
	Initializes the queues, flags of each mutexes.
	Called when kernel boots-up

*/

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


/*
	int mutex_create(void)
	Make a mutex available to a user program
	Returns index of the mutex
*/

int mutex_create(void)
{
	int i;
	

	for(i=0;i<OS_NUM_MUTEX;i++)
	{
		if(gtMutex[i].bAvailable == TRUE) 
		{
			gtMutex[i].bAvailable = FALSE;
			gtMutex[i].pHolding_Tcb = NULL;
			gtMutex[i].bLock=FALSE;
			gtMutex[i].pSleep_queue=NULL;
			return i;
		}
			
	}

	#ifdef DEBUG_MUTEX
	printf("Mutex limit exceeded\n");
	#endif
	return -ENOMEM;
	
}

int mutex_lock(int mutex)
{
	tcb_t* current_tcb;
	mutex_t *mutex_ref;
	
	if(mutex < 0 || mutex >= OS_NUM_MUTEX)
	{
		#ifdef DEBUG_MUTEX
		printf("Invalid Mutex\n");
		#endif
		return -EINVAL;
	}

	
	mutex_ref=&gtMutex[mutex];

	//Check if mutex_create has been called
	if(mutex_ref->bAvailable==TRUE)
	{
		#ifdef DEBUG_MUTEX
		printf("Mutex has not been created\n");
		#endif
		return -EINVAL;
	}
	
	current_tcb=get_cur_tcb();
	//Check for deadlock. if current task is already holding
	if(mutex_ref->pHolding_Tcb==current_tcb)
	{
		#ifdef DEBUG_MUTEX
		printf("Deadlock!!\n");
		#endif
		return -EDEADLOCK;

	}
	

	

	if(mutex_ref->bLock==TRUE)
	{	//Mutex is already locked
		//Remove from run queue & add to sleep of mutex
		//Wait for mutex to be available
	
		//Add task to mutexes sleep queue
		add_to_mutex_sleep(mutex_ref,current_tcb);//->pSleep_queue,current_tcb);
		
		//Removes task from runqueue & context s\w
		
		dispatch_sleep();
		//Returns only after context switched-in
	
	}
	else
	{ //If mutex is available
		mutex_ref->bLock=TRUE;
		mutex_ref->pHolding_Tcb=current_tcb;
		current_tcb->cur_prio=0;
		current_tcb->holds_lock++;

	}
	
	return 0;
}

int mutex_unlock(int mutex)
{
	tcb_t* current_tcb,*next_tcb;
	mutex_t *mutex_ref;
	

	if(mutex < 0 || mutex >= OS_NUM_MUTEX)
	{
		#ifdef DEBUG_MUTEX
		printf("Invalid Mutex\n");
		#endif
		return -EINVAL;
	}
	
	mutex_ref=&gtMutex[mutex];

	if(mutex_ref->bAvailable==TRUE)
	{
		#ifdef DEBUG_MUTEX
		printf("Mutex has not been created\n");
		#endif
		return -EINVAL;
	}
	current_tcb=get_cur_tcb();
	
	if(mutex_ref->pHolding_Tcb!=current_tcb)
	{
		#ifdef DEBUG_MUTEX
		printf("Current task does not hold mutex.\n");
		#endif
		return -EPERM;
	}

	if(mutex_ref->bLock==FALSE)
	{//Mutex is already unlocked

		#ifdef DEBUG_MUTEX
		printf("Mutex already unlocked.\n");
		#endif

	}
	else
	{ //Already acquired mutex
		//Normal case
		//Clears unlock
		mutex_ref->bLock=FALSE;
		mutex_ref->pHolding_Tcb=NULL;
		current_tcb->holds_lock--;
		current_tcb->cur_prio=current_tcb->native_prio;

		if(mutex_ref->pSleep_queue!=NULL)
		{// Next task waiting for the mutex
			next_tcb=mutex_ref->pSleep_queue;
			
			mutex_ref->pSleep_queue=next_tcb->sleep_queue;

			mutex_ref->pHolding_Tcb=next_tcb;
			next_tcb->sleep_queue=NULL;
			mutex_ref->bLock=TRUE;

			runqueue_add(next_tcb,next_tcb->cur_prio);
			

		}
		
	}
	
	dispatch_save();
	
	return 0;	

}



void add_to_mutex_sleep(mutex_t *mut,tcb_t *current_tcb)
{
	
        tcb_t *prev_tcb = NULL;
        tcb_t *tmp_tcb;
        
        if(mut->pSleep_queue == NULL) 
        {
                mut->pSleep_queue = current_tcb;        
                current_tcb->sleep_queue = NULL;
                return;
        }

        tmp_tcb = mut->pSleep_queue;

        while(tmp_tcb != NULL) 
        {
                prev_tcb = tmp_tcb;
                tmp_tcb = tmp_tcb->sleep_queue;
        }

        prev_tcb->sleep_queue = current_tcb;
        current_tcb->sleep_queue = NULL;
}

