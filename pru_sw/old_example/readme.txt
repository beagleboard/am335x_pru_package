This directory contains a collection of examples.  Each example consists
of PRU code and a CCS project used to build, load, and test the PRU
example code from the C674x DSP.

|--\examples
   |
   |--\common                   Contains common files used across multiple examples.
   |
   |--\PRU_access_const_table   Example showing PRU using internal constant table.
   |
   |--\PRU_DSPtoPRU_Interrupt   Example showing the host DSP interrupting the PRU via the INTC.
   |
   |--\PRU_edmaConfig           Example showing the PRU configuring the system EDMA controller.
   |
   |--\PRU_gpioToggle           Example showing the PRU controlling GPIO output via R31.
   |
   |--\PRU_mem1DTransfer        Simple 1-D byte array system memory transfer (DMA type example).
   |
   |--\PRU_memAccessL2          Example showing PRU accessing DSP L2 memory.
   |
   |--\PRU_memAccessL3andDDR    Example showing PRU accessing Shared L3 and External Memory.
   |
   |--\PRU_memAccessPRUDataRam  Example showing the PRU accessing local Data RAM.
   |
   |--\PRU_miscOperations       Example showing various PRU instructions in action.
   |
   |--\PRU_PRUtoDSP_Interrupt   Example showing PRU interrupting the host DSP.
   |
   |--\PRU_PRUtoPRU_Interrupt   Example showing one PRU interrupting the other.
   |
   |--\PRU_pscConfig            Power and Sleep Controller configuration.
   |
   |--\PRU_semaphore            Example showing PRU and DSP sharing memory access via a semaphore.
   |
   |--\PRU_timer0Config         Example showing the PRU configuring a system timer.
   |
   |--\PRU_timer0Interrupt      Example showing the PRU receiving a system event from a system timer.

