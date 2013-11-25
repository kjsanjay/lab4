/*

18-342 Lab3
Group-34
Sanjay K Jayasankar - sjayasan
Shobhit Kukreti - skukreti
Yashwant Vemuru - yvemuru
Date: Nov 10, 2013
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


void irq_handler()
{
	unsigned long osmr_ms;
	unsigned long new_osmr;
	tcb_t *highest_prio_tcb;
	tcb_t *current_tcb=get_cur_tcb();
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


        // defined in device.c
		dev_update(kernel_up_time*OS_TIMER_RESOLUTION); // sending default timer ticks into specified timer res
		highest_prio_tcb=_get_runList_tcb(highest_prio());
		if(current_tcb != highest_prio_tcb)
		{
			dispatch_save();

		}


	}

}


