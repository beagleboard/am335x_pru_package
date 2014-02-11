/* --------------------------------------------------------------------------
  FILE        : PRU_mem1DTransfer.c
  PROJECT     : OMAP-L1x/C674x PRU Development
  DESC        : PRU Mem1D Example
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

#include "PRU_mem1DTransfer_bin.h"

const Uint8 someDataSrc[32] =
{ 0x01, 0x03, 0x05, 0x07,
  0x02, 0x04, 0x06, 0x08,
  0x11, 0x13, 0x15, 0x17,
  0x12, 0x14, 0x16, 0x18,
  0x21, 0x23, 0x25, 0x27,
  0x22, 0x24, 0x26, 0x28,
  0x31, 0x33, 0x35, 0x37,
  0x32, 0x34, 0x36, 0x38 };

static Uint8 someDataDest[32];


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "Mem1D Transfer";


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
  Int32 i=0;

  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }

  pruDataMem[0] = (Uint32) &someDataSrc;
  pruDataMem[1] = (Uint32) &someDataDest;
  pruDataMem[2] = (Uint32) 32;

  // Check each byte at the destination
  for (i=0; i<32; i++)
  {
    someDataDest[i] = 0xFF;
  }

  // Make sure data is written back
  ((CSL_CacheRegsOvly) CSL_CACHE_0_REGS)->L1DWBINV = 0x01;
}


static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32 *pruDataMem;
  Int32 i;

  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }

  // Check counter
  if (pruDataMem[2] != 0)
  {
    return FALSE;
  }

  // Check each byte at the destination
  for (i=0; i<32; i++)
  {
    if (someDataSrc[i] != someDataDest[i])
    {
      return FALSE;
    }
  }

  return TRUE;
}


/***********************************************************
* End file                                                 *
***********************************************************/
