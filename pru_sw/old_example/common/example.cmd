-stack          0x00001000 /* Stack Size */  
-heap           0x00001000 /* Heap Size */

MEMORY
{
   L1D:     o = 11F00000h   l = 00008000h
   L1P:     o = 11E00000h   l = 00008000h
  DRAM        org=0xC0000000 len=0x04000000 /* External DDR */
  L2_PROGRAM  org=0x11830000 len=0x00010000 /* PROGRAM MEM in L2 */
  L2_RAM      org=0x11800000 len=0x00030000 /* DSP Internal L2 RAM */
  SHARED_RAM  org=0x80000000 len=0x00020000 /* Shared L3 */
  AEMIF       org=0x60000000 len=0x02000000 /* AEMIF CS2 region */
  AEMIF_CS3   org=0x62000000 len=0x02000000 /* AEMIF CS3 region */
}

SECTIONS
{
  .vecs :
  {
  } > L2_PROGRAM
  .text :
  {
  } > L2_PROGRAM
  .const :
  {
  } > L2_PROGRAM
  .bss :
  {
  } > L2_PROGRAM
  .far :
  {
  } > L2_PROGRAM
  .stack :
  {
  } > L2_PROGRAM
  .data :
  {
  } > L2_PROGRAM
  .cinit :
  {
  } > L2_PROGRAM
  .sysmem :
  {
  } > L2_PROGRAM
  .cio :
  {
  } > L2_PROGRAM
  .switch :
  {
  } > L2_PROGRAM

  .l2ram :
  {
    . += 0x00030000;
  } > L2_RAM, type=DSECT, START(_L2_RAM_START), END(_L2_RAM_END), SIZE(_L2_RAM_SIZE)
  
    .sharedl3ram region	 :
  {
    . += 04000000;
  } > SHARED_RAM, type=DSECT, START(_SHARED_RAM_START), END(_SHARED_RAM_END)
  
  .ddrram	 :
  {
    . += 0x04000000;
  } load=DRAM, type=DSECT, START(_EXTERNAL_RAM_START), END(_EXTERNAL_RAM_END)
  
}