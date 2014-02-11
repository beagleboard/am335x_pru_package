/* =======================================================================      */
/* PRU_DSPtoPRU_Interrupt.c --PRU Example to demonstrate DSP interrupting PRU   */
/*                            to perform some event handling function.          */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*********************************************************************
 * PRU_DSPtoPRU_Interrupt.c
 *
 * In this example code the Timer0 configured by the DSP.The
 * DSP enables the timer interrupt and waits for interrupt once the interrupt
 * is received,it exports the event handling to the PRU by generating a system
 * event that the PRU is polling by writing into the SRSR2 registers.
 * The PRU performs Event handling functions like reseting the timer
 * and performing some other functionality .Once it completes the event handling,
 * it informs the the DSP that the interrupt was serviced interrupt by writing
 * in a memory location which is verified by the code to check if the example
 * executed successfully.
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
#include "csl/cslr_pruintc.h"
#include "csl/cslr_tmr.h"
#include "csl/soc_OMAPL138.h"

// PRU Function header file
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

/* Memory start defintions are made in the linker command file*/
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;
extern void intcVectorTable(void);


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define  T64P0_TINT12_EVT   (4)
#define  PRU_NUM            (0)
#define  TIME_CONST         (0x5000)
#define  SCORE              (0x0000000A)

/************************************************************
* Local Function Declarations                               *
************************************************************/

void T64P0_TINT12_isr();
static void LOCAL_configureDspIntc( );
static void LOCAL_exampleInit ( );
static Bool LOCAL_examplePassed ( );
static void LOCAL_exampleFinalize( );


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_DSPtoPRU_Interrupt";
CSL_DspintcRegsOvly intcRegs = (CSL_DspintcRegsOvly)CSL_INTC_0_REGS;
Uint8 tmrIntReceived = 0;

#include "PRU_DSPtoPRU_Interrupt_bin.h"


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main()
{
  CSL_TmrRegsOvly timer0Regs = (CSL_TmrRegsOvly)CSL_TMR_0_REGS;

  printf("Starting %s example.\r\n",exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  // Enable and load the code to the specified pru
  printf("\tINFO: Loading example.\r\n");
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(Uint32)));

  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit(PRU_NUM);

  printf("\tINFO: Executing example.\r\n");
  PRU_run(PRU_NUM);

  // Configure Timer0
  timer0Regs->TGCR &= ~0x00000003;    // place timer in reset
  timer0Regs->TIM12 = 0;              // reset counter registers
  timer0Regs->TCR = 0x00000000;       // setup TCR register
  timer0Regs->TGCR = 0x00000015;      // setup global control register
  timer0Regs->PRD12 = TIME_CONST;     // load time constant into period register
  timer0Regs->INTCTLSTAT |= 0x000A000A;   // clear pending interrupts
  timer0Regs->INTCTLSTAT |= 0x00000001;   // enable timer0 1:2 interrupt

  // Configure the DSP Interrupt Controller
  LOCAL_configureDspIntc();

  // Enable Timer0 in one-shot mode
  timer0Regs->TCR = 0x00000040;

  // Wait for Timer 0 interrupt.
  printf("\tINFO: Waiting for Timer0 Interrupt.\r\n");
  while ( !tmrIntReceived );

  printf("\tINFO: Timer0 interrupt received by DSP, signaling PRU.\n");
  LOCAL_exampleFinalize();

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
    printf("Example completed successfully.\n");
  }
  else
  {
    printf("Example failed.\n");
  }

  /* Make sure PRU disabled/reset */
  PRU_disable();
}


/************************************************************
* Local Function Definitions                                *
************************************************************/

/*
 * DSP interrupt controller configuration.
 */
static void LOCAL_configureDspIntc()
{
  /* clear Timer 0 interrupt */
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC4, SET);

  /* connect the Timer0 event to the DSP interrupt 12 */
  CSL_FINS(intcRegs->INTMUX3, DSPINTC_INTMUX3_INTSEL12, T64P0_TINT12_EVT);

  /* set ISTP to point to the vector table address */
  ISTP = (Uint32)intcVectorTable;

  /* clear all interrupts, bits 4 thru 15 */
  ICR = 0xFFF0;

  /* enable the bits for non maskable interrupt 12 and NMIE*/
  IER = 0x1002;

  /* enable interrupts, set GIE bit */
  _enable_interrupts();
}


/*
 * Interrupt service routines
 */
interrupt void T64P0_TINT12_isr()
{
  tmrIntReceived = 1;
  CSL_FINST(intcRegs->EVTCLR[0], DSPINTC_EVTCLR_EC4, SET);
}

/*
* Example Initialization routine
*/
static void LOCAL_exampleInit ( )
{
  Uint32 *memPtr;
  CSL_PruintcRegsOvly hPruIntc = (CSL_PruintcRegsOvly) CSL_PRUINTC_0_REGS;

  // Make sure interrupt is cleared
  hPruIntc->STATCLRINT1 = 0x00000001;

  // Initialize memory pointer to start of External DDR memory
  memPtr = (Uint32 *)&EXTERNAL_RAM_START;

  //Store Addend and offsets in external memory locations
  memPtr[0] = SCORE;
}

/*
* Example Result verification routine
*/
static Bool LOCAL_examplePassed ( )
{
  Uint32  *memPtr;

  // Initialize memory pointer to start of External DDR memory
  memPtr = (Uint32 *)&EXTERNAL_RAM_START;

  // Read in the example done flag to check if the example completed
  // successfully
  return (memPtr[0] == (SCORE-1));
}

static void LOCAL_exampleFinalize(  )
{
  CSL_PruintcRegsOvly hPruIntc = (CSL_PruintcRegsOvly) CSL_PRUINTC_0_REGS;

  hPruIntc->STATSETINT1 = 0x00000001;
}


/* ======================================================================= */
/*               End of file:   PRU_DSPtoPRU_Interrupt.c                   */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */








