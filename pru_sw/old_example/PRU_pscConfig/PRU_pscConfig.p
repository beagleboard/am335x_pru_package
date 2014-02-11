//=============================================================================
// File: PRU_pscConfig.p
// Desc: PRU configuring LPSC modules of system PSC
// Vers: 1.0
//
//  Copyright (c) 2009 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
// The PSC control routine allows changing PSC state of
// up to four LPSCs in the system
// Parameter Table:
//
// 31        23       15       7        0
//  ____________________________________
// |              MOD_COUNT             |
// |____________________________________|
// | PSCNUM | PDNUM  |  LPSC  |  STATE  |
// |____________________________________|
// | PSCNUM | PDNUM  |  LPSC  |  STATE  |
// |____________________________________|
// |                                    |
// |                             ...    |
// |____________________________________|
// | PSCNUM | PDNUM  |  LPSC  |  STATE  |
// |____________________________________|
//=============================================================================

.origin       0
.entrypoint   PSC_CONFIG

#include "PRU_pscConfig.hp"

PSC_CONFIG:
    // Load the count, init the offset
    LBCO      global.count, CONST_PRUDRAM, 0x00, 2
    MOV       global.offset, 0x0004

    // Setup pscRegs
    MOV       pscRegs.PTCMD,  #0x120
    MOV       pscRegs.PTSTAT, #0x128

NEXT_LPSC:
    // If count is zero, exit
    QBEQ      PSC_CONFIG_EXIT, global.count, #0x00

    // Get LPSC params
    LBCO      pscParams, CONST_PRUDRAM, global.offset, 4

    // If pscnum > 0x1, skip to next
    QBLT      DEC_CNT, pscParams.pscnum, #0x01

    // If pscnum == 0x1, skip to PSCNUM_ONE case
    QBEQ      PSCNUM_ONE, pscParams.pscnum, #0x01
    MOV32     global.pscRegBase, PSC0_ADDR
    QBA       GET_PD
PSCNUM_ONE:
    MOV32     global.pscRegBase, PSC1_ADDR

GET_PD:
    LSL       global.scratch, pscParams.module, 2
    MOV       pscRegs.MDSTAT, #0x800
    ADD       pscRegs.MDSTAT, pscRegs.MDSTAT, global.scratch
    MOV       pscRegs.MDCTL,  #0xA00
    ADD       pscRegs.MDCTL, pscRegs.MDCTL, global.scratch


WAIT_DONE1:
    // Wait for any outstanding transition to complete
    // while ( (PSC->PTSTAT) & (0x00000001 << domain) );
    LBBO      global.scratch, global.pscRegBase, pscRegs.PTSTAT, 4
    QBBS      WAIT_DONE1, global.scratch, pscParams.domain

    // If we are already in that state, just return
    // if (((PSC->MDSTAT[module]) & 0x1F) == state) return;
    LBBO      global.scratch.b0, global.pscRegBase, pscRegs.MDSTAT, 1
    AND       global.scratch.b0, global.scratch.b0, #0x1F
    QBEQ      DEC_CNT, pscParams.state, global.scratch.b0

    // Perform transition
    // PSC->MDCTL[module] = ((PSC->MDCTL[module]) & (0xFFFFFFE0)) | (state);
    LBBO      global.scratch, global.pscRegBase, pscRegs.MDCTL, 4
    AND       global.scratch.b0, global.scratch.b0, 0xE0
    OR        global.scratch.b0, global.scratch.b0, pscParams.state.b0
    SBBO      global.scratch, global.pscRegBase, pscRegs.MDCTL, 4

    // PSC->PTCMD |= (0x00000001 << domain);
    MOV32     global.scratch, 0x00000000
    SET       global.scratch.b0, pscParams.domain
    SBBO      global.scratch, global.pscRegBase, pscRegs.PTCMD, 4

WAIT_DONE2:
    // Wait for transition to complete
    // while ( (PSC->PTSTAT) & (0x00000001 << domain) );
    LBBO      global.scratch, global.pscRegBase, pscRegs.PTSTAT, 4
    QBBS      WAIT_DONE2, global.scratch, pscParams.domain

WAIT_DONE3:
    // Wait and verify the state
    // while (((PSC->MDSTAT[module]) & 0x1F) != state);
    LBBO      global.scratch, global.pscRegBase, pscRegs.MDSTAT, 4
    AND       global.scratch.b0, global.scratch.b0, #0x1F
    QBNE      WAIT_DONE3, pscParams.state, global.scratch.b0

DEC_CNT:
    // Calculate the remaining count
    SUB       global.count, global.count, 0x01
    ADD       global.offset, global.offset, 0x04

    // Get next LPSC transition
    QBA       NEXT_LPSC

PSC_CONFIG_EXIT:
    HALT
