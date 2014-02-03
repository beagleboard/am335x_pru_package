/* --------------------------------------------------------------------------
  FILE        : PRU_multiply.c
  PROJECT     : OMAP-L1x/C674x PRU Development
  DESC        : PRU Multiply Example
 ----------------------------------------------------------------------------- */

/************************************************************
* Include Files                                             *
************************************************************/

// Standard header files
#include <stdlib.h>
#include <stdio.h>
#include "tistdtypes.h"

// CSL header files
#include "csl/cslr.h"
#include "csl/soc_OMAPL138.h"
#include "csl/cslr_cache.h"

// PRU load/run functions
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM  (0)
#define SRC1     (0x100)
#define SRC2     (0x1AA)


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

#include "PRU_multiply_bin.h"




/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "Muliply";


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main (void)
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

  if ( LOCAL_examplePassed(PRU_NUM) )
  {
    printf("Example completed successfully.");
  }
  else
  {
    printf("Example failed.");
  }

  // Disable the PRUSS when done
  PRU_disable();
}


/************************************************************
* Local Function Definitions                                *
************************************************************/

static void LOCAL_exampleInit ( Uint8 pruNum )
{
  Uint32 *pruDataMem;

  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }

  pruDataMem[0] = (Uint32) SRC1;
  pruDataMem[1] = (Uint32) SRC2;
  pruDataMem[2] = 0xFFFFFFFF;

  // Make sure data is written back
  ((CSL_CacheRegsOvly) CSL_CACHE_0_REGS)->L1DWBINV = 0x01;
}


static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32 *pruDataMem;

  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }

  // Check result
  if (pruDataMem[2] != (SRC1*SRC2))
  {
    return FALSE;
  }

  return TRUE;
}


/***********************************************************
* End file                                                 *
***********************************************************/
