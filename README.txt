AM335x PRU PACKAGE

The hardware / software modules and descriptions referred 
to in this package are *NOT SUPPORTED* by Texas Instruments 
(www.ti.com / e2e.ti.com). 

These materials are intended for do-it-yourself (DIY) users 
who want to use the PRU at their own risk without TI support.  
"Community" support is offered at BeagleBoard.org/discuss.
 
-------------------------------------------------------------
This tree contains a modified PRUSS assembler source that
(currently) implements the following undocumented PRUSS V2
instructions:
  XIN, XOUT, XCHG, ZERO, FILL, SXIN, SXOUT and SXCHG.
For documentation see the SPRUH73C (TRM rev. C) document.
Note: the pasm binaries in this tree are old, you'll have
to compile pasm for your platform yourself.
-------------------------------------------------------------

The AM335x PRU Package includes:

Documentation:
   AM335x PRU-ICSS overview slides
   AM18x to AM335x PRU software migration guide
   AM335x PRU Linux Application Driver documentation
   CCS AM335x PRU Debugger training slides
   AM335x_PRU_ICSS gel file for use with CCS AM335x PRU Debugger

PRU software (pru_sw):
   Utils:
      PASM (PRU assembler) binary
      PASM Source code

   Linux PRU userspace driver (app_loader):
      prussdrv.c

   Example applications:
      PRU_memAccess_DDR_PRUsharedRAM
      PRU_memAccessPRUDataRam
      PRU_PRUtoPRUInterrupt   
