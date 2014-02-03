//=============================================================================
// File: PRU_memAccessL2.p
// Desc: Accessing L2 memory from the PRU
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
//  PRU Example to access L2 Memory
//  The code stores three 32 bit values into the L2 memory
//
//  The code is specifically designed to work on PRU0
//  in order to replicate the code on PRU1 the control registers
//  for PRU1 must be similarly configured
//=============================================================================

.origin 0
.entrypoint MEMACCESS_L2

#include "PRU_memAccessL2.hp"

MEMACCESS_L2:
    // Configure the programmable pointer register for PRU0 by setting c28_pointer[15:0] field to 0x8100
    // to access DSP L2 memory @0x11810000
    MOV32     r0, 0x00008100
    MOV       r1, CTPPR_0
    ST32      r0, r1

    // Configure the programmable pointer register for PRU1 by setting c28_pointer[15:0] field to 0x8100
    // to access DSP L2 memory @0x11810000
    MOV       r1, CTPPR_0+0x800
    ST32      r0, r1

    // Load values to be stored in the L2 memory
    MOV32     r0, 0x52737829
    MOV32     r1, 0x12341234
    MOV32     r2, 0xAAAA0210

    // Store the three values into L2 memory locations
    SBCO      r0, CONST_DSPL2, 12, 12

    HALT

//===================================================================================
// End of file PRU_memAccessL2.p
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//===================================================================================