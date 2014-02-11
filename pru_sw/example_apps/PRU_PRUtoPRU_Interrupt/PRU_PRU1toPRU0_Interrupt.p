// *
// * PRU_PRU1toPRU0_Interrupt.p
// *
// * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
// *
// *
// *  Redistribution and use in source and binary forms, with or without
// *  modification, are permitted provided that the following conditions
// *  are met:
// *
// *    Redistributions of source code must retain the above copyright
// *    notice, this list of conditions and the following disclaimer.
// *
// *    Redistributions in binary form must reproduce the above copyright
// *    notice, this list of conditions and the following disclaimer in the
// *    documentation and/or other materials provided with the
// *    distribution.
// *
// *    Neither the name of Texas Instruments Incorporated nor the names of
// *    its contributors may be used to endorse or promote products derived
// *    from this software without specific prior written permission.
// *
// *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// *
// *

// *
// * ============================================================================
// * Copyright (c) Texas Instruments Inc 2010-12
// *
// * Use of this software is controlled by the terms and conditions found in the
// * license agreement under which this software has been supplied or provided.
// * ============================================================================
// *

// *****************************************************************************/
// file:   PRU_PRU1toPRU0_Interrupt.p
//
// brief:  PRU example to show PRU to PRU interrupts.
//
//
//  (C) Copyright 2012, Texas Instruments, Inc
//
//  author     M. Watkins
//
//  version    0.1     Created
// *****************************************************************************/

.origin 0
.entrypoint PRU1_TO_PRU0_INTERRUPT

#include "PRU_PRUtoPRU_Interrupt.hp"


// ***************************************
// *       Local Macro definitions       *
// ***************************************

#define SYS_EVT         PRU1_PRU0_INTERRUPT
#define SYS_EVT_PRU0    PRU0_PRU1_INTERRUPT

PRU1_TO_PRU0_INTERRUPT:
POLL:
    // Poll for receipt of interrupt on host 0
    WBS       eventStatus, 31

FUNC:
    // Clear the status of the interrupt
    LDI	        regVal.w2,	0x0000
    LDI	        regVal.w0,	SYS_EVT_PRU0
    SBCO	regVal,	CONST_PRUSSINTC,	SICR_OFFSET,        4

    // Configure the programmable pointer register for PRU1 by setting c31_pointer[15:0]
    // field to 0x0000.  This will make C31 point to offset 0x0000 of the DDR memory.

    MOV       r0, 0x00000000
    MOV       r1, CTPPR_1
    ST32      r0, r1

    MOV       regVal, 0x0A
    SBCO      regVal, CONST_DDR, 0x00, 4

    // Initialize pointer to INTC registers
    MOV       regOffset, 0x00000000

    //Generate SYS_EVT
#ifdef AM33XX
    MOV       r31.b0, SYS_EVT+16
#else
    MOV       r31, SYS_EVT
#endif

DONE:
    // Send notification to Host for program completion
#ifdef AM33XX
    MOV       r31.b0, PRU1_ARM_INTERRUPT+16
#else
    MOV       r31.b0, #PRU1_ARM_INTERRUPT
#endif

    HALT
