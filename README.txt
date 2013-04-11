AM335x PRU PACKAGE

The hardware / software modules and descriptions referred 
to in this package are *NOT SUPPORTED* by Texas Instruments 
(www.ti.com / e2e.ti.com). 

These materials are intended for do-it-yourself (DIY) users 
who want to use the PRU at their own risk without TI support.  
"Community" support is offered at BeagleBoard.org/discuss.
 
-------------------------------------------------------------

PLEASE NOTE:

This tree contains a modified PRUSS assembler source that
implements a number of additional instructions:

  XIN, XOUT, XCHG, ZERO, FILL, SXIN, SXOUT, SXCHG,
  LOOP & ILOOP.

These are the (very powerfull) PRUSS Version 2 extensions
as implemented on the AM335X silicon.

For documentation see the SPRUH73C (TRM rev. C) document, you'll
need this version! The information was removed from later issues.

The XIN, XOUT, ZERO, FILL and LOOP instructions are confirmed
to be working as documented. The MAC seems to be working too,
albeit with some issues that need to be worked around and will
degrade performance, depending on the application.

Reports on ILOOP, SXIN, SXOUT and SXCHG are welcome!

Warning: the pasm binaries in this tree are old, you'll have
to compile pasm for your platform yourself.

Last updated: 2013-04-11 - Bas Laarhoven.

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
