/* =======================================================================      */
/* PRU_access_const_table.c --PRU Example to demonstrate use of contant tables  */
/*                                                                              */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*******************************************************************

  This example demonstrates use of constant tables
  The example writes an value 0x00000001 in the 1st memory location
  of the exteranl DDR memory using entry 31 of the constant table.
  The entry 31 in the constant table corresponding to the DDR is a
  programmable entry.The example illustrates how the 31st entry of
  the constant table is programmed using the constant table programmable
  pointer register 1(CTPPR_1).
  Using the constant tables the value 0x00000001 is fetched from the DDR
  memory and is placed in the PWREMU_MGMT register in the HPI region. This
  sets the FREE bit of the PWREMU_MGMT. The value in the HPIC is then read
  from the location 0x01E10030 by using an offset(0x30) along with the
  15th entry in the constant table (base address for UHPI configurations).
  Value of HPIC is then stored in the second location of the DDR memory.
  Thus this example illustrates use of both fixed and programmable entries
  in the constant table.

********************************************************************/

/************************************************************
* Include Files                                             *
************************************************************/

#include <stdlib.h>
#include <stdio.h>

// General type include
#include "tistdtypes.h"

// CSL header files
#include "csl/cslr_psc_OMAPL138.h"
#include "csl/soc_OMAPL138.h"

// Device Specific Init Functions
#include "device.h"

// PRU load/run functions
#include "pru.h"


/************************************************************
* Explicit External Declarations                            *
************************************************************/

/* Memory start definitions as in linker command file*/
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM  (0)
#define ADDEND1 0x00000001


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

#include "PRU_access_const_table_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_access_const_table";


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main (void)
{
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

static void LOCAL_exampleInit ( Uint8 pruNum )
{
  Uint32 *memPtr;

  //Initalize memory pointer to start of External DDR memory
  memPtr = (Uint32 *) &EXTERNAL_RAM_START;
  //Load the 1st DDR memory location
  memPtr[0] = ADDEND1;

  // Turn on the HPI peripeheral
  DEVICE_LPSCTransition(CSL_PSC_1,CSL_PSC_UHPI,0,(CSL_PSC_MDCTL_NEXT_ENABLE & 0x1F));
}

static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint32 *memPtr;
  Uint32 result_1;
  Uint32 result_2;
  Uint32 result_3;

  //Initalize memory pointer to HPI region
  memPtr = (Uint32 *) CSL_HPI_0_REGS;

  //Read values from memory to confirm desired results from the example
  result_1 =  memPtr[1];
  result_2 =  memPtr[12];

  memPtr= (Uint32 *) &EXTERNAL_RAM_START;
  result_3 =  memPtr[1];

  // Report success or failure of the example
  return ((result_1 == ADDEND1) && (result_2 == result_3)) ;
}



/* ======================================================================= */
/*                End of file:  PRU_access_const_table.c                   */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */



