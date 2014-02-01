//=============================================================================
// File: PRU_DSPtoPRU_Interrupt.p
// Desc: Accessing L2 memory from the PRU
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments, Incorporated
//  All rights reserved
//
//=============================================================================
//  PRU Example to demonstrate DSP interrupting PRU to export event handling
//
//  In this example code demonstrates Timer0 configured by the DSP.The
//  DSP enables the timer interrupt and waits for interrupt once the interrupt
//  is received,it exports the event handling to the PRU by setting a flag
//  that the PRU is polling. The PRU performs Event handling like reseting
//  the timer and performing a functionality and the DSP is informed of the
//  serviced interrupt by reseting the flag
//=============================================================================


.origin 0
.entrypoint DSP_TO_PRU_INTERRUPT

#include "PRU_DSPtoPRU_Interrupt.hp"


  // Initialize pointer to INTC registers
    MOV32     regOffset, 0x00000000
    // Clear SYS_EVT32
    MOV32     r31, 0x00000000

    // Global enable the all host interrupts
    LDI       regvalue.w0, 0x0001
    SBCO      regvalue, CONST_PRUSSINTC, GER_OFFSET, 2

    // Enable host interrupt 0
    MOV32     regvalue, (0x00000000 | HOST_NUM)
    SBCO      regvalue, CONST_PRUSSINTC, HIESR_OFFSET, 4

    // Map channel 0 to host 0
    LDI       regOffset.w0, INTC_HOSTMAP_REGS_OFFSET
    ADD       regOffset.w0, regOffset.w0, HOST_NUM
    LDI       regvalue.w0, CHN_NUM
    SBCO      regvalue, CONST_PRUSSINTC, regOffset.w0, 1

    // Map SYS_EVT32 interrupt to channel 0
    LDI       regOffset.w0, INTC_CHNMAP_REGS_OFFSET
    ADD       regOffset, regOffset, SYS_EVT
    LDI       regvalue.b0, CHN_NUM
    SBCO      regvalue.b0, CONST_PRUSSINTC, regOffset.w0, 1

    // Make sure the SYS_EVT32 system interrupt is cleared
    MOV32     regvalue, (0x00000000 | SYS_EVT)
    SBCO      regvalue, CONST_PRUSSINTC, SICR_OFFSET, 4

    // Enable SYS_EVT32 system interrupt
    SBCO      regvalue, CONST_PRUSSINTC, EISR_OFFSET, 4

    //Generate SYS_EVT32
    MOV32     r31, SYS_EVT


DSP_TO_PRU_INTERRUPT:
POLL:
    // Poll for receipt of interrupt on host 0
    QBBS      EVENT, eventStatus, 30
    JMP       POLL


EVENT:
    //Clear system event in SECR2
     MOV32  regvalue,0x00000001
     MOV32  r3,SECR2_OFFSET
     SBCO	regvalue,	CONST_PRUSSINTC,	r3,        4

    //Clear system event enable in ECR2
     MOV32  regvalue,0x00000001
     MOV32  r3,ECR2_OFFSET
     SBCO	regvalue,	CONST_PRUSSINTC,	r3,        4


    // Init the timer TGCR register pointer
    MOV32     global.regPointer, TMR_0_TGCR_REG

    // Place Timer0 in reset
    MOV	      global.regVal.w0, 0x0000
    ST16      global.regVal, global.regPointer

    // Functionality implemented in this code is simple decrement of the value of
    // SCORE stored in the DDR memory. To access the DDR memory, the programmable
    // pointer register is configured by setting C31[15:0] field. Set R0 to zero
    // and store that value into in CTPR_1 to configure c31_pointer[15:0].
    MOV32     r0, 0x00000000
    MOV       r1, CTPPR_1
    ST32      r0, r1

    // Load values from external DDR Memory into score register
    LBCO      global.score, CONST_DDR, 0, 4

    // Decrement value of score by 1
    SUB	      global.score, global.score, 1

    // Store values from read from the DDR memory into L3 memory
    SBCO      global.score, CONST_DDR, 0, 4

    HALT

.end
//===================================================================================
// End of file PRU_DSPtoPRU_Interrupt.p
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//===================================================================================