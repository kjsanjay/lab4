@ 18-342 Lab3
@ Group-34
@ Sanjay K Jayasankar - sjayasan
@ Shobhit Kukreti - skukreti
@ Yashwant Vemuru - yvemuru
@ Date: Nov 10, 2013

@ Implementation of SWI Handler

#include <kernel_consts.h>



    .file "swi_handler.S"
    .text
    .align  2
    .global new_SWI_Handler
    .type new_SWI_Handler, %function

@r0 - arg-1 /SWI Number/return from function
@r1 - arg-2/parameter array
@r2 - arg-3/spsr

@spsr is stored before 

new_SWI_Handler:
    
    
    
    STMFD sp,{sp,lr}^   @User mode sp,lr. to restore context - usage of ^
    SUB sp,sp,#8        @Makeing space required for user mode sp,lr
    SUB sp,sp,#4        @ Makin space for spsr
    STMFD sp!, {r0-r12,lr}  @Pushing all svc mode registers
    LDR r2,=global_data     @ Restore r8 in-case it has changed
    LDR r8,[r2]
    MRS r2,spsr @ the user mode cpsr
    STR r2, [sp, #14*4] @ push the cpsr_user into stack to pop after executing SWI
    MRS r2,cpsr         @ 
    bic r2,r2,#0x80 @ Enable IRQ
    MSR cpsr_cxsf,r2
    MOV r1,sp @ parameters of for c (r0,r1,r2)
    @Extract SWI Number
    LDR r0,[lr,#-4] 
    bic r0,r0,#CLR_SWI_NUM
    BL C_SWI_Handler
    LDR r2, [sp, #14*4] @Get spsr
    ORR r2,r2,#0xC0     @ Disable IRQ
    MSR spsr, r2
    ADD sp,sp,#4    @so that value of r0 is not clobbered
    LDMFD sp!, {r1-r12,lr}
    ADD sp,sp,#4
    LDMFD sp, {sp,lr}^
    ADD sp,sp,#8
    MOVS pc,lr

