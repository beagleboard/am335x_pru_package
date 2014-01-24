/* --------------------------------------------------------------------------
  FILE        : lnk.cmd                                                   
  PROJECT     : OMAP-L1x/C674x PRU Development
  DESC    	  : Linker CMD file for test project

  Version	  : 1.0
  Initial version of the file
  ----------------------------------------------------------------------------- */

-stack          0x00000800 /* Stack Size */  
-heap           0x00000800 /* Heap Size */

MEMORY
{
  DRAM        org=0xC0000000 len=0x04000000 /* DRAM */
  L2_RAM_PROG org=0x11800000 len=0x00010000 /* DSP Internal L2 RAM for program*/
  L2_RAM      org=0x11810000 len=0x00030000 /* DSP Internal L2 RAM */
  SHARED_RAM  org=0x80000000 len=0x00020000 /* Shared L3 */
  AEMIF       org=0x60000000 len=0x02000000 /* AEMIF CS2 region */
  AEMIF_CS3   org=0x62000000 len=0x02000000 /* AEMIF CS3 region */

  /* PRU Memories */
  PRU0_DATA_RAM        : o = 0x01C30000, l = 0x00000200  /*    512B  */
  PRU1_DATA_RAM        : o = 0x01C32000, l = 0x00000200  /*    512 B */
  PRU0_PROG_RAM        : o = 0x01C38000, l = 0x00001000  /*      4KB */
  PRU1_PROG_RAM        : o = 0x01C3C000, l = 0x00001000  /*      4KB */ 
}

SECTIONS
{
  .text :
  {
  } > L2_RAM_PROG
  .const :
  {
  } > L2_RAM_PROG
  .bss :
  {
  } > L2_RAM_PROG
  .far :
  {
  } > L2_RAM_PROG
  .stack :
  {
  } > L2_RAM_PROG
  .data :
  {
  } > L2_RAM_PROG
  .cinit :
  {
  } > L2_RAM_PROG
  .sysmem :
  {
  } > L2_RAM_PROG
  .cio :
  {
  } > L2_RAM_PROG
  .switch :
  {
  } > L2_RAM_PROG

  .l2ram :
  {
    . += 0x00030000;
  } > L2_RAM, type=DSECT, START(_L2_RAM_START), END(_L2_RAM_END), SIZE(_L2_RAM_SIZE)
  
  .ddrram	 :
  {
    . += 0x04000000;
  } > DRAM, type=DSECT, START(_EXTERNAL_RAM_START), END(_EXTERNAL_RAM_END)
  
  .PRU0InstRAM:
  {
    . += 0x1000;
  } load=PRU0_PROG_RAM, type=DSECT, FILL=0x00000000, START(_PRU0_INST_START), SIZE(_PRU0_INST_SIZE)

  .PRU1InstRAM:
  {
    . += 0x1000;
  } load=PRU1_PROG_RAM, type=DSECT, FILL=0x00000000, START(_PRU1_INST_START), SIZE(_PRU1_INST_SIZE)
    
  .PRU0DataRAM :
  {
    . += 0x200;
  } load=PRU0_DATA_RAM, type=DSECT, FILL=0x00000000, START(_PRU0_DATA_START), SIZE(_PRU0_DATA_SIZE)
  
  .PRU1DataRAM :
  {
    . += 0x200;
  } load=PRU1_DATA_RAM, type=DSECT, FILL=0x00000000, START(_PRU1_DATA_START), SIZE(_PRU1_DATA_SIZE)
    
}