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
#include <exports.h> // <= Remove before submission
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif


mutex_t gtMutex[OS_NUM_MUTEX];
//void add_to_mutex_sleep(tcb_t* mutex_tcb,tcb_t *current_tcb);

void add_to_mutex_sleep(mutex_t *mut,tcb_t *current_tcb);


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
		#ifdef DEBUG_MUTEX
		printf("Mutex limit exceeded\n");
		#endif
		return -ENOMEM;
	}
		

	gtMutex[i].bAvailable = FALSE;
	return i;
	
}

int mutex_lock(int mutex)
{
	tcb_t* current_tcb;
	mutex_t *mutex_ref;
	
	if(mutex < 0 || mutex > OS_NUM_MUTEX)
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
		printf("User has not created this mutex\n");
		#endif
		return -EINVAL;
	}
	
	current_tcb=get_cur_tcb();
	//Check for deadlock. if current task is already holding
	if(mutex_ref->pHolding_Tcb==current_tcb)
	{
		#ifdef DEBUG_MUTEX
		printf("deadlock!!\n");
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
	
	mutex_ref->bLock=TRUE;
	mutex_ref->pHolding_Tcb=current_tcb;
	//enable_interrupts();
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
		printf("User has not created this mutex\n");
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
		mutex_ref->bLock=FALSE;
		mutex_ref->pHolding_Tcb=NULL;

		if(mutex_ref->pSleep_queue!=NULL)
		{// A task is waiting for the mutex
			next_tcb=mutex_ref->pSleep_queue;
			
			mutex_ref->pSleep_queue=(mutex_ref->pSleep_queue)->sleep_queue;

			mutex_ref->pHolding_Tcb=next_tcb;
			next_tcb->sleep_queue=NULL;
			mutex_ref->bLock=1;
			runqueue_add(next_tcb,next_tcb->cur_prio);

		}
		disable_interrupts();
		dispatch_save();
	}

	
	return 0;	

}



void add_to_mutex_sleep(mutex_t *mut,tcb_t *current_tcb)
{
	
        tcb_t *prev_tcb = NULL;
        tcb_t *cur_tcb;
        
        if(mut->pSleep_queue == NULL) {
                mut->pSleep_queue = current_tcb;        
                current_tcb->sleep_queue = NULL;
                return;
        }

        cur_tcb = mut->pSleep_queue;

        while(cur_tcb != NULL) {
                prev_tcb = cur_tcb;
                cur_tcb = cur_tcb->sleep_queue;
        }

        prev_tcb->sleep_queue = current_tcb;
        current_tcb->sleep_queue = NULL;
}

