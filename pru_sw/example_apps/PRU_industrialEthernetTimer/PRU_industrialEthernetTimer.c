/* @file PRU_industrialEthernetTimer.h
 * @brief A am335x routine to load in the PRU a binary file to
 *       test the Industrial Ethernet Timer (32-bit)
 *
 * @author Igor Borges Tavares (igorborgest@gmail.com)
 * @bug No know bugs.
 */

#include <stdio.h>
#include "prussdrv.h"
#include <pruss_intc_mapping.h>

// Delay in microseconds that PRU will expect to trigger the interruption in
// the Cortex main processor
#define DELAY_US 10000000u // Max. value = 21474836 us

// Convertion of the DELAY_US in PRU timer ticks
// PRU timer tick frequency = 200 MHz
// PRU timer tick period    =   5 ns
// Number of ticks (TICKS)  = (DELAY_US * 1000) / 5
#define TICKS ((DELAY_US / 5u) * 1000u)

// PRU number can be 0 or 1
#define PRU_NUM 1
// Path to binary that will be load in PRU
#define PRU_BINARY "./obj/PRU_industrialEthernetTimer.bin"

// Local function
static void write_pru_shared_mem ( void);

int main(void)
{
        unsigned int ret;
        tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

        printf("\t-> initializing the PRU\n");
        prussdrv_init();
        ret = prussdrv_open(PRU_EVTOUT_1); // Open PRU Interrupt
        if (ret) {
                printf("\t* prussdrv_open open failed!\n");
                return (ret);
        }
        prussdrv_pruintc_init( &pruss_intc_initdata);
        write_pru_shared_mem ();
        prussdrv_exec_program (PRU_NUM, PRU_BINARY); // Load/exec the bin in PRU
        printf("\t-> waiting PRU event...\n");
        printf("\t-> This should take nearly %u us...\n", (unsigned int)DELAY_US);
        prussdrv_pru_wait_event (PRU_EVTOUT_1);
        printf("\t-> PRU event received.\n");
        printf("\t-> closing the program...\n");
        prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
        prussdrv_pru_disable(PRU_NUM);
        prussdrv_exit ();
        return 0;
}

// Function to map and store the TICKS in the PRUs shared memory
static void write_pru_shared_mem ( void)
{
        void *sharedMem;
        unsigned int *sharedMem_int;

        prussdrv_map_prumem( 4, &sharedMem); // Map shared PRUs memory
        sharedMem_int = (unsigned int*) sharedMem;
        // Writes in the PRUs shared memory in the first addr
        sharedMem_int[0] = (unsigned int)TICKS;
}
