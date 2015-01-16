/*
 * PRU_PRUtoPRU_Interrupt.c
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/*
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2010-12
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

/*****************************************************************************
* PRU_PRUtoPRU_Interrupt.c
*
* This example illustrates how two PRUs can communicate between each other by
* interrupting each other during a process. In this example code, the PRU0
* configures the PRU INTC registers and connects system event 32 to channel 0
* which in turn is hooked to the host port 0. The PRU0 then generates a system
* event 32 by writing into its R31 register which sends an interrupt to PRU1
* which is polling for it. On receiving the interrupt, the PRU1 performs
* certain functionality and sets an external flag in DDR memory. The PRU1
* completes its task and interrupts PRU0 once the task is done using system
* event 33 by first mapping this system event number to channel 1 and channel
* 1 to host 1 and then writing into its R31 register. The PRU0 polls for the
* interrupt and acknowledges the completion of task by setting another flag
* in DDR memory. The ARM checks the flag values in DDR memory to verify the
* example was successful.
*
*****************************************************************************/


/*****************************************************************************
* Include Files                                                              *
*****************************************************************************/

// Standard header files
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

// Driver header file
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

#define PRU_NUM0	  0
#define PRU_NUM1	  1

#define FLAG1         0x0A
#define FLAG2         0x0B
#define SCORE         0x0000000Au

#define AM33XX

#ifdef AM33XX
    #define DDR_BASEADDR  0x80000000
#else
    #define DDR_BASEADDR  0xC0000000
#endif


/*****************************************************************************
* Explicit External Declarations                                             *
*****************************************************************************/


/*****************************************************************************
* Local Function Declarations                                                *
*****************************************************************************/

static int LOCAL_exampleInit ( );
static unsigned short LOCAL_examplePassed ( unsigned short pruNum );


/*****************************************************************************
* Local Variable Definitions                                                 *
*****************************************************************************/


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

static int mem_fd;
static void *ddrMem;


/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

int main(void)
{
    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

    printf("\nINFO: Starting %s example.\r\n","PRU_PRUtoPRU_Interrupt");
    /* Initialize the PRU */
    prussdrv_init ();

    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }

    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_1);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }

    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);

    /* Initialize example */
    printf("\tINFO: Initializing example.\r\n");
    LOCAL_exampleInit( );

    /* Execute example on PRU */
    printf("\tINFO: Executing example on PRU0.\r\n");
    prussdrv_exec_program (PRU_NUM0, "./PRU_PRU0toPRU1_Interrupt.bin");
    printf("\t\tINFO: Executing example on PRU1.\r\n");
    prussdrv_exec_program (PRU_NUM1, "./PRU_PRU1toPRU0_Interrupt.bin");


    /* Wait until PRU0 has finished execution */
    printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU0 completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

    /* Wait until PRU1 has finished execution */
    printf("\t\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_1);
    printf("\t\tINFO: PRU1 completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);

    /* Check if example passed */
    if ( LOCAL_examplePassed(PRU_NUM1) )
    {
       printf("Example executed succesfully.\r\n");
    }
    else
    {
       printf("Example failed.\r\n");
    }

    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable (PRU_NUM0);
    prussdrv_pru_disable (PRU_NUM1);
    prussdrv_exit ();
    munmap(ddrMem, 0x0FFFFFFF);
    close(mem_fd);

    return(0);
}
/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/

static int LOCAL_exampleInit ()
{
    void *DDR_regaddr;

    /* open the device */
    mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd < 0) {
        printf("Failed to open /dev/mem (%s)\n", strerror(errno));
        return -1;
    }

    /* map the memory */
    ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
    if (ddrMem == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }

    //FLush the flag locations of PRU0 and PRU1
    DDR_regaddr = ddrMem;
    *(unsigned long*) DDR_regaddr = 0x00;

    DDR_regaddr = ddrMem + 0x000000004;
    *(unsigned long*) DDR_regaddr = 0x00;

    return(0);

}

static unsigned short LOCAL_examplePassed ( unsigned short pruNum )
{
    unsigned long result_0, result_1;
    void *DDR_regaddr;

    //Read in the first example done flags
    DDR_regaddr = ddrMem;
    result_0 = *(unsigned long*) DDR_regaddr;

    DDR_regaddr = ddrMem + 0x000000004;
    result_1 = *(unsigned long*) DDR_regaddr;

    //check if Example completed
    return ((result_0 == FLAG1) & (result_1 == FLAG2) ) ;
}
