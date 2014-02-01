//=============================================================================
// File: PRU_PRU1toPRU0_Interrupt.p
// Desc: PRU example to show PRU to PRU interrupts
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================

.origin 0
.entrypoint PRU1_TO_PRU0_INTERRUPT

#include "PRU_PRUtoPRU_Interrupt.hp"


// ***************************************
// *       Local Macro definitions       *
// ***************************************
#define SYS_EVT         33
#define SYS_EVT_PRU0    32
#define HOST_NUM        1
#define CHN_NUM         1


PRU1_TO_PRU0_INTERRUPT:
POLL:
    // Poll for receipt of interrupt on host 0
    QBBS      FUNC, eventStatus, 30
    JMP       POLL

FUNC:
    // Clear the status of the interrupt
    LDI	regVal.w2,	0x0000
    LDI	regVal.w0,	SYS_EVT_PRU0
    SBCO	regVal,	CONST_PRUSSINTC,	0x24,        4

    MOV32     regVal, 0x0A
    SBCO      regVal, CONST_DDR, 0x00, 4

    // Initialize pointer to INTC registers
    MOV32     regOffset, 0x00000000

    //Clear SYS_EVT32
    MOV32     r31, #00

    // Global enable the all host interrupts
    LDI       regVal.w0, 0x0001
    SBCO      regVal, CONST_PRUSSINTC, GER_OFFSET, 2

    // Enable host interrupt 1
    LDI       regVal.w2, 0x0000
    LDI       regVal.w0, HOST_NUM
    SBCO      regVal,  CONST_PRUSSINTC, HIESR_OFFSET,        4

    // Map channel 0 to host 1
    LDI       regOffset.w0, INTC_HOSTMAP_REGS_OFFSET
    ADD       regOffset.w0, regOffset.w0, HOST_NUM
    LDI       regVal.w0, CHN_NUM
    SBCO      regVal, CONST_PRUSSINTC, regOffset.w0,   1

    // Map SYS_EVT33 interrupt to channel 1
    LDI       regOffset.w0, INTC_CHNMAP_REGS_OFFSET
    ADD       regOffset, regOffset, SYS_EVT
    LDI       regVal.b0, CHN_NUM
    SBCO      regVal.b0, CONST_PRUSSINTC, regOffset.w0,   1

    // Make sure the SYS_EVT33 system interrupt is cleared
    MOV32     regVal, SYS_EVT
    SBCO      regVal, CONST_PRUSSINTC, SICR_OFFSET, 4

    // Enable SYS_EVT33 system interrupt
    SBCO      regVal, CONST_PRUSSINTC, EISR_OFFSET, 4

    //Generate SYS_EVT32
    MOV32     r31, SYS_EVT

DONE:
    HALT
