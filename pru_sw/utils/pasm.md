pasm(1) -- Assembler for PRU subsystem included in OMAP-L1x8/C674m/AM18xx devices
=================================================================================

## SYNOPSIS

`pasm` [-V#EBbcmLldfz] [-Idir] [-Dname=value] [-Cname] InFile [OutFileBase]

## DESCRIPTION

**pasm** is a command line driven assembler for the Programmable
Real-time execution unit (PRU) of the Programmable Real-time Unit
Subsystem (PRUSS). It is designed to build single executable images
using a flexible source code syntax and a variety of output options.
PASM is available for Windows and Linux. 

##Calling Syntax

The command line syntax to `PASM` is:

`pasm` [-bBcmldexfz] SourceFile [OutFileBasename] [-Dname=value] [-CArrayName]

Note that only the source file SourceFile is required on the command
line. The assembler will default to output option "-c" which generates a
C array containing the binary opcode data. The majority of the option
flags select a variety of output formats.

The output file OutFileBasename is a base name only. It defaults to the
same name as the source file (for example "myprog.p" would have a
default base name of "myprog". Standard filename extensions are applied
to the base name to create the final output filename(s), depending on
the output option(s) selected.

When specifying PASM options, options can be specified individually or
as a group. For example, either of the two invocations below is valid:

    pasm -cdl myprog.p
    pasm -c -d -l myprog.p

Also filenames and options can be mixed for example:

    pasm myprog.p -cdl
    pasm -cd myprog.p -DMYVAL=1 -l 

## OPTIONS

 * `-V#`:
    Specify core version (V0,V1,V2,V3). (Default is V1)

 * `-E`:
    Assemble for big endian core

 * `-B`:
    Create big endian binary output (*.bib)

 * `-b`:
    Create little endian binary output (*.bin)

 * `-c`:
    Create "C array" binary output (*_bin.h)

 * `-m`:
    Create "image" binary output (*.img)

 * `-L`:
    Create annotated source file style listing (*.txt)

 * `-l`:
    Create raw listing file (*.lst)

 * `-d`:
    Create pView debug file (*.dbg)

 * `-f`:
    Create "FreeBasic array" binary output (*.bi)

 * `-z`:
    Enable debug messages

 * `-I`:
    Add the directory dir to search path for #include "filename" type of
    directives (where angled brackets are used instead of quotes).

 * `-D`:
    Set equate "name" to 1 using "-Dname", or to any value using
    "-Dname=value"

 * `-C`:
    Name the C array in "C array" binary output to "name" using "-Cname"


## COPYRIGHT

`pasm` is (C) 2005-2013 by Texas Instruments Inc.

