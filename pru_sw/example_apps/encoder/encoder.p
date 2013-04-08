// *
// * PRU_memAccessPRUDataRam.p
// *
// * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
// *
// *
// *  Redistribution and use in source and binary forms, with or without
// *  modification, are permitted provided that the following conditions
// *  are met:
// *
// *    Redistributions of source code must retain the above copyright
// *    notice, this list of conditions and the following disclaimer.
// *
// *    Redistributions in binary form must reproduce the above copyright
// *    notice, this list of conditions and the following disclaimer in the
// *    documentation and/or other materials provided with the
// *    distribution.
// *
// *    Neither the name of Texas Instruments Incorporated nor the names of
// *    its contributors may be used to endorse or promote products derived
// *    from this software without specific prior written permission.
// *
// *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// *
// *

// *
// * ============================================================================
// * Copyright (c) Texas Instruments Inc 2010-12
// *
// * Use of this software is controlled by the terms and conditions found in the
// * license agreement under which this software has been supplied or provided.
// * ============================================================================
// *

// *****************************************************************************/
// file:   PRU_memAccessPRUDataRam.p 
//
// brief:  PRU access of internal Data Ram.
//
//
//  (C) Copyright 2012, Texas Instruments, Inc
//
//  author     M. Watkins
//
//  version    0.1     Created
// *****************************************************************************/

.origin 0
.entrypoint MEMACCESSPRUDATARAM

#include "encoder.hp"

MEMACCESSPRUDATARAM:
      
#ifdef AM33XX

    // Configure the block index register for PRU0 by setting c24_blk_index[7:0] and
    // c25_blk_index[7:0] field to 0x00 and 0x00, respectively.  This will make C24 point
    // to 0x00000000 (PRU0 DRAM) and C25 point to 0x00002000 (PRU1 DRAM).
    MOV       r0, 0x00000000
    MOV       r1, CTBIR_0
    ST32      r0, r1

#endif
    //Register map:
	//r0: Memory loc of shared memory (where position will be written)
	//r1: encoder_A current*
	//r2: encoder_A prev*
	//r3: encoder_B current*
	//r4: encoder_A inverted*
	//r5: Is edge?*
	//r6: position1
	//r7: position2
	//r31: GPIO input register (readonly)
	//*These registers are reused from encoder to encoder
	
    //Load address of PRU data memory in r2
    MOV r0, 0x0004
    
    //Initialize the "previous" value for pinA of encoder (edge pin)
    MOV r2, 0
    
    //Initialize position
    MOV r6, 0
	ST_POS1 r6, r0 //Loads position into shared mem
	MOV r7, 0
	ST_POS2 r7, r0 //Loads position into shared mem
    
READPINS_ENCODER1:
    //Store pin1 current value
    LSR r1, r31, ENC1_A //ENC1_A defined in .hp
    AND r1, r1, 1
    
    //Store pin2 current value
    LSR r3, r31, ENC1_B //ENC1_B defined in .hp
    AND r3, r3, 1
    
    //Invert pin1 value and store in r4 to test logic later on...
    NOT r4, r1
	AND r4, r4, 1
    
    //Store boolean for if pin1 is experiencing an edge (high to low) in r5
    AND r5, r4, r2
    
    //Update previous value of ENC_A
    MOV r2, r1
    
    //Jump to edge detection steps if edge detected
    QBEQ EDGEDETECTED_ENCODER1, r5, 1
    
    //Loop forever
    QBA READPINS_ENCODER2
    
//Jump to CW handling if clockwise (B high while edge present).
//Otherwise handle CCW in this label
EDGEDETECTED_ENCODER1:
    QBEQ CW_ENCODER1, r3, 1
    SUB r6, r6, 1
    // Move value from register to the PRU local data memory using registers
    ST_POS1 r6, r0
    QBA READPINS_ENCODER2

CW_ENCODER1:
    ADD r6, r6, 1
    // Move value from register to the PRU local data memory using registers
    ST_POS1 r6, r0
    QBA READPINS_ENCODER2
	


READPINS_ENCODER2:
    //Store pin1 current value
    LSR r1, r31, ENC2_A
    AND r1, r1, 1
    
    //Store pin2 current value
    LSR r3, r31, ENC2_B
    AND r3, r3, 1
    
    //Invert pin1 value and store in r4 to test logic later on...
    NOT r4, r1
	AND r4, r4, 1
    
    //Store boolean for if pin1 is experiencing an edge (high to low) in r5
    AND r5, r4, r2
    
    //Update previous value of ENC_A
    MOV r2, r1
    
    //Jump to edge detection steps if edge detected
    QBEQ EDGEDETECTED_ENCODER2, r5, 1
    
    //Loop forever
    QBA READPINS_ENCODER1
    
//Jump to CW handling if clockwise (B high while edge present).
//Otherwise handle CCW in this label
EDGEDETECTED_ENCODER2:
    QBEQ CW_ENCODER2, r3, 1
    SUB r7, r7, 1
    // Move value from register to the PRU local data memory using registers
    ST_POS2 r7, r0
    QBA READPINS_ENCODER1

CW_ENCODER2:
    ADD r7, r7, 1
    // Move value from register to the PRU local data memory using registers
    ST_POS2 r7, r0
    QBA READPINS_ENCODER1
	
#ifdef AM33XX    

    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16

#else

    MOV R31.b0, PRU0_ARM_INTERRUPT

#endif

    HALT
