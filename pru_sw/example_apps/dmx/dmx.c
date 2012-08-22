/*****************************************************************************
* Include Files                                                              *
*****************************************************************************/

#include <stdio.h>

// Driver header file
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

/*****************************************************************************
* Explicit External Declarations                                             *
*****************************************************************************/

/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

#define PRU_NUM 	0

// This pin is pin 3 on the P8 header
#define DMX_PIN (6)
#define DMX_CHANNELS (2)

#define DMX_HALT_ADDR (0x100)
#define DMX_CHANNELS_ADDR (0x101)
#define DMX_PIN_ADDR (0x102)

#define AM33XX

/*****************************************************************************
* Local Typedef Declarations                                                 *
*****************************************************************************/


/*****************************************************************************
* Local Function Declarations                                                *
*****************************************************************************/

static int LOCAL_exampleInit ();

/*****************************************************************************
* Local Variable Definitions                                                 *
*****************************************************************************/


/*****************************************************************************
* Intertupt Service Routines                                                 *
*****************************************************************************/


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

static void *pruDataMem;
static unsigned char *pruDataMem_byte;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

int main (void)
{
    unsigned int ret, i, j, k;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    
    printf("\nINFO: Starting %s example.\r\n", "dmx");
    /* Initialize the PRU */
    prussdrv_init ();		
    
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }
    
    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);

    /* Initialize example */
    printf("\tINFO: Initializing example.\r\n");
    LOCAL_exampleInit();
    
    /* Execute example on PRU */
    printf("\tINFO: Executing example.\r\n");
    prussdrv_exec_program (PRU_NUM, "./dmx.bin");

    // Instead of waiting patiently for the PRU to finish, we're going to screw around with the shared memory and hopefully influence the PRU
    
    for (k = 0; k < 10; k++) {
      for (j = 0; j < 5; j++) {
	for (i = 0; i < 3; i++) {
          pruDataMem_byte[0] = 64;
          usleep(10000);
          pruDataMem_byte[0] = 0;
          usleep(10000);
	}
	for (i = 0; i < 3; i++) {
          pruDataMem_byte[1] = 64;
          usleep(10000);
          pruDataMem_byte[1] = 0;
          usleep(10000);
	}
      }
      for (j = 0; j < 5; j++) {
        for (i = 0; i < 64; i++) {
          pruDataMem_byte[0] = i;
          pruDataMem_byte[1] = 64-i;
          usleep(10000);
        }
        for (i = 0; i < 64; i++) {
          pruDataMem_byte[0] = 64-i;
          pruDataMem_byte[1] = i;
          usleep(10000);
        }
      }
    }
  
    pruDataMem_byte[DMX_HALT_ADDR] = 1;
    
    /* Wait until PRU0 has finished execution */
    printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU0_ARM_INTERRUPT);

    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable (PRU_NUM);
    prussdrv_exit ();

    return(0);

}

/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/

static int LOCAL_exampleInit ()
{  
    int i;

    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, &pruDataMem);
    pruDataMem_byte = (unsigned char*) pruDataMem;

    pruDataMem_byte[DMX_HALT_ADDR] = 0;
    pruDataMem_byte[DMX_CHANNELS_ADDR] = DMX_CHANNELS;
    pruDataMem_byte[DMX_PIN_ADDR] = DMX_PIN;

    for (i = 0; i < DMX_CHANNELS; i++) {
        pruDataMem_byte[i] = 0;
    }

    return(0);
}
