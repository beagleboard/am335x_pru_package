//=============================================================================
// File: PRU_edmaConfig.p
// Desc: PRU configuring EDMA for data transfer
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
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
.entrypoint EDMA_CONFIG

#include "PRU_edmaConfig.hp"

//
// Example showing configuration of the EDMA to transfer
// a few bytes of data.  PaRAM is setup for self-chaining.
//

EDMA_CONFIG:
    // Init the register pointer
    MOV32     global.regPointer, EDMA3CC_0_REGS+0x1000

    // Clear channel event from EDMA event registers
    LDI	      global.regVal.w0, 1 << EDMA3CC_0_GP0_EVT
    SBBO      global.regVal, global.regPointer, 0x08, 2   // ECR
    SBBO      global.regVal, global.regPointer, 0x40, 2	  // SECR
    SBBO      global.regVal, global.regPointer, 0x70, 2	  // ICR

    // Enable channel interrupt
    SBBO      global.regVal, global.regPointer, 0x60, 2	  // IESR

    // Clear event missed register
    LDI       global.regPointer.w0, 0x0300
    SBBO      global.regVal, global.regPointer, 0x08, 2	  // EMCR

    // Setup EDMA region access
    LBBO      global.regVal, global.regPointer, 0x48, 4
    SET       global.regVal, global.regVal, EDMA3CC_0_GP0_EVT
    SBBO      global.regVal, global.regPointer, 0x48, 4	  // DMA region 1 access enable register

    // Setup channel to submit to EDMA Q0/TC0
    LDI       global.regPointer.w0, 0x0240                // DMAQNUM0
    LD32      global.regVal, global.regPointer
    AND	      global.regVal.b3,	global.regVal.b3,	0x00    // EVT6 and EVT7
    ST32      global.regVal, global.regPointer

    // Enable event
    LDI       global.regPointer.w0,	0x1030  //EESR
    LDI       global.regVal.w0,	1 << EDMA3CC_0_GP0_EVT
    ST16      global.regVal, global.regPointer

    // Setup PaRAM for transfer
    LDI       r15,(EDMA3CC_0_GP0_EVT << 12)
    OR        r15.w2,r15.w2,0x0090
    MOV       param.opt, r15 //0x00900000 | r15 // (EDMA3CC_0_GP0_EVT << 12)
    LBCO      param.src, CONST_PRUDRAM, 0, 4
    MOV       param.acnt, ACNT
    MOV       param.bcnt, BCNT
    LBCO      param.dst, CONST_PRUDRAM, 4, 4
    MOV       param.srcbidx, 0x0001
    MOV       param.dstbidx, 0x0001
    MOV       param.link, 0xFFFF
    MOV       param.bcntrld, 0x0000
    MOV       param.srccidx, 0x0000
    MOV       param.dstcidx, 0x0000
    MOV       param.ccnt, CCNT

    // Store PaRAM data
    MOV32     global.regPointer, EDMA3CC_0_PARAM + (EDMA3CC_0_GP0_EVT * 0x20)
    SBBO      param, global.regPointer,	0x00, SIZE(param)

    // Trigger transfer manually
    LDI       global.regPointer.w0, 0x1010    // ESR
    LDI       global.regVal.w0,	1 << EDMA3CC_0_GP0_EVT
    ST32      global.regVal, global.regPointer

    HALT