//=============================================================================
// File: PRU_memAccessPRUDataRam.p
// Desc: PRU access of internal Data Ram
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
//  PRU Example to access PRU local data Memory
//
//  the following example illustrates PRU access of the local memory
//  using local address using the register pointed base address and
//  also illustrates use of global address pointed by entries in constant
//  table to access Local data RAM
//=============================================================================

.origin 0
.entrypoint MEMACCESSPRUDATARAM

#include "PRU_memAccessPRUDataRam.hp"


MEMACCESSPRUDATARAM:
    //Load 32 bit value in r1
    MOV32     r1, 0x0010f012

    //Load address of PRU data memory in r2
    MOV       r2, 0x0004

    // Move value from register to the PRU local data memory using registers
    ST32      r1,r2

    // Load 32 bit value into r3
    MOV32     r3, 0x0000567A

    LBCO      r4, CONST_PRUDRAM, 4, 4 //Load 4 bytes from memory location c3(PRU0/1 Local Data)+4 into r4 using constant table

    // Add r3 and r4
    ADD       r3, r3, r4

    //Store result in into memory location c3(PRU0/1 Local Data)+8 using constant table
    SBCO      r3, CONST_PRUDRAM, 8, 4
    HALT

//===================================================================================
// End of file PRU_memAccessPRUDataRam.p
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//===================================================================================


