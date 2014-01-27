// *
// * PRU_PRU0toPRU1_Interrupt.p
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
// file:   PRU_PRU0toPRU1_Interrupt.p
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
.entrypoint PRU0_TO_PRU1_INTERRUPT

#include "PRU_PRUtoPRU_Interrupt.hp"


// ***************************************
// *       Local Macro definitions       *
// ***************************************

#define SYS_EVT         PRU0_PRU1_INTERRUPT
#define SYS_EVT_PRU1    PRU1_PRU0_INTERRUPT


PRU0_TO_PRU1_INTERRUPT:

#ifdef AM33XX
    // Enable OCP master port
    LBCO      r0, CONST_PRUCFG, 4, 4
    CLR       r0, r0, 4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO      r0, CONST_PRUCFG, 4, 4
#endif

    //Generate SYS_EVT
#ifdef AM33XX
    MOV       r31.b0, SYS_EVT+16
#else
    MOV       r31, SYS_EVT
#endif

    // Poll for receipt of interrupt on host 1
POLL:
    WBS       eventStatus, #30

DONE:
    // Configure the programmable pointer register for PRU0 by setting c31_pointer[31:16]
    // field to 0x0000.  This will make C31 point to 0x80001000 (DDR memory).
    MOV       r0, 0x00000000
    MOV       r1, CTPPR_1
    ST32      r0, r1

    MOV       regVal, 0x0B
    SBCO      regVal, CONST_DDR, 0x04, 4

    // Clear the status of the interrupt
    LDI	      regVal.w2,	0x0000
    LDI	      regVal.w0,	SYS_EVT_PRU1
    SBCO      regVal,	CONST_PRUSSINTC,	SICR_OFFSET,        4

    // Send notification to Host for program completion
#ifdef AM33XX
    MOV       r31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV       R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
