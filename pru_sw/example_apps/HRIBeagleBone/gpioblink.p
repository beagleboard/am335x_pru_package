.origin 0
.entrypoint START

#define PRU0_ARM_INTERRUPT 19
#define AM33XX

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194

START:
// clear that bit
    LBCO r0, C4, 4, 4
    CLR r0, r0, 4
    SBCO r0, C4, 4, 4

    MOV r1, 100 //Counts how many cycles have passed, counter for program loop
BLINK:
    SET r30.t15 //Set GPIO13 HIGH
    MOV r0, 0x00f00000 //Counter for delay loop
DELAY:
    SUB r0, r0, 1
    QBNE DELAY, r0, 0
    CLR r30.t15 // set GPIO1_13 low
    MOV r0, 0x00f00000 //Counter for delay loop
DELAY2:
    SUB r0, r0, 1
    QBNE DELAY2, r0, 0
    SUB r1, r1, 1
    QBNE BLINK, r1, 0

//#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
//#else
//    MOV R31.b0, PRU0_ARM_INTERRUPT
//#endif

HALT
