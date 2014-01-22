# vim: ts=2:sw=2:tw=80:nowrap

import ctypes

from ptypes import *
from errors import assert_success, PrussDrvError, PRUNOTOPENED


__all__ = [
  'init', 'open', 'version', 'strversion', 'pru_reset', 'pru_disable',
  'pru_enable', 'pruintc_init', 'map_l3mem', 'map_extmem', 'map_prumem',
  'map_peripheral_io', 'get_phys_addr', 'get_virt_addr',  'pru_wait_event',
  'pru_send_event', 'pru_clear_event', 'pru_send_wait_clear_event', 'exit',
  'exec_program', 'start_irqthread',
]


try:
  #load the prussdrv library
  drv = ctypes.CDLL('libprussdrv.so')
except:
  class fakefunc:
    def __call__(self, *args, **kwargs):
      raise PrussDrvError(PRUNOTOPENED)


  class fakelib:
    def __getattr__(self, name):
      if not self.__dict__.has_key(name):
        self.__dict__[name] = fakefunc()

      return self.__dict__[name]
  drv = fakelib()



# ### set function prototypes ###

init                                = drv.prussdrv_init
drv.prussdrv_init.argtypes          = []
drv.prussdrv_init.restype           = assert_success

open                                = drv.prussdrv_open
drv.prussdrv_open.argtypes          = [ c_uint ]
drv.prussdrv_open.restype           = assert_success

version                             = drv.prussdrv_version
drv.prussdrv_version.argtypes       = []
drv.prussdrv_version.restype        = c_int

strversion                          = drv.prussdrv_strversion
drv.prussdrv_strversion.argtypes    = [ c_int ]
drv.prussdrv_strversion.restype     = c_char_p

pru_reset                           = drv.prussdrv_pru_reset
drv.prussdrv_pru_reset.argtypes     = [ c_int ]
drv.prussdrv_pru_reset.restype      = assert_success

pru_disable                         = drv.prussdrv_pru_disable
drv.prussdrv_pru_disable.argtypes   = [ c_uint ]
drv.prussdrv_pru_disable.restype    = assert_success

pru_enable                          = drv.prussdrv_pru_enable
drv.prussdrv_pru_enable.argtypes    = [ c_uint ]
drv.prussdrv_pru_enable.restype     = assert_success

pruintc_init                        = drv.prussdrv_pruintc_init
drv.prussdrv_pruintc_init.argtypes  = [ POINTER(tpruss_intc_initdata) ]
drv.prussdrv_pruintc_init.restype   = assert_success

map_l3mem                           = drv.prussdrv_map_l3mem
drv.prussdrv_map_l3mem.argtypes     = [ POINTER(POINTER(c_ubyte)) ]
drv.prussdrv_map_l3mem.restype      = assert_success

map_extmem                          = drv.prussdrv_map_extmem
drv.prussdrv_map_extmem.argtypes    = [ POINTER(POINTER(c_ubyte)) ]
drv.prussdrv_map_extmem.restype     = assert_success

map_prumem                          = drv.prussdrv_map_prumem
drv.prussdrv_map_prumem.argtypes    = [ c_uint, POINTER(POINTER(c_ubyte)) ]
drv.prussdrv_map_prumem.restype     = assert_success

map_peripheral_io                   = drv.prussdrv_map_peripheral_io
drv.prussdrv_map_peripheral_io.argtypes= [ c_uint, POINTER(POINTER(c_ubyte)) ]
drv.prussdrv_map_peripheral_io.restype = assert_success

get_phys_addr                       = drv.prussdrv_get_phys_addr
drv.prussdrv_get_phys_addr.argtypes = [ POINTER(c_ubyte) ]
drv.prussdrv_get_phys_addr.restype  = c_uint

get_virt_addr                       = drv.prussdrv_get_virt_addr
drv.prussdrv_get_virt_addr.argtypes = [ c_uint ]
drv.prussdrv_get_virt_addr.restype  = POINTER(c_ubyte)

pru_wait_event                      = drv.prussdrv_pru_wait_event
drv.prussdrv_pru_wait_event.argtypes= [ c_uint ] # pru_evtout_num
drv.prussdrv_pru_wait_event.restype = assert_success

pru_send_event                      = drv.prussdrv_pru_send_event
drv.prussdrv_pru_send_event.argtypes= [ c_uint ]
drv.prussdrv_pru_send_event.restype = assert_success

pru_clear_event                     = drv.prussdrv_pru_clear_event
drv.prussdrv_pru_clear_event.argtypes= [ c_uint ] # pru_evtout_num
drv.prussdrv_pru_clear_event.restype = assert_success

pru_send_wait_clear_event           = drv.prussdrv_pru_send_wait_clear_event
drv.prussdrv_pru_send_wait_clear_event.argtypes= [
                                         c_uint, # send_eventnum
                                         c_uint, # pru_evtout_num
                                         c_uint ]# ack_eventnum
drv.prussdrv_pru_send_wait_clear_event.restype = assert_success

exit                                = drv.prussdrv_exit
drv.prussdrv_exit.argtypes          = []
drv.prussdrv_exit.restype           = assert_success

exec_program                        = drv.prussdrv_exec_program
drv.prussdrv_exec_program.argtypes  = [ c_uint, c_char_p ]
drv.prussdrv_exec_program.restype   = assert_success

# NOTE:  This function cannot work if the callback is a python function (and the
# non-pthread capable Cython implementation of python is used).
start_irqthread                     = drv.prussdrv_start_irqthread
drv.prussdrv_start_irqthread.argtypes= [ c_uint,# pru_evtout_num
                                         c_int, # priority
                                         prussdrv_function_handler,
                                       ]
drv.prussdrv_start_irqthread.restype= assert_success
