/* =======================================================================      */
/* PRU_semaphore.c -- Semaphore implementation on PRU                           */
/*                                                                              */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*******************************************************************

  This example is a semphore implemented using PRU code and DSP code.
  The example demonstartes how the L2 memory is accessed by PRU and DSP
  in succession usings flags and memory address pointers stored in L3 memory
  The PRU accesses the L2 memory with the information of number of memory
  reads and the address of the source location and stores the values into it`s
  internal memory when the flag is set to 1. Once it finishes accesing the memory
  it resets the flag and stores the source address and the number of reads
  for the DSP to follow.This process continues on till the terminating condition
  of 70 reads from L2 memory is satisfied

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

/* Memory start defintions are made in the linker command file*/
extern __FAR__ Uint32 PRU0_DATA_START, PRU0_DATA_SIZE;
extern __FAR__ Uint32 PRU0_INST_START, PRU0_INST_SIZE;
extern __FAR__ Uint32 PRU1_DATA_START, PRU1_DATA_SIZE;
extern __FAR__ Uint32 PRU1_INST_START, PRU1_INST_SIZE;
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;
extern __FAR__ Uint32 SHARED_RAM_START, SHARED_RAM_SIZE;
extern __FAR__ Uint32 L2_RAM_START, L2_RAM_SIZE;


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM       (0)
//Set read Length and the flag for the memory exchange
#define FLAGINIT      (0x00000001u)
// set the offsets for memory reads and writes
#define READSIZE      (0x00000004u)
#define SRCOFFSET     (0x00000000u)
#define DSTOFFSET     (0x00000004u)
#define READSIZEINCR  (0x00000004u)

#define MAX_XFER_SIZE (70)


/************************************************************
* Local Typedef Declarations                                *
************************************************************/


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit ( );
static Bool LOCAL_examplePassed ( );


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_semaphore_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_semaphore";


/************************************************************
* Global Function Definitions                               *
************************************************************/

void main (void)
{
  Bool semFlag, doneFlag;
  Uint32 read_size;

  Uint8 *ptr_src    = (Uint8 *) &(L2_RAM_START) ;
  Uint8 *ptr_dst    = (Uint8 *) &(EXTERNAL_RAM_START) ;
  Uint32 *sharedMem = (Uint32 *) &(SHARED_RAM_START);

  printf("Starting PRU %s example.\r\n",exampleName);

  // Make sure PRU sub system is first disabled/reset
  PRU_disable();

  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit();
  doneFlag    = (Bool) sharedMem[4];
  semFlag     = (Bool) sharedMem[0];

  while (!doneFlag)
  {
    // Get xfer size
    read_size = sharedMem[1];
    printf("\tINFO: Current Transfer size = %d bytes.\r\n",read_size);

    // Check for terminating condition Number of memory transfer=0x46[d(70)]
    if (read_size >= MAX_XFER_SIZE)
    {
      sharedMem[4] = (Uint32) TRUE;
    }
    else
    {
      read_size += READSIZEINCR;
    }

    // semFlag TRUE implies PRU has the permission to access memory
    if (semFlag)
    {
      printf("\tINFO: Loading and executing transfer in PRU.\r\n");
      // Enable and load the code to the specified pru
      PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(int)));

      //Execute code on PRU
      PRU_run(PRU_NUM);

      // Wait for the PRU to call the HALT command
      if (PRU_waitForHalt(PRU_NUM,-1) != E_PASS)
      {
         printf("\tINFO: PRU halt failed.\r\n");
      }
      else
      {
        printf("\tINFO: PRU completed transfer.\r\n");
      }

      // Disable the PRUSS when done
      PRU_disable();
    }
    // semFlag=0 implies DSP has the permission to access memory
    else
    {
      // Adjust source and destination pointers
      Uint8 *src = ptr_src+sharedMem[2];
      Uint8 *dst = ptr_dst+sharedMem[3];
      Uint32 i=0;
      while (i < read_size)
      {
        dst[i] = src[i];
        i++;
      }
      // Set flag to indicate PRU has access
      sharedMem[0]=TRUE;
      sharedMem[1]=read_size;
      sharedMem[2]+=read_size;
      sharedMem[3]+=read_size;
      printf("\tINFO: DSP completed transfer.\r\n");
    }

    // Get current doneFlag
    doneFlag = (Bool) sharedMem[4];
    // Get current semFlag
    semFlag = (Bool) sharedMem[0];
  }

  // Check to see if the example produced desired result
  if ( LOCAL_examplePassed() )
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

static void LOCAL_exampleInit ( )
{
  Uint32 *sharedMem = (Uint32 *) &SHARED_RAM_START;

  // Init the semFlag
  sharedMem[0] = (Uint32) FLAGINIT;

  // Set memory offsets in the Shared Ram memory locations 1,2,3
  sharedMem[1] = READSIZE;
  sharedMem[2] = SRCOFFSET;
  sharedMem[3] = DSTOFFSET;

  // Reset example done flag
  sharedMem[4]= (Uint32) FALSE;
}

static Bool LOCAL_examplePassed ( )
{
  Uint32 *sharedMem = (Uint32 *) &SHARED_RAM_START;
  Bool exampleDone = (Bool) sharedMem[4];

  // Check if example completed
  return (exampleDone);
}

/* ======================================================================= */
/*          End of file:   PRU_semaphore.c                                 */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */


