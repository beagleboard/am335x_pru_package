/*********************************************************************
 * PRU_edmaConfig
 *
 * This example code demonstrates EDMA configuration by the PRU.  The
 * DSP is programmed to load the PRU example code and to detect the
 * EDMA interrupt.
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
#include "csl/cslr.h"
#include "csl/soc_OMAPL138.h"
#include "csl/cslr_cache.h"
#include "csl/cslr_dspintc.h"
#include "csl/cslr_psc_OMAPL138.h"

#include "device.h"

// PRU load/run functions
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

extern void intcVectorTable(void);


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define EDMA3_CC0_INT1	(8)
#define PRU_NUM         (0)


/************************************************************
* Local Typedef Declarations                                *
************************************************************/


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit ( Uint8 pruNum );
static Bool LOCAL_examplePassed ( );


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_edmaConfig_bin.h"

Uint8 srcBuf[0x100];
Uint8 dstBuf[0x100];


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_edmaConfig";
CSL_DspintcRegsOvly intcRegs = (CSL_DspintcRegsOvly)CSL_INTC_0_REGS;
Uint8 edmaIntReceived = 0;


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main()
{
  printf("Starting %s example.\r\n",exampleName);

  // Make sure PRU is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified pru
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit(PRU_NUM);

  printf("\tINFO: Executing example.\r\n");
  PRU_run(PRU_NUM);

  // Wait for EDMA CC0 interrupt.
  while ( !edmaIntReceived );
  printf("\tINFO: EDMA interrupt received.\r\n");

  // Wait for the PRU to call the HALT command
  if (PRU_waitForHalt(PRU_NUM,-1) == E_PASS)
  {
    printf("\tINFO: PRU halted successfully.\r\n");
  }
  else
  {
    printf("\tINFO: PRU halt failed.\r\n");
  }

  // Check if the example executed successfully
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

static void LOCAL_exampleInit(Uint8 pruNum)
{
  Uint32 i;

  Uint32 *pruDataMem;

  // Turn on the EDMA CC and TCs
  DEVICE_LPSCTransition(CSL_PSC_0, CSL_PSC_CC0, 0, CSL_PSC_MDCTL_NEXT_ENABLE);
  DEVICE_LPSCTransition(CSL_PSC_0, CSL_PSC_TC0, 0, CSL_PSC_MDCTL_NEXT_ENABLE);
  DEVICE_LPSCTransition(CSL_PSC_0, CSL_PSC_TC1, 0, CSL_PSC_MDCTL_NEXT_ENABLE);

  // Initialize memory pointer
  if (pruNum == 0)
  {
    pruDataMem = (Uint32 *) PRU0_DATA_RAM_START;
  }
  else if (pruNum == 1)
  {
    pruDataMem = (Uint32 *) PRU1_DATA_RAM_START;
  }

  // Put src and dst addresses into PRU data memory so PRU code can read them
  pruDataMem[0] = (Uint32) srcBuf;
  pruDataMem[1] = (Uint32) dstBuf;

	// Init src and dst buffers
  for (i = 0; i < sizeof(srcBuf); i++)
  {
    srcBuf[i] = rand() & 0xFF;
    dstBuf[i] = 0;
  }

  // Clear EDMA CC0 interrupt
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC8, SET);

  // Connect the EDMA CC0 event to the DSP interrupt 12
  CSL_FINS(intcRegs->INTMUX3, DSPINTC_INTMUX3_INTSEL12, EDMA3_CC0_INT1);

  // Set ISTP to point to the vector table address
  ISTP = (Uint32)intcVectorTable;

  // Clear all interrupts, bits 4 thru 15
  ICR = 0xFFF0;

  // Enable the bits for non maskable interrupt 12 and NMIE
  IER = 0x1002;

  // Enable interrupts, set GIE bit
  _enable_interrupts();
}

static Bool LOCAL_examplePassed()
{
  Uint32 i;

  /* Check data. */
  for (i = 0; i < sizeof(srcBuf); i++)
  {
    if ( srcBuf[i] != dstBuf[i] )
    {
			return FALSE;
    }
  }
  return TRUE;
}


/*
 * Interrupt service routines
 */
interrupt void EDMA3_CC0_INT1_isr()
{
  edmaIntReceived = 1;
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC8, SET);
}


/***********************************************************
* End file                                                 *
***********************************************************/
