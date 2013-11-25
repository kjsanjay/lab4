@ 18-342 Lab3
@ Group-34
@ Sanjay K Jayasankar - sjayasan
@ Shobhit Kukreti - skukreti
@ Yashwant Vemuru - yvemuru
@ Date: Nov 10, 2013


@Setup stack top
@Change mode to user

	
#include <kernel_consts.h>

	

	.file "switchToUserMode.S"
	.text
	.align	2
		
	.global globalsp_svc


	.global switchToUserMode
	

switchToUserMode:
	@ In SVC mode
	@ save lr in r3
	@ lr is return value of function
	@ returns to line in main after the function

	
		@Store sp
	LDR r7,=globalsp_svc
	STR sp,[r7]
		@Switching to User mode
		@ enable IRQ & disable FIQ 
	mrs r3,CPSR
	BIC r3,r3,#0x2F
	ORR r3,r3,#0xD0
	BIC r3,r3,#0x80	   @enable IRQ
	MSR CPSR_cxsf,r3

	ldr sp,=USER_STACK_TOP	@Setup user stack
		@ n+1=> including NULL
	MOV r5,r0
		@r5 = argc => will be used as counter variable
	ADD	r4,r1,r0,LSL #2
		@r4 is at argv[argc]=0x0
	cmp	r5, #0
		@pushing argv
loop1:
	ldr	r3, [r4], #-4
	@ r3 <= mem(r4)
	@ r4 <= r4+4
	str	r3, [sp, #-4]!
	@ sp=sp-4
	@ mem(sp)=r3
	subs r5,r5,#1
	cmp r5,#0
	bge loop1
	str	r0, [sp, #-4]!	@pushing argc
	
	ldr pc, =USER_APP_LOCATION
	@ BX r7

globalsp_svc:
	.word globalsp_svc

