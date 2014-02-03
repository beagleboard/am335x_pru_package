/* =======================================================================      */
/* PRU_memAccessPRUDataRam.c --PRU Example to demonstrate accessing PRU DataRAM */
/*                                                                              */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*******************************************************************

  This example demonstrates how values can be stored and read into
  the PRU DataRAM memory using register pointing adress or by use of
  constant tables.
  The example loads 0x0010f012 into the 2nd meory location using local
  PRU local addressing adds 0x0000567A and stores the result into the
  3rd memory location using constant table global addressing

********************************************************************/

/************************************************************
* Include Files                                             *
************************************************************/

// Standard header files
#include <stdlib.h>
#include <stdio.h>
#include "tistdtypes.h"

// CSL header files
#include "csl/soc_OMAPL138.h"

// This module's header file
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM     (0)
#define ADDEND1     (0x0010F012u)
#define ADDEND2     (0x0000567Au)


/************************************************************
* Local Typedef Declarations                                *
************************************************************/


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit ( Uint8 pruNum );
static Bool LOCAL_examplePassed ( Uint8 pruNum );


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_memAccessPRUDataRam_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_memAccessPRUDataRam";

/************************************************************
* Global Function Definitions                               *
************************************************************/
Int32 main (void)
{
  printf("Starting PRU %s example.\r\n",exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified pru
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(int)));

  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit(PRU_NUM);

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

  // Verify the Successful execution of the example
  if ( LOCAL_examplePassed(PRU_NUM) )
  {
    printf("Example executed succesfully.\n");
  }
  else
  {
    printf("Example failed.\n");
  }

  // Disable the PRUSS when done
  PRU_disable();

  return 0;
}

/**********************************************************
  Local Function Definitions
***********************************************************/
static void LOCAL_exampleInit ( Uint8 pruNum )
{
  Uint32 *pruDataMem;
  // Initialize memory pointer
  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }
  // Flush the values in the PRU data memory locations
  pruDataMem[1] = 0x00;
  pruDataMem[2] = 0x00;

}

static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32 *pruDataMem;
  Uint32 result_1;
  Uint32 result_2;
  //Initialize pointer to PRU data memory
  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }
  //Read Resulting values placed in PRU data memory
  result_1 = pruDataMem[1];
  result_2 = pruDataMem[2];

  // Return acceptance/denial of success of the example
  return ((result_1 == ADDEND1) & (result_2 == ADDEND1 + ADDEND2)) ;
}


/* ======================================================================= */
/*            End of file:  PRU_memAccessPRUDataRam.c                      */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */



