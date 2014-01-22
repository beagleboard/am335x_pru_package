# vim: ts=2:sw=2:tw=80:nowrap

from ptypes import *
from constants_simple import *

def getPRUSS_INTC_INITDATA():
  return tpruss_intc_initdata(
    sysevts_enabled = (
      PRU0_PRU1_INTERRUPT,
      PRU1_PRU0_INTERRUPT,
      PRU0_ARM_INTERRUPT,
      PRU1_ARM_INTERRUPT,
      ARM_PRU0_INTERRUPT,
      ARM_PRU1_INTERRUPT,
      -1 ),
    sysevt_to_channel_map = (
      tsysevt_to_channel_map(PRU0_PRU1_INTERRUPT, CHANNEL1),
      tsysevt_to_channel_map(PRU1_PRU0_INTERRUPT, CHANNEL0),
      tsysevt_to_channel_map(PRU0_ARM_INTERRUPT,  CHANNEL2),
      tsysevt_to_channel_map(PRU1_ARM_INTERRUPT,  CHANNEL3),
      tsysevt_to_channel_map(ARM_PRU0_INTERRUPT,  CHANNEL0),
      tsysevt_to_channel_map(ARM_PRU1_INTERRUPT,  CHANNEL1),
      tsysevt_to_channel_map(-1,-1) ),
    channel_to_host_map = (
      tchannel_to_host_map(CHANNEL0, PRU0),
      tchannel_to_host_map(CHANNEL1, PRU1),
      tchannel_to_host_map(CHANNEL2, PRU_EVTOUT0),
      tchannel_to_host_map(CHANNEL3, PRU_EVTOUT1),
      tchannel_to_host_map(-1,-1) ),
    host_enable_bitmask = (
      #Enable PRU0, PRU1, PRU_EVTOUT0
      PRU0_HOSTEN_MASK |
      PRU1_HOSTEN_MASK |
      PRU_EVTOUT0_HOSTEN_MASK |
      PRU_EVTOUT1_HOSTEN_MASK ),
  )
