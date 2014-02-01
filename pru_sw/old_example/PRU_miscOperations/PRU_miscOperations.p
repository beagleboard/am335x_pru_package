//=============================================================================
// File: PRU_miscOperations.p
// Desc: PRU Example to miscellaneous operations
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
//  PRU Example to miscellaneous operations like use of masks to extract bits,
//  bubble sorting,thresholding
//
//  This example illustrates
//
//  (a)extraction of 4bit values of a 32 bit number using masks
//  and bit shift operations. In the example a 32 bit value is placed in source location
//  in L3 memory. The PRU reads the source data and extracts 8 four bit datas of a 32 bit number
//  and stores the extracted 4 bits(byte rounded) in PRU1 Data memory.
//
//  (b) The array of extracted bits is used as the initial array to demonstrate
//  implementation of bubble sort algorithm and the result of the algorithm is stored
//  in the DDR memory.
//
//  (c) The sorted array is then thresholded by applying a cut-off value and converted to
//  an array of zeros and ones. The subsequent thresholded result is stored in PRU0 Data memory
//=============================================================================

.origin 0
.entrypoint MISCOPERATIONS
#include "PRU_miscOperations.hp"

MISCOPERATIONS:
    // (A) Mask and bit shift operations to extract bits of a 32 bit number
    LDI       offset, #00
    LDI       Shift_reg, #00
    LDI       cnt, #00
    MOV32     max_shift_mask_reg, max_shift_mask

    //Load data value from memory
    LBCO      data_str_reg, CONST_L3RAM, 0, 4

    //Initialize MASK
    LDI       MASK_reg, Mask_4bit_extract

    //Extract 4 bit data of the 32bit data value
EXTRACT:
    MOV       temp, data_str_reg
    AND       temp, temp, MASK_reg
    LSR       temp, temp, Shift_reg
    SBCO      temp, CONST_PRU1DRAM, offset, 1               // Store extraction result in PRU1 Data memory
    SBCO      temp, CONST_DDR, offset, 1                    // Inititialize array to perform bubble sort in DDR memory
    ADD       offset, offset, 1
    QBEQ      EXIT_EXTRACT, MASK_reg, max_shift_mask_reg
    LSL       MASK_reg, MASK_reg, Shift
    ADD       Shift_reg, Shift_reg, Shift
    JMP       EXTRACT

    //End of Extract Functionality
EXIT_EXTRACT:
    LDI       offset, #00

    // (B) Bubble sort extracted values
START_SORT:
    LBCO      r0.b0, CONST_DDR, offset, 2
    QBLT      SORT, r0.b0, r0.b1
    JMP       DONT_SORT

SORT:
    MOV       temp_sort, r0.b0
    MOV       r0.b0, r0.b1
    MOV       r0.b1, temp_sort
    SBCO      r0.b0, CONST_DDR, offset, 2

DONT_SORT:
    ADD       offset, offset, 1
    QBGT      START_SORT, offset, N_extracts-1

LOOP:
    ADD       cnt, cnt, 1
    QBGT      EXIT_EXTRACT, cnt, N_extracts
    LDI       offset, #00


    // (C) Thresholding the sorted  values
THRESHOLD:
    QBLE      END, offset, N_extracts
    LBCO      r0.b0, CONST_DDR, offset, 1
    QBGT      LOW, r0.b0, threshold

HIGH:
    LDI       r0.b0, #01
    SBCO      r0.b0, CONST_PRU0DRAM, offset, 1
    ADD       offset, offset, 1
    JMP       THRESHOLD

LOW:
    LDI       r0.b0, #00
    SBCO      r0.b0, CONST_PRU0DRAM, offset, 1
    ADD       offset, offset, 1
    JMP       THRESHOLD

END:
    HALT

