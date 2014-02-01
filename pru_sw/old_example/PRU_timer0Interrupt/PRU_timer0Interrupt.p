//=============================================================================
// File: PRU_timer0Interrupt.p
// Desc: PRU example to show interrupt processing from the TIMER0 interrupt
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
// Example showing the PRU waiting for an interrupt from Timer 0.
//=============================================================================


.origin 0
.entrypoint TIMER0INTERRUPT
#include "PRU_timer0Interrupt.hp"
//
// Macro definitions
//

TIMER0INTERRUPT:
    // Initialize pointer to INTC registers
    MOV32     regOffset, 0x00000000
    // Clear SYS_EVT
    MOV32     r31, 0x00000000

    // Global enable of all host interrupts
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

    // Map Timer 0 interrupt to channel 0
    LDI       regOffset.w0, INTC_CHNMAP_REGS_OFFSET
    ADD       regOffset, regOffset, SYS_EVT
    LDI       regVal.b0, CHN_NUM
    SBCO      regVal.b0, CONST_PRUSSINTC, regOffset.w0, 1

    // Set the polarity registers
    MOV32     regPointer, (INTC_REGS_BASE + INTC_SYS_INT_REGS_OFFSET)
    MOV32     regVal, 0xFFFFFFFF
    SBBO      regVal, regPointer, 0x00, 4
    SBBO      regVal, regPointer, 0x04, 4

    // Set the type registers
    MOV32     regVal, 0x1C000000
    SBBO      regVal, regPointer, 0x80, 4
    LDI       regVal.w2, #0x0000
    SBBO      regVal, regPointer, 0x84, 4

    // Make sure the Timer 0 system interrupt is cleared
    MOV32     regVal, (0x00000000 | SYS_EVT)
    SBCO      regVal, CONST_PRUSSINTC, SICR_OFFSET, 4

    // Enable Timer 0 system interrupt
    SBCO      regVal, CONST_PRUSSINTC, EISR_OFFSET, 4

CORE_LOOP:
    // Poll for receipt of interrupt on host 0
    QBBS      EVENT_HANDLER, eventStatus, #30
    JMP       CORE_LOOP

EVENT_HANDLER:
    // Initialize pointer to priority index register location in the INTC
    LDI       regOffset.w0, INTC_HOSTINTPRIO_REGS_OFFSET

    // Retrieve interrupt number from the INTC registers
    LBCO      jumpOffset, CONST_PRUSSINTC, regOffset.w0, 4
    AND       jumpOffset.b0, jumpOffset.b0, PT_MASK

    // Jump to the timer interrupt ISR
    QBEQ      TIMER64P0_EVENTOUT12, jumpOffset.b0, 3
    JMP       DONE

TIMER64P0_EVENTOUT12:
    // Clear the status of the timer interrupt
    MOV32     regVal, (0x00000000 | SYS_EVT)
    SBCO      regVal, CONST_PRUSSINTC, SICR_OFFSET, 4

DONE:
    MOV32     regVal, 0x00000000
    SBCO      regVal, CONST_PRUDRAM, 0x00, 4
    HALT
