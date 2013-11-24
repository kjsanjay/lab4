/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"
#include <kernel_consts.h>



static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number3 of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{

    int i;
    for(i=0;i<OS_MAX_TASKS;i++)
    {
        run_list[i]=0;
    }

    for(i=0;i<OS_MAX_TASKS/8;i++)
    {
        run_bits[i]=0;
    }
    group_run_bits = 0;
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{

 	uint8_t ostcby,ostcbx;

    ostcby = prio >> 3; 
	ostcbx = prio & 0x07;

	// Sets the group
	group_run_bits |= (0x1 << ostcby);
	 // Sets the corresponding lines that were set in group
	run_bits[ostcby] |= (0x1 << ostcbx);
	run_list[prio] = tcb;

}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{

    uint8_t ostcby,ostcbx;
    tcb_t *ret_tcb;
    

    ostcby = prio >> 3; 
    ostcbx = prio & 0x07;


    if(run_bits[ostcby] == 0)
    { // Have to do this so that group run bits are not modified when there is 
        //something else in that group.
        group_run_bits &= ~(0x1 << ostcby);
    }
    run_bits[ostcby] &= ~(0x1 << ostcbx);
    
    ret_tcb = run_list[prio];

    run_list[prio] = NULL;

	return (ret_tcb); // fix this; dummy return to prevent warning messages	
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{

    uint8_t y,x,prio;
    y = prio_unmap_table[group_run_bits];
    x = prio_unmap_table[run_bits[y]];
    prio = (y << 3) + x;
    return prio;
}


void print_run_queue()
{
        int i;
        printf("group_run_bits = %x\n", group_run_bits);
        printf("Run List\n");
        for(i = 0; i < OS_MAX_TASKS; i++) {
                printf("run_list[%d] = %p\n", i, run_list[i]);
        }
        printf("Run Bits\n");
        for(i = 0; i < OS_MAX_TASKS/8; i++) {
                printf("run_bits[%d] = %x\n", i, run_bits[i]);
        }
        
}

tcb_t* _get_runList_tcb(int prio)
{
    return (tcb_t *)run_list[prio];



}