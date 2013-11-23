/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
	.file "mutex_create.S"

FUNC(mutex_create)
	    
	swi MUTEX_CREATE
        cmp r0, #0
        bge .L2

.L1:
        ldr r1, =errno  @ Get the correct error code
        mov r2 , #0
        sub r3, r2, r0
        str r3, [r1]    @ Set the absolute value of the errno
        mov r0, #-1     @ Exit with -1
.L2:
        mov pc, lr


        
