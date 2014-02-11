/* --------------------------------------------------------------------------
  FILE        : PRU_pscConfig.c
  PROJECT     : DA8xx/OMAP-L138/C674x PRU Development
  DESC        : PRU PSC Control Example
 ----------------------------------------------------------------------------- */

/************************************************************
* Include Files                                             *
************************************************************/

#include <stdlib.h>
#include <stdio.h>

// General type include
#include "tistdtypes.h"

// CSL header files
#include "csl/cslr.h"
#include "csl/soc_OMAPL138.h"
#include "csl/cslr_psc_OMAPL138.h"

// Device Specific Init Functions
#include "device.h"

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

#include "PRU_pscConfig_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PSC Config";


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

  return 0;
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

  // Put LPSCs in opposite states to what PRU will place them.
  DEVICE_LPSCTransition(CSL_PSC_0, CSL_PSC_UART0, 0, CSL_PSC_MDCTL_NEXT_SYNCRST );
  DEVICE_LPSCTransition(CSL_PSC_0, CSL_PSC_SPI0,  0, CSL_PSC_MDCTL_NEXT_ENABLE  );
  DEVICE_LPSCTransition(CSL_PSC_1, CSL_PSC_I2C1,  0, CSL_PSC_MDCTL_NEXT_SYNCRST );
  DEVICE_LPSCTransition(CSL_PSC_1, CSL_PSC_SPI1,  0, CSL_PSC_MDCTL_NEXT_ENABLE  );

  // Set up the PRU to do the PSC state transition
  // Set the PSC numbers
  pruDataMem[0] = 4;
  pruDataMem[1] = ((CSL_PSC_0                   & 0xFF)<< 24 ) |
                  ((0                           & 0xFF)<< 16 ) |
                  ((CSL_PSC_UART0               & 0xFF)<<  8 ) |
                  ((CSL_PSC_MDCTL_NEXT_ENABLE   & 0xFF)<<  0 );

  pruDataMem[2] = ((CSL_PSC_0                   & 0xFF)<< 24 ) |
                  ((0                           & 0xFF)<< 16 ) |
                  ((CSL_PSC_SPI0                & 0xFF)<<  8 ) |
                  ((CSL_PSC_MDCTL_NEXT_SYNCRST  & 0xFF)<<  0 );

  pruDataMem[3] = ((CSL_PSC_1                   & 0xFF)<< 24 ) |
                  ((0                           & 0xFF)<< 16 ) |
                  ((CSL_PSC_I2C1                & 0xFF)<<  8 ) |
                  ((CSL_PSC_MDCTL_NEXT_ENABLE   & 0xFF)<<  0 );

  pruDataMem[4] = ((CSL_PSC_1                   & 0xFF)<< 24 ) |
                  ((0                           & 0xFF)<< 16 ) |
                  ((CSL_PSC_SPI1                & 0xFF)<<  8 ) |
                  ((CSL_PSC_MDCTL_NEXT_SYNCRST  & 0xFF)<<  0 );
}


static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Bool retVal = TRUE;
  CSL_PscRegsOvly PSC0 = (CSL_PscRegsOvly) CSL_PSC_0_REGS;
  CSL_PscRegsOvly PSC1 = (CSL_PscRegsOvly) CSL_PSC_1_REGS;

  // Make sure the PRU did the PSC transitions
  retVal = retVal && ( CSL_FEXT(PSC0->MDSTAT[CSL_PSC_UART0], PSC_MDSTAT_STATE) == CSL_PSC_MDSTAT_STATE_ENABLE );
  retVal = retVal && ( CSL_FEXT(PSC0->MDSTAT[CSL_PSC_SPI0] , PSC_MDSTAT_STATE) == CSL_PSC_MDCTL_NEXT_SYNCRST  );
  retVal = retVal && ( CSL_FEXT(PSC1->MDSTAT[CSL_PSC_I2C1] , PSC_MDSTAT_STATE) == CSL_PSC_MDSTAT_STATE_ENABLE );
  retVal = retVal && ( CSL_FEXT(PSC1->MDSTAT[CSL_PSC_SPI1] , PSC_MDSTAT_STATE) == CSL_PSC_MDCTL_NEXT_SYNCRST  );

  return retVal;
}


/***********************************************************
* End file                                                 *
***********************************************************/
