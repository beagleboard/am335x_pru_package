//=============================================================================================================
// File: PRU_access_const_tables.p
// Desc: Use of constant tables to access memory and registers associated with peripherals
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=================================================================================================================
// PRU Example to access constant tables
// Note:
// Fixed base addresses in constant table: Entry 0-23
// Programmable base addresses in constant table: Entry 24-31
// Programmable entries are configured using programmbale pointer registers and Index registers of the PRU
//
// The following example demonstartes use of Constant table to write data into HPI registers from external DDR memory
// use of fixed entries and programmable entries in the Constant table
//
// Intialization in CCS project:
// A value of 0x00000001 is loaded into 1st memory location of the DDR
//==================================================================================================================

.origin 0
.entrypoint ACCESS_CONST_TABLE


#include "PRU_access_const_table.hp"

ACCESS_CONST_TABLE:
    // To access the DDR memory, the programmable pointer register is
    // configured by setting C31[15:0] field. Set R0 to zero and store
    // that value into in CTPR_1 to configure c31_pointer[15:0]

    MOV32     r0, 0x00000000
    MOV       r1, CTPPR_1
    ST32      r0, r1

    // Load values from external DDR Memory into registers r0
    LBCO      r0, CONST_DDR, 0, 4

    // Entry 15 of the Constant table contains the base address for HPI control registers.
    // Write into the PWREMU_MGMT register and read from HPIC using C15 as base address

    //Setting the FREE bit of the PWREMU_MGMT to set the HPI in FREE mode
    SBCO      r0, CONST_HPI, PWREMU_MGMT_OFFSET, 4

    // Read value in the HPIC register to find out HPI configurations
    LBCO      r0, CONST_HPI, HPIC_REG_OFFSET, 4

    //Store HPI configuration in 2nd location of DDR
    SBCO      r0, CONST_DDR, 4, 4

    HALT

//===================================================================================
// End of file PRU_access_const_tables.p
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//===================================================================================







