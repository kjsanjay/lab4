/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
_start:
	ldr r0, [sp]	@ Get the argument argc
	add r1, sp, #4	@ Get argv from the next location on stack
_start_loop:
	bl main			@ Call main function
	b _start_loop
