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

#include <kernel_consts.h>


extern volatile unsigned long kernel_up_time;
extern unsigned int TIMER_SLICE;

void irq_handler()
{
	unsigned long osmr_ms;
	unsigned long new_osmr;
	//Checking if OSMR0 - is matched - 26th bit
	unsigned int os_icpr=reg_read(INT_ICIP_ADDR);
	//Check bit for interrupt from correct source
	os_icpr=(os_icpr >> INT_OSTMR_0)  & BIT_0;

	if(os_icpr) //timer-0 interrupt
	{
		//Reset timer
		osmr_ms=OSTMR_FREQ/TIMER_SLICE;
		new_osmr=reg_read(OSTMR_OSMR_ADDR(0))+osmr_ms;
		reg_write(OSTMR_OSMR_ADDR(0),new_osmr);
		kernel_up_time++;
		//Clear bit in status register
		reg_set(OSTMR_OSSR_ADDR,OSTMR_OSSR_M0);


	}

}
