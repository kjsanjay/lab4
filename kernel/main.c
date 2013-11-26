/** @file main.c
 *
 * @brief kernel main
 *
 * 18-342 Lab4
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 25, 2013
 // */
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <arm/reg.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/exception.h>
#include <kernel_consts.h>
#include <lock.h>

void init_IRQregs();
void init_timer();


extern int new_SWI_Handler();
extern int irq_wrapper();
extern inline int switchToUserMode(int,char**);

inline void restore_OldSWI(void);
inline void restore_OldIRQ(void);

//Global variables

//Stores old SWI & IRQ handler instructions
unsigned int old_SWIinstr1 = 0;
unsigned int old_SWIinstr2 = 0;
unsigned int old_IRQinstr1=0;
unsigned int old_IRQinstr2=0;
unsigned int *UBOOT_SWI_CALC;
unsigned int *UBOOT_IRQ_CALC;
// 
//Stores return value to uboot
unsigned int k_r0;

//COunts number times the timer generated interrupt
volatile unsigned long kernel_up_time;


/* Support code -Start*/
uint32_t global_data;
/* Support code -End */

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	/* Support code -Start*/
	app_startup(); // bss is valid after this point 
	global_data = table;
	/* Support code -End */


	volatile unsigned int*writeLoc;
	volatile unsigned int jump_no;
	//Wire in SWI Handler
	//Save U-Boot's SWI Handler instructions
	writeLoc=(unsigned int*)SWI_VTABLE_ADDR;
	jump_no=*writeLoc;
	jump_no= (jump_no & OFFSET_12); 
	writeLoc =(unsigned int*) (SWI_VTABLE_ADDR + PC_OFFSET + jump_no);
	jump_no=*writeLoc;
	writeLoc=(unsigned int*)jump_no;
	UBOOT_SWI_CALC=(unsigned int*)writeLoc;
	old_SWIinstr1=*writeLoc;
	//Location of UBoot's SWI
	*writeLoc=(unsigned int)EXEC_NXT_INSTR; //ldr pc,[pc,#-4]
	writeLoc++;
	old_SWIinstr2=*writeLoc;
	*writeLoc=(unsigned int)new_SWI_Handler;
	
	//Wire in IRQ Handler
	//Save U-Boot's IRQ Handler instructions
	writeLoc=(unsigned int*)IRQ_VTABLE_ADDR;
	jump_no=*writeLoc;
	jump_no= (jump_no & OFFSET_12); 
	writeLoc =(unsigned int*) (IRQ_VTABLE_ADDR + PC_OFFSET + jump_no);
	jump_no=*writeLoc;
	writeLoc=(unsigned int*)jump_no;
	UBOOT_IRQ_CALC=(unsigned int*)writeLoc;
	old_IRQinstr1=*writeLoc;
	//Location of UBoot's IRQ
	*writeLoc=(unsigned int)EXEC_NXT_INSTR; //ldr pc,[pc,#-4]
	writeLoc++;
	old_IRQinstr2=*writeLoc;
	*writeLoc=(unsigned int)irq_wrapper;


    init_IRQregs();
	enable_interrupts();

	init_timer();
	mutex_init();
	dev_init();

	// Switch to user mode & load user program
	k_r0=switchToUserMode(argc,argv);


	disable_interrupts();
	//Restore SWI & IRQ handler 
	restore_OldSWI();
	restore_OldIRQ();

	printf("Ret=%d\n",k_r0 );
	return k_r0;
		
	// assert(0);        /* should never get here */
}

/* void restore_OldSWI(void)
	restores UBoot's original SWI handler instructions

*/

inline void restore_OldSWI(void)
{

	unsigned int *loc=0;
	loc = (unsigned int*)UBOOT_SWI_CALC;
	*loc =  old_SWIinstr1;
	*(loc+1)=  old_SWIinstr2;

}

/* void restore_OldIRQ(void)
	restores UBoot's original IRQ handler instructions

*/

inline void restore_OldIRQ(void)
{                                                               
                                                                
    unsigned int *loc=0;                                    
    loc = (unsigned int*)UBOOT_IRQ_CALC;                          
	*loc =  old_IRQinstr1;                                  
    *(loc+1)=  old_IRQinstr2;                               
                                                                
}           

/* void init_IRQregs()
	Initialize the interupt register for a timer

*/
void init_IRQregs()
{
	//Set 26th bit to one
	reg_write(INT_ICMR_ADDR,1<<INT_OSTMR_0);
	//
	reg_clear(INT_ICLR_ADDR,1<<INT_OSTMR_0);

	

}
/* void init_timer()
	Reset timer counting,
	Enable timer interruption
	Set the interrupt frequency

*/

void init_timer()
{
	
	kernel_up_time=0;
	int osmr_ms=OSTMR_FREQ/OS_TICKS_PER_SEC;
	reg_write(OSTMR_OSCR_ADDR,0x0);
	
	reg_write(OSTMR_OSMR_ADDR(0),osmr_ms);
	
	reg_set(OSTMR_OIER_ADDR,OSTMR_OIER_E0);

}
