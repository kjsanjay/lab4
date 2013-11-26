/*

18-342 Lab4
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 25, 2013
 */

/*Implementation of custom IRQ Handler*/

#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <task.h>
#include <lock.h>
#include <sched.h>

#include <kernel_consts.h>

#include <device.h>
#include <config.h>
// We need to call dev_update in device.c with fresh time tick each 
// time timer isr is called.

extern volatile unsigned long kernel_up_time;

// #define DEBUG

void irq_handler()
{
	unsigned long osmr_ms;
	unsigned long new_osmr;
	
	#ifdef DEBUG
	puts("IRQH");

	#endif
	
	//Checking if OSMR0 - is matched - 26th bit
	unsigned int os_icpr=reg_read(INT_ICIP_ADDR);
	//Check bit for interrupt from correct source
	os_icpr=(os_icpr >> INT_OSTMR_0)  & BIT_0;

	if(os_icpr) //timer-0 interrupt
	{
		//Reset timer
		osmr_ms=OSTMR_FREQ/OS_TICKS_PER_SEC;
		new_osmr=reg_read(OSTMR_OSMR_ADDR(0))+osmr_ms;
		reg_write(OSTMR_OSMR_ADDR(0),new_osmr);
		kernel_up_time++;
        //Clear bit in status register
		reg_set(OSTMR_OSSR_ADDR,OSTMR_OSSR_M0);
		#ifdef DEBUG
		puts("IRQT");

		#endif

        
		// sending default timer ticks into specified timer res
		   dev_update(kernel_up_time*OS_TIMER_RESOLUTION); 
		


	}

}


