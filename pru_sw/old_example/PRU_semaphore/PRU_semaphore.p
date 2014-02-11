//=========================================================
// File: PRU_semaphore.p
// Desc: Co-ordinated PRU and DSP access of L2 memory
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=========================================================
// Description:
//
//
//
//=========================================================
  .origin 0
  .entrypoint SEMAPHORE

#include "PRU_semaphore.hp"

SEMAPHORE:
    // Configure the programmable pointer register by setting c28_pointer[15:0] field to 0x8100
    // 28th entry in the constant table will point to base address 0x11810000
    MOV32     r0, 0x00008100
    MOV       r1, CTPPR_0
    ST32      r0, r1

    // Configure the programmable pointer register by setting c30_pointer[15:0] field to 0x0000
    // 30th entry in the constant table will point to base address 0x80000000 of the L3 (Shared RAM)
    MOV32     r0, 0x00000000
    MOV       r1, CTPPR_1
    ST32      r0, r1

// Read values of flags, readSize and source and destination offsets
// from the start of L3 Shared RAM
INITIAL:
    // Load the PSC CTL parameters
    LBCO      semParams, CONST_L3RAM, #0x00, SIZE(SemParams)
    MOV       global.tempSize, semParams.readSize
    QBNE      INITIAL, semParams.semFlag, 1

//Read the memory one byte at a time
COPYLOOP:
    QBEQ      HANDSHAKE, semParams.readSize, 0
    LBCO      global.data, CONST_DSPL2, semParams.srcOffset, 1    // read a byte
    ADD       semParams.srcOffset, semParams.srcOffset, 1
    SBCO      global.data, CONST_DDR, semParams.dstOffset, 1      // write a byte
    ADD       semParams.dstOffset, semParams.dstOffset, 1
    SUB       semParams.readSize, semParams.readSize, 1
    JMP       COPYLOOP

// Set the Read size , Source and destination location for the DSP
// memory read to follow
HANDSHAKE:
    ADD       semParams.readSize, global.tempSize, READSIZEINCR
    MOV       semParams.semFlag, 0
    SBCO      semParams, CONST_L3RAM, #0x00, SIZE(SemParams)

END:
    HALT

