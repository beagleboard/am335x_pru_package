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

    MOV r1, 0xf0000000 //Duration in #instructions of program
	
INPUTTEST:
	SET r2, r31.t14
    SUB r1, r1, 1 //Subtract from counter
	QBNE TURNON, r2, 1
    QBNE TURNOFF, r2, 0
	
TURNON:
	SET r30.t15
	QBA INPUTTEST
TURNOFF:
	CLR r30.t15
	QBA INPUTTEST

	

//#ifdef AM33XX
    // Send notification to Host for program completion
    MOV R31.b0, PRU0_ARM_INTERRUPT+16
//#else
//    MOV R31.b0, PRU0_ARM_INTERRUPT
//#endif

HALT