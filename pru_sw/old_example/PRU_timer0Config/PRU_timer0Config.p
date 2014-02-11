//=============================================================================
// File: PRU_timer0Config.p
// Desc: PRU example to configure the TIMER0 peripheral
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
// Example showing configuration of Timer0 in 32-bit unchained
// mode by PRU.
//=============================================================================

.origin 0
.entrypoint TIMER0CONFIG

#include "PRU_timer0Config.hp"


// ***************************************
// *       Local Macro definitions       *
// ***************************************


TIMER0CONFIG:
    // Init the register pointer
    MOV32     regPointer, TMR_0_REGS

    // Place Timer0 in reset
    MOV32     regVal, 0x00000000
    SBBO      regVal, regPointer, TMR_TGCR_OFFSET,  2

    // Reset the Timer0 counter registers
    SBBO      regVal, regPointer, TMR_TIM12_OFFSET,  4

    // Setup the TCR register
    SBBO      regVal, regPointer, TMR_TCR_OFFSET,  2

    // Setting the Timer0 global control register
    LDI       regVal.w0, 0x0015
    SBBO      regVal, regPointer, TMR_TGCR_OFFSET,  2

    // Load the time constant into the period register
    LDI       regVal.w0, TIME_CONST
    SBBO      regVal, regPointer, TMR_PRD12_OFFSET,  4

    // Clear any pending interrupts
    MOV32     regVal, 0x000A000A
    SBBO      regVal, regPointer, TMR_INTCTL_OFFSET,  4

    // Enable the Timer0 1:2 interrupt
    LDI       regVal.w0, 0x0001
    SBBO      regVal, regPointer, TMR_INTCTL_OFFSET,  2

    // Enable the Timer0 in one-shot mode
    LDI       regVal.w0, 0x0040
    SBBO      regVal, regPointer, TMR_TCR_OFFSET,  2

    HALT
