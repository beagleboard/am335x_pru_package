# vim: ts=2:sw=2:tw=80:nowrap

import ctypes
from ctypes import \
  c_int, c_uint, c_short, c_ushort, \
  c_uint8, c_uint16, c_uint32, c_uint64, \
  c_byte, c_ubyte, c_char, c_char_p, c_void_p, POINTER

from constants_simple import *


prussdrv_function_handler = ctypes.CFUNCTYPE(c_void_p, c_void_p)

class tsysevt_to_channel_map(ctypes.Structure):
  _fields_ = [ ('sysevt', c_short), ('channel', c_short) ]

class tchannel_to_host_map(ctypes.Structure):
  _fields_ = [ ('channel', c_short), ('host',    c_short) ]

class tpruss_intc_initdata(ctypes.Structure):
  _fields_ = [
    #Enabled SYSEVTs - Range:0..63
    #{-1} indicates end of list
    ('sysevts_enabled', c_byte * NUM_PRU_SYS_EVTS),
    #SysEvt to Channel map. SYSEVTs - Range:0..63 Channels -Range: 0..9
    #{-1, -1} indicates end of list
    ('sysevt_to_channel_map', tsysevt_to_channel_map * NUM_PRU_SYS_EVTS),
    #Channel to Host map.Channels -Range: 0..9  HOSTs - Range:0..9
    #{-1, -1} indicates end of list
    ('channel_to_host_map', tchannel_to_host_map * NUM_PRU_CHANNELS),
    #10-bit mask - Enable Host0-Host9 {Host0/1:PRU0/1, Host2..9 : PRUEVT_OUT0..7)
    ('host_enable_bitmask', c_uint),
  ]
