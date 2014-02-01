/* ======================================================================= */
/* PRU_memAccessL2.c -- PRU Example to demonstrate accessing DSP L2 RAM    */
/*                                                                         */
/* Rev 0.0.1   May 29th 2009                                               */
/* Initial version of the file                                             */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */
/************************************************************
DESCRIPTION:
This example demonstrates how values can be loaded into the L2 memory
by configuring the PRU Constant Table Programmable Pointer Register 0.

*************************************************************/

// Standard header files
#include <stdlib.h>
#include <stdio.h>
#include "tistdtypes.h"

// CSL header files
#include "csl/soc_OMAPL138.h"

// PRU load/run functions
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

/*Memory start declaration in Linker command file pruRun.cmd*/
extern __FAR__ Uint32 L2_RAM_START, L2_RAM_SIZE;


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define ADDEND1  (0x52737829u)
#define ADDEND2  (0x12341234u)
#define ADDEND3  (0xAAAA0210u)

#define PRU_NUM0  (0)
#define PRU_NUM1  (1)


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit ( Uint8 pruNum );
static Bool LOCAL_examplePassed ( Uint8 pruNum );


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_memAccessL2_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_memAccessL2";


/************************************************************
* Global Function Definitions                               *
************************************************************/

Int32 main (void)
{
  printf("Starting %s example.\r\n",exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified pru
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM0, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

  // Memory 0x11810000-0x11810018 are used for
  //this demonstration and hence the pointer(L2_RAM_START) is hard coded
  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit(PRU_NUM0);

  printf("\tINFO: Executing example.\r\n");
  PRU_run(PRU_NUM0);

  // Wait for the PRU to call the HALT command
  if (PRU_waitForHalt(PRU_NUM0,-1) == E_PASS)
  {
    printf("\tINFO: PRU0 halted successfully.\r\n");
  }
  else
  {
    printf("\tINFO: PRU0 halt failed.\r\n");
  }
  // Verify the Successful execution of the example
  if ( LOCAL_examplePassed(PRU_NUM0) )
  {
    printf("Example completed successfully on PRU0.\r\n");
  }
  else
  {
    printf("Example failed on PRU0.\r\n");
  }

  // Disable the PRUSS when done
  PRU_disable();

  // Enable and load the code to the specified pru
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM1, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(int)));

  // Memory 0x11810000-0x11810018 are used for
  // this demonstration and hence the pointer(L2_RAM_START) is hard coded
  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit(PRU_NUM1);

  printf("\tINFO: Executing example.\r\n");
  PRU_run(PRU_NUM1);

  // Wait for the PRU to call the HALT command
  if (PRU_waitForHalt(PRU_NUM1,-1) == E_PASS)
  {
    printf("\tINFO: PRU1 halted successfully.\r\n");
  }
  else
  {
    printf("\tINFO: PRU1 halt failed.\r\n");
  }

  // Verify the Successful execution of the example
  if ( LOCAL_examplePassed(PRU_NUM1) )
  {
    printf("Example completed successfully on PRU1.");
  }
  else
  {
    printf("Example failed on PRU1.");
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
  Uint32 *memPtr;

  //Initialize pointer to L2 data memory
  memPtr = (Uint32 *) 0x11810000;

  // Flush the values in the PRU data memory locations
  memPtr[3] = 0x00;
  memPtr[4] = 0x00;
  memPtr[5] = 0x00;
}

static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32 *memPtr;

  //Initialize pointer to L2 data memory
  memPtr = (Uint32 *) 0x11810000;

  // Return acceptance/denial of success of the example
  return ((memPtr[3] == ADDEND1) && (memPtr[4] == ADDEND2) && (memPtr[5] == ADDEND3)) ;
}


/* ======================================================================= */
/*  End of file:  PRU_memAccessL2.c.c                                      */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */


