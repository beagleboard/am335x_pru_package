/* =======================================================================      */
/* PRU_miscOperations.c --PRU Example to miscellaneous operations like         */
/*               use of masks to extract bits,bubble sorting,thresholding       */
/* Rev 0.0.1   May 29th 2009                                                    */
/* Initial version of the file                                                  */
/*                                                                              */
/* -----------------------------------------------------------------------      */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.               */
/*                           All Rights Reserved.                               */
/* =======================================================================      */

/*******************************************************************

  This example illustrates

  (a)extraction of 4bit values of a 32 bit number using masks
  and bit shift operations. In the example a 32 bit value is placed in source location
  in L3 memory. The PRU reads the source data and extracts 8 four bit datas of a 32 bit number
  and stores the extracted 4 bits(byte rounded) in PRU1 Data memory.

  (b) The array of extracted bits is used as the initial array to demonstrate
  implementation of bubble sort algorithm and the result of the algorithm is stored
  in the DDR memory.

  (c) The sorted array is then thresholded by applying a cut-off value and converted to
  an array of zeros and ones. The subsequent thresholded result is stored in PRU0 Data memory

  (d) The above implementations are also performed in C and the results from the PRU code and
  the C code are compared to determine the success of the example.

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

// Memory locations
//  SOURCE: 0x80000000(L3)
//  DESTINATION for Mask and bit shift operations: 0x01C30000-0x01c30008(PRU0 Data Mem)
//  DESTINATION for Bubble Sort: 0xc0000000-0xc00008(DDR),
//  DESTINATION for Thresholding ouptut:0x01C32000-0x01c32008(PRU1 Data Mem)

/* Memory start defintions are made in the linker command file*/
extern __FAR__ Uint32 EXTERNAL_RAM_START, EXTERNAL_RAM_START_SIZE;
extern __FAR__ Uint32 SHARED_RAM_START, SHARED_RAM_SIZE;


/************************************************************
* Local Macro Declarations                                  *
************************************************************/

#define PRU_NUM       (0)
#define ADDEND1       (0x0FDB5BF2u)
#define MASK          (0x0000000Fu)
#define SHIFT         (0x00000004u)
#define THRESHOLD     (0x08u)

//decimal offset for a 32 bit pointer pointing to DDR memory
#define OFFSET_DDR    (0)
//Memory locations 0x80002000-0x800200C are used for the example
//decimal offset for a 32 bit pointer pointing to L3 memory
#define OFFSET_L3     (0)


/************************************************************
* Local Typedef Declarations                                *
************************************************************/


/************************************************************
* Local Function Declarations                               *
************************************************************/

static void LOCAL_exampleInit ( );
static Bool LOCAL_examplePassed ( Uint8 pruNum );


/************************************************************
* Local Variable Definitions                                *
************************************************************/

#include "PRU_miscOperations_bin.h"


/************************************************************
* Global Variable Definitions                               *
************************************************************/

String exampleName = "PRU_miscOperations";


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
  PRU_load(PRU_NUM, (Uint32*)PRU_Code, (sizeof(PRU_Code)/sizeof(int)));

  printf("\tINFO: Initializing example.\r\n");
  LOCAL_exampleInit();

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
    printf("Example executed succesfully.\n");
  }
  else
  {
    printf("Example failed.\n");
  }

  // Disable the PRUSS when done
  PRU_disable();
}


/************************************************************
* Local Function Definitions                                *
************************************************************/

static void LOCAL_exampleInit ( )
{
  Uint32 *pruDataMem;
  // Initialize memory pointer to start of shared L3 memory
  pruDataMem = (Uint32 *) &SHARED_RAM_START; // SOurce data location
  //Store Addends in DDR memory locations
  pruDataMem[OFFSET_L3] = ADDEND1;

}

static Bool LOCAL_examplePassed ( Uint8 pruNum )
{
  Uint8 *pruDataMem;
  Uint8 tmp,result[8];
  Uint32 temp,temp_mask=MASK;
  Uint8 result_expected[8];
  Bool pass_flag = TRUE;
  Int32 i,j,shift_right=0;

  // (a) Mask and bitshift operations
  // Initialize memory pointer to start of PRU1_DATA memory
  pruDataMem = (Uint8 *) PRU1_DATA_RAM_START;

  for (i=0;i<8;i++)
  {
    //Read result computed from PRU code
    result[i] = pruDataMem[i];

    // Compute expected result in C
    temp= (ADDEND1 & temp_mask);
    result_expected[i]=temp>>shift_right;
    shift_right+=4;
    temp_mask=temp_mask<<SHIFT;

    //Compare the results from the C and PRU code
    pass_flag = pass_flag && (result[i] == result_expected[i]);
  }

  //Report result from comparision
  if (pass_flag)
  {
    printf("Masking and bit_shift operations were executed succesfully\n");
  }
  else
  {
    printf("Masking and bit_shift operations failed\n");
  }

  // (b) Implementation of Bubble sort Algortihm
  // Initialize memory pointer to start of DDR memory
  pruDataMem = (Uint8 *) &EXTERNAL_RAM_START;

  //Read result computed from PRU code
  for (i=0;i<8;i++)
  {
    result[i] = pruDataMem[i];
  }

  //Compute expected result of bubble sort algorithm in C
  for (i=0;i<8;i++)
  {
    for (j=0;j<7;j++)
    {
      if(result_expected[j]>result_expected[j+1])
      {
        tmp = result_expected[j+1];
        result_expected[j+1] = result_expected[j];
        result_expected[j] = tmp;
      }
    }
  }

  //Compute expected result in C
  for(i=0;i<8;i++)
  {
    pass_flag = pass_flag && (result[i] == result_expected[i]);
  }

  //Report result from comparision
  if(pass_flag==1)
  {
    printf("Bubble sort was executed succesfully\n");
  }
  else
  {
    printf("Bubble sort function failed\n");
  }

  // (c) Thresholding
  // Initialize memory pointer to start of PRU0_DATA memory
  pruDataMem = (Uint8 *) PRU0_DATA_RAM_START;
  //Read result computed from PRU code
  for (i=0;i<8;i++)
  {
    result[i] = pruDataMem[i];
  }

  //Compute expected result of Thresholding in C
  for(j=0;j<8;j++)
  {
    if(result_expected[j]> THRESHOLD)
    {
      result_expected[j]=1;
    }
    else
    {
      result_expected[j]=0;
    }
    //Compare the results from the C and PRU code
    pass_flag = pass_flag && (result[j] == result_expected[j]);
  }

  //Report result from comparision
  if(pass_flag)
  {
    printf("Thresholding functionality was executed succesfully\n");
  }
  else
  {
    printf("Thresholding functionality failed\n");
  }

  return pass_flag;// & (result_1 ==  ADDEND2) & (result_2 ==  ADDEND3)) ;
}

/* ======================================================================= */
/*          End of file:   PRU_miscOperations.c                      */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */


