//=============================================================================
// File: PRU_memAccessL3andDDR.p
// Desc: Use of constant tables to access memory and registers associated with peripherals
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
// PRU Example to access  L3 and DDR Memory
// The code reads three 32 bit values from the DDR memory
// and writes it into the L3 memory interface
//
// Initialization :
// the CCS project places three values into the DDR RAM
//=============================================================================

.origin 0
.entrypoint MEMACCESSL3ANDDDR

#include "PRU_memAccessL3andDDR.hp"

MEMACCESSL3ANDDDR:
    // Configure the programmable pointer register for PRU0 by setting c30_pointer[15:0] and
    // c31_pointer[15..0] field to 0x0020 and 0x0010, respectively.  This will make C30 point
    // to 0x80002000 (L3) and C31 point to 0xC0001000 (DDR memory).
    MOV32     r0, 0x00100020
    MOV       r1, CTPPR_1
    ST32      r0, r1

    //Load values from external DDR Memory into Registers R0/R1/R2
    LBCO      r0, CONST_DDR, 0, 12

    //Store values from read from the DDR memory into L3 memory
    SBCO      r0, CONST_L3RAM, 0, 12

    HALT

//===================================================================================
// End of file PRU_memAccessL3andDDR.p
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//===================================================================================


