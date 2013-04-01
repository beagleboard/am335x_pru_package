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

    MOV r0, 1000 //# cycles
	
INPUTTEST:
	WBS r31.t14 //Wait til GPIO-14-in is high...
	SET r30.t15 //...then set GPIO-13-out high.
	WBC r31.t14 //Wait til GPIO-14-in is low...
	CLR r30.t15 //...then set GPIO-13-out low.
	
	SUB r0, r0, 1 //Subtract from counter
	QBNE INPUTTEST, r0, 0 //Loop if counter not at zero
	
//#ifdef AM33XX
	// Send notification to Host for program completion
	MOV R31.b0, PRU0_ARM_INTERRUPT+16
//#else
//    MOV R31.b0, PRU0_ARM_INTERRUPT
//#endif

CLR r30.t15
MOV r0, 0
HALT
	
