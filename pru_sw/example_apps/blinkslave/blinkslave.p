.origin 0
.entrypoint START

#include "blinkslave.hp"

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194

#define LED_OFFSET 22

START:
    // clear that bit
    LBCO r0, C4, 4, 4
    CLR r0, r0, 4
    SBCO r0, C4, 4, 4

//    MOV r7, 7<<22
//    MOV r4, GPIO1 | GPIO_SETDATAOUT
//    SBBO r7, r4, 0, 4

    MOV r7, 7<<22
    MOV r4, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r7, r4, 0, 4

LOOP:
    // Exit this program if the exit flag is cleared
    LBCO r2, CONST_PRUDRAM, 3, 1
    QBEQ EXIT, r2.b0, 0

    MOV r1, 0

    XOR r6, r6, r6
    XOR r7, r7, r7

EACH:
    QBEQ DO_IT_3, r1, 3

DO_IT:
    LBCO r2, CONST_PRUDRAM, r1, 1
    QBNE SET_IT, r2.b0, 0

CLEAR_IT:
    MOV r5, 1
    LSL r5, r5, r1
    LSL r5, r5, LED_OFFSET
    OR r6, r6, r5
    ADD r1, r1, 1
    QBA EACH

SET_IT:
    MOV r5, 1
    LSL r5, r5, r1
    LSL r5, r5, LED_OFFSET
    OR r7, r7, r5
    ADD r1, r1, 1
    QBA EACH


DO_IT_3:
    MOV r4, GPIO1 | GPIO_CLEARDATAOUT
    SBBO r6, r4, 0, 4
    MOV r4, GPIO1 | GPIO_SETDATAOUT
    SBBO r7, r4, 0, 4
    QBA LOOP


EXIT:
#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
#else
    MOV R31.b0, PRU0_ARM_INTERRUPT
#endif

    HALT
