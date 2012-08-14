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
.mparam us,lab
    MOV r3, us
lab:
    SUB r3, r3, 1
    QBEQ lab, r3, 0
.endm

START:
    // clear that bit
    LBCO r0, C4, 4, 4
    CLR r0, r0, 4
    SBCO r0, C4, 4, 4

//    MOV r7, 7<<22
//    MOV r4, GPIO1 | GPIO_CLEARDATAOUT
//    SBBO r7, r4, 0, 4

LOOP:
    // Exit if the halt flag is set
    LBCO r2, CONST_PRUDRAM, DMX_HALT, 1
    QBNE EXIT, r2.b0, 0

    // Make DMX pin mask
    LBCO r2, CONST_PRUDRAM, DMX_PIN, 1
    MOV r4, 1
    LSL r4, r4, r2.b0

    // 1. Bring low for 88 us
    SBBO r4, GPIO1, GPIO_CLEARDATAOUT, 4
    SLEEPUS 88, START_LOW_SLEEP

    // 2. Bring high for 8 us
    SBBO r4, GPIO1, GPIO_SETDATAOUT, 4
    SLEEPUS 8, START_HIGH_SLEEP

    // 3. Send a DMX data frame of 0 (may not be important...)
    SBBO r4, GPIO1, GPIO_CLEARDATAOUT, 4
    SLEEPUS (9*4), ZERO_START_SLEEP
    SBBO r4, GPIO1, GPIO_SETDATAOUT, 4
    SLEEPUS (2*4), ZERO_STOP_SLEEP

    // 4. Send a DMX data frame for each channel
    MOV r3, 0
    LBCO r5, CONST_PRUDRAM, DMX_CHANNELS, 1

DMX_LOOP:
    QBEQ LOOP, r3.b0, r5.b0
    LBCO r2, CONST_PRUDRAM, r3, 1
    MOV r1, 8
    
    // bring low to start
    SBBO r4, GPIO1, GPIO_CLEARDATAOUT, 4
    SLEEPUS 4, DMX_FRAME_START_SLEEP

DMX_FRAME:
    QBEQ DMX_FRAME_LOW, r2.t0, 0

DMX_FRAME_BIT_HIGH:
    SBBO r4, GPIO1, GPIO_SETDATAOUT, 4
    QBA DMX_FRAME_BIT_FINISH

DMX_FRAME_BIT_LOW:
    SBBO r4, GPIO1, GPIO_CLEARDATAOUT, 4

DMX_FRAME_BIT_FINISH
    SLEEPUS 4, DMX_FRAME_BIT_SLEEP

    SUB r1, r1, 1
    LSR r2, r2, 1
    QBNE DMX_FRAME, r1, 0

    ADD r5, r5, 1
    QBA DMX_LOOP

EXIT:
#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
