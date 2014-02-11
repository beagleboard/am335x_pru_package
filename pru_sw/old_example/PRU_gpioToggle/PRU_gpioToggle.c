/*********************************************************************
 * PRU_gpioToggle.c
 *
 * This example code demonstrates the PRU toggling GPIO pins.
 * The DSP is used to load the PRU example code.
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


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define  PRU_NUM        (0)


/************************************************************
* Local Function Declarations                               *
************************************************************/


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_gpioToggle";
CSL_DspintcRegsOvly intcRegs = (CSL_DspintcRegsOvly)CSL_INTC_0_REGS;
Uint8 tmrIntReceived = 0;

#include "PRU_gpioToggle_bin.h"


/************************************************************
* Global Function Definitions                               *
************************************************************/

/*
 * Main function
 */
void main()
{
  printf("Starting %s example.\r\n", exampleName);

  // Make sure PRU is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified PRU
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

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

  printf("Example completed successfully.");

  // Disable the PRUSS when done
  PRU_disable();
}


/* ======================================================================= */
/*               End of file:   PRU_gpioToggle.c                           */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

