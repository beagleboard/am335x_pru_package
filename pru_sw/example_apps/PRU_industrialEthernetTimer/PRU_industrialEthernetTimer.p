/* @file PRU_industrialEthernetTimer.h
 * @brief A am335x PRU routine to test the Industrial Ethernet Timer (32-bit)
 *
 * @author Igor Borges Tavares (igorborgest@gmail.com)
 * @bug No know bugs.
 */

.origin 0
.entrypoint start

#include "PRU_industrialEthernetTimer.hp"

start:
	// Copy the TICKS value from PRU shared memory first addr to r0
	lbco    r0, CONST_PRUSHAREDRAM, 0, 4

	// timer configuration
	// CMP_INC     = 1
	// DEFAULT_INC = 1
	// CNT_ENABLE  = 1
	mov  r1, 0x0111
	sbco r1, CONST_IEP, 0x0, 2

	lbco r1, CONST_IEP, IEP_TIMER_COUNT, 4 //read the timer counter

WAIT_LOOP:
	lbco r2, CONST_IEP, IEP_TIMER_COUNT, 4 // read the timer counter
	sub  r2, r2,  r1 // r2 = r2 - r1
	qblt WAIT_LOOP, r0, R2 //if(r2 < r3) branch;

END:
	mov r31.b0, PRU1_ARM_INTERRUPT+16
	halt
