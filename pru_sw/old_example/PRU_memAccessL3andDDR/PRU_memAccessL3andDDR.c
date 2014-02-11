/* =======================================================================      */
/* PRU_memAccessL3andDDR.c --PRU Example to demonstrate accessing L3 and        */
/* external DDRAM memory                                                        */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*******************************************************************

  This example demonstrates access of L3 memory and external DDR memory
  using constant table entries which are programmed using Constant table
  Programmable poiter Register 1(CTPPR_1).
  The example initially loads 3 values into the external DDR RAM.
  The example code first configures the CTPPR_1 to point to appropriate
  location in the DDR memory and the L3 memory.
  The values are then read from the DDR memory and stored into the L3 memory
  using the values in the 30th and 31st entries of the constant table.

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

// PRU load/run functions
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

/* Memory start defintions are made in the linker command file*/
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;
extern __FAR__ Uint32 SHARED_RAM_START, SHARED_RAM_SIZE;


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM       (0)
#define ADDEND1       (0x98765400u)
#define ADDEND2       (0x12345678u)
#define ADDEND3       (0x10210210u)

//Memory locations 0xc0001000-0xc00010C are used for the example
//decimal offset for a 32 bit pointer pointing to DDR memory
#define OFFSET_DDR    (1024)

//Memory locations 0x80002000-0x800200C are used for the example
//decimal offset for a 32 bit pointer pointing to L3 memory
#define OFFSET_L3     (2048)


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

#include "PRU_memAccessL3andDDR_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_memAccessL3andDDR";


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
  // Check to see if the example produced desired result
  if ( LOCAL_examplePassed(PRU_NUM) )
  {
    printf("Example executed succesfully.\r\n");
  }
  else
  {
    printf("Example failed.\r\n");
  }

  // Disable the PRUSS when done
  PRU_disable();

  return 0;
}

/************************************************************
* Local Function Definitions                                *
************************************************************/

static void LOCAL_exampleInit ( Uint8 pruNum )
{
  Uint32 *pruDataMem;
  // Initialize memory pointer to start of External DDR memory
  pruDataMem = (Uint32 *) &EXTERNAL_RAM_START;
  //Store Addends in DDR memory locations
  pruDataMem[OFFSET_DDR] = ADDEND1;
  pruDataMem[OFFSET_DDR+1] = ADDEND2;
  pruDataMem[OFFSET_DDR+2] = ADDEND3;

}


static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32 *pruDataMem;
  Uint32 result_0;
  Uint32 result_1;
  Uint32 result_2;
  // Initialize memory pointer to start of L3 memory
  pruDataMem = (Uint32 *) &SHARED_RAM_START;
  //Read in the first three memory locations
  result_0 = pruDataMem[OFFSET_L3 ];
  result_1 = pruDataMem[OFFSET_L3 +1];
  result_2 = pruDataMem[OFFSET_L3 +2];

  return ((result_0 == ADDEND1) & (result_1 ==  ADDEND2) & (result_2 ==  ADDEND3)) ;
}



/* ======================================================================= */
/* 				 End of file:   PRU_memAccessL3andDDR.c                    */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */


