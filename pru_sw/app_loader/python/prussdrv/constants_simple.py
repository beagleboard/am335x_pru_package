# vim: ts=2:sw=2:tw=80:nowrap

# FROM prussdrv.h
NUM_PRU_HOSTIRQS       =  8
NUM_PRU_HOSTS          = 10
NUM_PRU_CHANNELS       = 10
NUM_PRU_SYS_EVTS       = 64

PRUSS0_PRU0_DATARAM    =  0
PRUSS0_PRU1_DATARAM    =  1
PRUSS0_PRU0_IRAM       =  2
PRUSS0_PRU1_IRAM       =  3

PRUSS_V1               =  1 # AM18XX
PRUSS_V2               =  2 # AM33XX

#Available in AM33xx series - begin
PRUSS0_SHARED_DATARAM  =  4
PRUSS0_CFG             =  5
PRUSS0_UART            =  6
PRUSS0_IEP             =  7
PRUSS0_ECAP            =  8
PRUSS0_MII_RT          =  9
PRUSS0_MDIO            = 10
#Available in AM33xx series - end

PRU_EVTOUT_0           =  0
PRU_EVTOUT_1           =  1
PRU_EVTOUT_2           =  2
PRU_EVTOUT_3           =  3
PRU_EVTOUT_4           =  4
PRU_EVTOUT_5           =  5
PRU_EVTOUT_6           =  6
PRU_EVTOUT_7           =  7



# From pruss_intc_mapping.h
AM33XX = True
if AM33XX:
  PRU0_PRU1_INTERRUPT     = 17
  PRU1_PRU0_INTERRUPT     = 18
  PRU0_ARM_INTERRUPT      = 19
  PRU1_ARM_INTERRUPT      = 20
  ARM_PRU0_INTERRUPT      = 21
  ARM_PRU1_INTERRUPT      = 22
else:
  PRU0_PRU1_INTERRUPT     = 32
  PRU1_PRU0_INTERRUPT     = 33
  PRU0_ARM_INTERRUPT      = 34
  PRU1_ARM_INTERRUPT      = 35
  ARM_PRU0_INTERRUPT      = 36
  ARM_PRU1_INTERRUPT      = 37

CHANNEL0                  = 0
CHANNEL1                  = 1
CHANNEL2                  = 2
CHANNEL3                  = 3
CHANNEL4                  = 4
CHANNEL5                  = 5
CHANNEL6                  = 6
CHANNEL7                  = 7
CHANNEL8                  = 8
CHANNEL9                  = 9

PRU0                      = 0
PRU1                      = 1
PRU_EVTOUT0               = 2
PRU_EVTOUT1               = 3
PRU_EVTOUT2               = 4
PRU_EVTOUT3               = 5
PRU_EVTOUT4               = 6
PRU_EVTOUT5               = 7
PRU_EVTOUT6               = 8
PRU_EVTOUT7               = 9

PRU0_HOSTEN_MASK          = 0x0001
PRU1_HOSTEN_MASK          = 0x0002
PRU_EVTOUT0_HOSTEN_MASK   = 0x0004
PRU_EVTOUT1_HOSTEN_MASK   = 0x0008
PRU_EVTOUT2_HOSTEN_MASK   = 0x0010
PRU_EVTOUT3_HOSTEN_MASK   = 0x0020
PRU_EVTOUT4_HOSTEN_MASK   = 0x0040
PRU_EVTOUT5_HOSTEN_MASK   = 0x0080
PRU_EVTOUT6_HOSTEN_MASK   = 0x0100
PRU_EVTOUT7_HOSTEN_MASK   = 0x0200
