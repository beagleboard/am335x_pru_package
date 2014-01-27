/*********************************************************************
 * PRU_timer0Interrupt
 *
 * This example code demonstrates the PRU detecting a Timer0 interrupt.
 * The DSP is used to program Timer0, and to load the PRU example code.
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
#include "csl/cslr_tmr.h"

// Util header
#include "util.h"

// This module's header file
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM             (0)
#define TIME_CONST				  (0x5000)


/************************************************************
* Local Typedef Declarations                                *
************************************************************/


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit(Uint8 pruNum);
static Bool LOCAL_examplePassed(Uint8 pruNum);


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_timer0Interrupt_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_timer0Interrupt";


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main()
{
  CSL_TmrRegsOvly timer0Regs = (CSL_TmrRegsOvly)CSL_TMR_0_REGS;

  printf("Starting %s example.\r\n",exampleName);

	// Make sure PRU is first disabled/reset
	PRU_disable();

	printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

  printf("\tINFO: Initializing example.\r\n");
  // Configure Timer0
  LOCAL_exampleInit(PRU_NUM);

	printf("\tINFO: Executing example.\r\n");
	PRU_run(PRU_NUM);

	// Give PRU some time
  UTIL_waitLoop(0x100);

	// Enable Timer0 in one-shot mode
	timer0Regs->TCR = 0x00000040;

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
    printf("Example executed succesfully.\r\n");
  }
  else
  {
    printf("Example failed.\r\n");
  }
}


/************************************************************
* Local Function Definitions                                *
************************************************************/

static void LOCAL_exampleInit(Uint8 pruNum)
{
  Uint32 *pruDram;
  CSL_TmrRegsOvly timer0Regs = (CSL_TmrRegsOvly)CSL_TMR_0_REGS;

  /* Configure Timer0 */
	timer0Regs->TGCR &= ~0x00000003;	// place timer in reset
	timer0Regs->TIM12 = 0;				// reset counter registers
	timer0Regs->TCR = 0x00000000;		// setup TCR register
	timer0Regs->TGCR = 0x00000015;		// setup global control register
	timer0Regs->PRD12 = TIME_CONST;		// load time constant into period register
	timer0Regs->INTCTLSTAT |= 0x000A000A;	// clear pending interrupts
	timer0Regs->INTCTLSTAT |= 0x00000001;	// enable timer0 1:2 interrupt

	if (pruNum == 0)
  {
		pruDram = (Uint32 *)PRU0_DATA_RAM_START;
  }
	else
  {
		pruDram = (Uint32 *)PRU1_DATA_RAM_START;
  }
	pruDram[0] = 0xFFFFFFFF;
}

static Bool LOCAL_examplePassed(Uint8 pruNum)
{
  Uint32 *pruDram;

  if (pruNum == 0)
  {
		pruDram = (Uint32 *)PRU0_DATA_RAM_START;
  }
	else
  {
		pruDram = (Uint32 *)PRU1_DATA_RAM_START;
  }

  /* Wait for PRU to acknowledge completion. */
	while ( pruDram[0] == 0xFFFFFFFF );

  return TRUE;

}


/* ======================================================================= */
/*          End of file:   PRU_timer0Interrupt.c                                 */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */
