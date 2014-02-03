//=============================================================================
// File: PRU_gpioToggle.p
// Desc: PRU toggling direct connected GPIOs
// Vers: 1.1
//
//  Copyright (c) 2009-2010 Texas instruments,Incorporated
//  All rights reserved
//
//=============================================================================
//  PRU Example to demonstrate toggling direct connected GPIOs (through R31)
//=============================================================================

.origin 0
.entrypoint GPIO_TOGGLE

#include "PRU_gpioToggle.hp"

GPIO_TOGGLE:
    LDI       global.count,	GPIO_BIT
    LDI       global.delay,	0x0

LOOP:
    gpioSet   global.count

LOOP2:
    ADD       global.delay, global.delay, 0x1
    QBGT      LOOP2, global.delay, 0xFF

    gpioClr   global.count

    ADD       global.count, global.count, 0x1
    QBGT      LOOP, global.count, GPIO_COUNT

    HALT