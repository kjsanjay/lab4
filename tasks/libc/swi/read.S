/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>
	.file "read.S"

FUNC(read)

read:
        stmfd sp!, {r1-r3} @ Storing Banked registers
        swi READ_SWI
        cmp r0, #0
        bge .L2

.L1:
        ldr r1, =errno  @ Get the correct error code
        mov r2 , #0
        sub r3, r2, r0
        str r3, [r1]    @ Set the absolute value of the errno
        mov r0, #-1     @ Exit with -1
.L2:
        ldmfd sp!, {r1-r3}
        mov pc, lr

