/* =======================================================================      */
/*PRU_PRUtoDSP_Interrupt.c --PRU Example to demonstrate PRU Interrupting DSP    */
/*                                                                              */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*******************************************************************/
/*********************************************************************
 * PRU_PRUtoDSP_Interrupt.c
 *
 * This example code demonstrates the PRU interrupting the DSP by generating
 * internal system events that are mapped to host2-host9 ports to generate events
 * PRUSS_EVTOUT0-PRUSS_EVTOUT7 for the DSP.These events are mapped  on the DSP
 * as events 6,17,22,35,39,44,50 respectively.By hooking these events to one of
 * the DSP interrupts the PRU is given the capabilty of interrupting the DSP.
 *
 * DSP is programmed to load the PRU example code and to detect the
 * PRU interrupt.
 *********************************************************************/


/************************************************************
* Include Files                                             *
************************************************************/

// Standard header files
#include <stdlib.h>
#include <stdio.h>
#include "tistdtypes.h"

// C6000 DSP Header file
#include <c6x.h>

// CSL header files
#include "csl/cslr_dspintc.h"
#include "csl/soc_OMAPL138.h"

// PRU Function header file
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

extern void intcVectorTable(void);
/* Memory start defintions are made in the linker command file*/
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define  PRUSS_EVTOUT0_EVT    (6)
#define  PRU_NUM              (0)
/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit();
static Bool LOCAL_examplePassed();


/************************************************************
* Global Variable Definitions                               *
************************************************************/

CSL_DspintcRegsOvly intcRegs = (CSL_DspintcRegsOvly)CSL_INTC_0_REGS;
Uint8 PRU_IntReceived = 0;
String exampleName = "PRU_PRUtoDSP_Interrupt";

#include "PRU_PRUtoDSP_Interrupt_bin.h"


/************************************************************
* Global Function Definitions                               *
************************************************************/


/*
 * Interrupt service routines
 */
interrupt void PRUSS_EVTOUT0_isr()
{
  PRU_IntReceived = 1;
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC6, SET);
}

void main()
{
  printf("Starting %s example.\r\n", exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified pru
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit();

  printf("\tINFO: Executing example.\r\n");
  PRU_run(PRU_NUM);

  // Wait for the PRU to call the HALT command
  if (PRU_waitForHalt(PRU_NUM,-1) == E_PASS)
  {
    printf("\tINFO: PRU halted successfully.\r\n");
  }
  else
  {
    printf("\tINFO: PRU halt failed.\r\n");
  }

  // Check to see if the example produced desired result
  if ( LOCAL_examplePassed(PRU_NUM) )
  {
    printf("Example executed succesfully.\n");
  }
  else
  {
    printf("Example failed.\n");
  }

  // Put the PRUSS back in the disabled state
  PRU_disable();
}


/**********************************************************
  Local Function Definitions
***********************************************************/

/*
 * DSP interrupt controller configuration.
 */
static void LOCAL_exampleInit()
{
  /* clear PRUSS_EVTOUT0 interrupt */
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC6, SET);

  /* connect the PRUSS_EVTOUT0 event to the DSP interrupt 12 */
  CSL_FINS(intcRegs->INTMUX3, DSPINTC_INTMUX3_INTSEL12, PRUSS_EVTOUT0_EVT);

  /* set ISTP to point to the vector table address */
  ISTP = (Uint32)intcVectorTable;

  /* clear all interrupts, bits 4 thru 15 */
  ICR = 0xFFF0;

  /* enable the bits for non maskable interrupt 12 and NMIE*/
  IER = 0x1002;

  /* enable interrupts, set GIE bit */
  _enable_interrupts();
}

static Bool LOCAL_examplePassed()
{
  Uint32 *memPtr;

  memPtr = (Uint32 *)EXTERNAL_RAM_START;

  // Wait for PRUSS_EVTOUT0 interrupt.
  while ( !PRU_IntReceived );
  if (memPtr[1] == 0x0B)
  {
    printf("\tINFO: PRU interrupt received!\n");
  }
  else
  {
    printf("\tINFO: PRU interrupt received!\n");
  }

  return TRUE;
}



/* ======================================================================= */
/*                End of file:  PRU_PRUtoDSP_Interrupt.c                   */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */







