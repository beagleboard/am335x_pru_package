/* =======================================================================      */
/* PRU_PRUtoPRU_Interrupt.c --PRU Example to demonstrate DSP interrupting PRU   */
/*                            to perform some event handling function.          */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*********************************************************************
 * PRU_PRUtoPRU_Interrupt.c
 *
 * This example illustrates how two PRUs can communicate between each other
 * by interrupting each other during a process. In this example code the PRU0
 * generates a system event 32 by writing into its R31 register which sends an
 * interrupt to PRU1 which is polling for it. On receiving the interrupt the PRU1
 * performs certain functionality and sets an external flag. The PRU1 completes its
 * task and interrupts PRU0 once the task is done by writing into its R31 register
 * The PRU0 polls for the interrupt and acknowledges the completion of task
 * by setting a flag
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
#include "csl/cslr_dspintc.h"
#include "csl/soc_OMAPL138.h"

// This module's header file
#include "pru.h"


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define FLAG1         (0x0A)
#define FLAG2         (0x0B)
#define TIME_CONST    (0x5000)
#define SCORE         (0x0000000Au)


/************************************************************
* Explicit External Declarations                            *
************************************************************/

/* Memory start defintions are made in the linker command file*/
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit ( );
static Bool LOCAL_examplePassed ( Uint8 pruNum );


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_PRU0toPRU1_Interrupt_bin.h"
#include "PRU_PRU1toPRU0_Interrupt_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_PRUtoPRU_Interrupt";
CSL_DspintcRegsOvly intcRegs = (CSL_DspintcRegsOvly)CSL_INTC_0_REGS;
Uint8 tmrIntReceived = 0;


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main()
{
  printf("Starting PRU %s example.\r\n",exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified PRU
  printf("\tINFO: Loading PRU0 core instruction memory.\n");
  PRU_load(CSL_PRUCORE_0, (Uint32*)PRU0_Code, (sizeof(PRU0_Code)/sizeof(Uint32)));

  // Enable and load the code to the specified PRU
  printf("\tINFO: Loading PRU1 core instruction memory.\n");
  PRU_load(CSL_PRUCORE_1, (Uint32*)PRU1_Code, (sizeof(PRU1_Code)/sizeof(Uint32)));

  printf("\tINFO: Initializing example.\r\n");
  // Initialize the Flag locations of PRU0 and PRU1
  LOCAL_exampleInit();

  // Enable and Start PRU1
  printf("\tINFO: Executing loaded PRU1 program.\n");
  PRU_run(CSL_PRUCORE_1);

	// Enable and Start PRU0
  printf("\tINFO: Executing loaded PRU0 program.\n");
  PRU_run(CSL_PRUCORE_0);

  // Wait for PRU0 to halt
  if ( PRU_waitForHalt(CSL_PRUCORE_0, -1) == E_PASS)
  {
    printf("\tINFO: PRU0 halted successfully.\r\n");
  }
  else
  {
    printf("\tINFO: PRU0 halt failed.\r\n");
  }

  // Wait for PRU1 to halt
  if ( PRU_waitForHalt(CSL_PRUCORE_1, -1) == E_PASS)
  {
    printf("\tINFO: PRU1 halted successfully.\r\n");
  }
  else
  {
    printf("\tINFO: PRU1 halt failed.\r\n");
  }


  // Check to see if the example produced desired result
  if ( LOCAL_examplePassed(CSL_PRUCORE_1) )
  {
    printf("%s example executed succesfully.\n", exampleName);
  }
  else
  {
    printf("%s example failed.\n", exampleName);
  }

  // Disable the PRUSS when done
  PRU_disable();
}
/************************************************************
* Local Function Definitions                                *
************************************************************/

/*
* Example Initialization routine
*/
static void LOCAL_exampleInit ()
{
  Uint32 *ptrflag;

  // Initialize memory pointer to start of External DDR memory
  ptrflag = (Uint32 *)&EXTERNAL_RAM_START;

  //FLush the flag locations of PRU0 an PRU1
  ptrflag[0] =0x00;
  ptrflag[1] =0x00;

}
/*
* Example Result verification routine
*/
static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32  *ptrflag;
  Uint8 result_0;
  Uint8 result_1;

  ptrflag = (Uint32 *)&EXTERNAL_RAM_START;

  //Read in the first example done flags
  result_0 =ptrflag[0];
  result_1 =ptrflag[1];
  //check if Example completed
  return ((result_0 == FLAG1) & (result_1 == FLAG2) ) ;
}



/* ======================================================================= */
/*               End of file:   PRU_PRUtoPRU_Interrupt.c                   */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */








