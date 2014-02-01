/*********************************************************************
 * PRU_timer0Config
 *
 * This example code demonstrates Timer0 configuration by the PRU.  The
 * DSP is programmed to load the PRU example code and to detect the
 * Timer0 interrupt.
 *********************************************************************/

/************************************************************
* Include Files                                             *
************************************************************/

// Standard header files
#include <stdlib.h>
#include <stdio.h>
#include "tistdtypes.h"

// C6000 header file
#include <c6x.h>

// CSL header files
#include "csl/soc_OMAPL138.h"
#include "csl/cslr_dspintc.h"

// This module's header file
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

extern void intcVectorTable(void);


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM             (0)
#define T64P0_TINT12_EVT    (4)


/************************************************************
* Local Typedef Declarations                                *
************************************************************/


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit();
static Bool LOCAL_examplePassed();


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_timer0Config_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

CSL_DspintcRegsOvly intcRegs = (CSL_DspintcRegsOvly)CSL_INTC_0_REGS;
Uint8 tmrIntReceived = 0;
String exampleName = "PRU_timer0Config";


/************************************************************
* Global Function Definitions                               *
************************************************************/

/*
 * Interrupt service routine
 */
interrupt void T64P0_TINT12_isr()
{
  tmrIntReceived = 1;
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC4, SET);
}


void main()
{
  printf("Starting PRU %s example.\r\n",exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

  printf("\tINFO: Initializing example.\r\n");
  // Configure the DSP Interrupt Controller
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
}


/************************************************************
* Local Function Definitions                                *
************************************************************/

static void LOCAL_exampleInit()
{
  /* clear Timer 0 interrupt */
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC4, SET);

  /* connect the Timer0 event to the DSP interrupt 12 */
  CSL_FINS(intcRegs->INTMUX3, DSPINTC_INTMUX3_INTSEL12, T64P0_TINT12_EVT);

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
/* Wait for Timer 0 interrupt. */
  while ( !tmrIntReceived );
  printf("\tINFO: Timer0 interrupt received!\r\n");
  return TRUE;
}


/* ======================================================================= */
/*          End of file:   PRU_semaphore.c                                 */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */









