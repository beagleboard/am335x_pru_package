//=============================================================================
// File: PRU_multiply.p
// Desc: Simple 16bx16b=32b multiply using macro.
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
//  Example use of the common multiply macro.
//=============================================================================


// The MULTIPLY service routine facilitates simple 1D:1D transfer.
// Parameter Table:
//
// 31        24        16        8        0
//  ______________________________________
// |                SRC1                  |
// |______________________________________|
// |                SRC2                  |
// |______________________________________|
// |               RESULT                 |
// |______________________________________|
//

.origin 0
.entrypoint MULTIPLY

#include "PRU_multiply.hp"

MULTIPLY:
    // Load the MEM1D parameters in registers R7:R9
    LBCO      mpyParams, CONST_PRUDRAM, #0x00, SIZE(MultiplyParams)
//    MOV32     mpyParams.src1, 0x000001AA
//    MOV32     mpyParams.src2, 0x00000100
    MPY       mpyParams.result, mpyParams.src1, mpyParams.src2, 16

    // Store the current state back to PARAM
    SBCO      mpyParams.result, CONST_PRUDRAM, OFFSET(mpyParams.result), SIZE(mpyParams.result)

MULTIPLY_EXIT:
    HALT


