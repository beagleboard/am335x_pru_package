.origin 0
.entrypoint START

#include "dmx.hp"

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194

#define DMX_HALT (0x100)
#define DMX_CHANNELS (0x101)
#define DMX_PIN (0x102)

.macro SLEEPUS
.mparam us,inst,lab
    MOV r7, (us*100)-1-inst
lab:
    SUB r7, r7, 1
    QBNE lab, r7, 0
.endm

.macro BRING_LOW
    MOV r6, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r4, r6, 0, 4
.endm

.macro BRING_HIGH
    MOV r6, GPIO1 | GPIO_SETDATAOUT
    SBBO r4, r6, 0, 4
.endm

START:
    // clear that bit
    LBCO r0, C4, 4, 4
    CLR r0, r0, 4
    SBCO r0, C4, 4, 4

   MOV r6, GPIO1 | GPIO_CLEARDATAOUT
   MOV r8, 7<<22
   SBBO r8, r6, 0, 4

LOOP:
    // Exit if the halt flag is set
    MOV r6, DMX_HALT
    LBCO r2, CONST_PRUDRAM, r6, 1
    QBNE EXIT, r2.b0, 0

    // Make DMX pin mask
    MOV r6, DMX_PIN
    LBCO r2, CONST_PRUDRAM, r6, 1
    MOV r4, 1
    LSL r4, r4, r2.b0

    // 1. Bring low for 92 us
    MOV r6, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS 92, 1, START_LOW_SLEEP

    // 2. Bring high for 12 us
    MOV r6, GPIO1 | GPIO_SETDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS 12, 1, START_HIGH_SLEEP

    // 3. Send a DMX data frame of 0 to start the sequence
    MOV r6, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS (9*4), 0, ZERO_START_SLEEP
    MOV r6, GPIO1 | GPIO_SETDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS (2*4), 8, ZERO_STOP_SLEEP

    // 4. Send a DMX data frame for each channel
    MOV r3, 0
    MOV r6, DMX_CHANNELS
    LBCO r5, CONST_PRUDRAM, r6, 1

DMX_LOOP:
//    QBEQ LOOP, r3.b0, 4
    QBEQ LOOP, r3.b0, r5.b0
    LBCO r2, CONST_PRUDRAM, r3, 1
    MOV r1, 8

    QBNE OK, r3.b0, 3
    QBNE OK, r2, 128
    MOV r6, GPIO1 | GPIO_SETDATAOUT
    MOV r8, 7<<22
    SBBO r8, r6, 0, 4
OK:
    
    // bring low to start
    MOV r6, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS 4, 2, DMX_FRAME_START_SLEEP

DMX_FRAME:
    QBBC DMX_FRAME_BIT_LOW, r2, 0

DMX_FRAME_BIT_HIGH:
    MOV r6, GPIO1 | GPIO_SETDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS 4, 5, DMX_FRAME_BIT_SLEEP_1
    QBA DMX_FRAME_BIT_FINISH

DMX_FRAME_BIT_LOW:
    MOV r6, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS 4, 4, DMX_FRAME_BIT_SLEEP_2

DMX_FRAME_BIT_FINISH:
    SUB r1, r1, 1
    LSR r2, r2, 1
    QBNE DMX_FRAME, r1, 0

    MOV r6, GPIO1 | GPIO_SETDATAOUT
    SBBO r4, r6, 0, 4
    SLEEPUS (2*4), 6, DMX_FRAME_END_SLEEP

    ADD r3, r3, 1
    QBA DMX_LOOP

EXIT:
#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
