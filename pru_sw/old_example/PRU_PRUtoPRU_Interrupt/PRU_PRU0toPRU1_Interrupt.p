//=============================================================================
// File: PRU_PRU0toPRU1_Interrupt.p
// Desc: PRU example to show PRU to PRU interrupts
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================

.origin 0
.entrypoint PRU0_TO_PRU1_INTERRUPT

#include "PRU_PRUtoPRU_Interrupt.hp"


// ***************************************
// *       Local Macro definitions       *
// ***************************************
#define SYS_EVT         32
#define SYS_EVT_PRU1    33
#define HOST_NUM        0
#define CHN_NUM         0


PRU0_TO_PRU1_INTERRUPT:
    // Initialize pointer to INTC registers
    MOV32     regOffset, 0x00000000
    // Clear SYS_EVT32
    MOV32     r31, 0x00000000

    // Global enable the all host interrupts
    LDI       regVal.w0, 0x0001
    SBCO      regVal, CONST_PRUSSINTC, GER_OFFSET, 2

    // Enable host interrupt 0
    MOV32     regVal, (0x00000000 | HOST_NUM)
    SBCO      regVal, CONST_PRUSSINTC, HIESR_OFFSET, 4

    // Map channel 0 to host 0
    LDI       regOffset.w0, INTC_HOSTMAP_REGS_OFFSET
    ADD       regOffset.w0, regOffset.w0, HOST_NUM
    LDI       regVal.w0, CHN_NUM
    SBCO      regVal, CONST_PRUSSINTC, regOffset.w0, 1

    // Map SYS_EVT32 interrupt to channel 0
    LDI       regOffset.w0, INTC_CHNMAP_REGS_OFFSET
    ADD       regOffset, regOffset, SYS_EVT
    LDI       regVal.b0, CHN_NUM
    SBCO      regVal.b0, CONST_PRUSSINTC, regOffset.w0, 1

    // Make sure the SYS_EVT32 system interrupt is cleared
    MOV32     regVal, (0x00000000 | SYS_EVT)
    SBCO      regVal, CONST_PRUSSINTC, SICR_OFFSET, 4

    // Enable SYS_EVT32 system interrupt
    SBCO      regVal, CONST_PRUSSINTC, EISR_OFFSET, 4

    //Generate SYS_EVT32
    MOV32     r31, SYS_EVT

    // Poll for receipt of interrupt on host 0
POLL:
    QBBS      DONE, eventStatus, #31
    JMP       POLL

DONE:
    MOV32     regVal, 0x0B
    SBCO      regVal, CONST_DDR, 0x04, 4

    // Clear the status of the interrupt
    LDI	regVal.w2,	0x0000
    LDI	regVal.w0,	SYS_EVT_PRU1
    SBCO	regVal,	CONST_PRUSSINTC,	0x24,        4

    HALT
