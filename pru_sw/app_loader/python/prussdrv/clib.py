# vim: ts=2:sw=2:tw=80:nowrap

import ctypes

from ptypes import *
from errors import assert_success, PrussDrvError, PRUNOTOPENED


__all__ = []


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


def prototype( funcname, argtypes=[], restype=assert_success,
               prefix='prussdrv_', G=globals() ):
  func = getattr(drv, prefix + funcname)
  func.argtypes = argtypes
  func.restype  = restype
  G[funcname]   = func
  __all__.append( funcname )


# ### set function prototypes ###
prototype( 'init' )
prototype( 'open',                     [c_uint]             )
prototype( 'version',                  [],        c_int     )
prototype( 'strversion',               [c_int],   c_char_p  )
prototype( 'pru_reset',                [c_uint]             )
prototype( 'pru_disable',              [c_uint]             )
prototype( 'pru_enable',               [c_uint]             )
prototype( 'pru_write_memory',         [c_uint,         # pru_ram_id
                                        c_uint,         # wordoffset
                                        POINTER(c_uint),# memarea
                                        c_uint] )       # bytelength
prototype( 'pruintc_init',             [POINTER(tpruss_intc_initdata)] )
prototype( 'get_event_to_channel_map', [c_uint],  c_short   )
prototype( 'get_channel_to_host_map',  [c_uint],  c_short   )
prototype( 'get_event_to_host_map',    [c_uint],  c_short   )
prototype( 'map_l3mem',                [POINTER(POINTER(c_ubyte))] )
prototype( 'map_extmem',               [POINTER(POINTER(c_ubyte))] )
prototype( 'extmem_size',              [],        c_uint    )
prototype( 'map_prumem',               [c_uint, POINTER(POINTER(c_ubyte))] )
prototype( 'map_peripheral_io',        [c_uint, POINTER(POINTER(c_ubyte))] )
prototype( 'get_phys_addr',            [POINTER(c_ubyte)],  c_uint )
prototype( 'get_virt_addr',            [c_uint],  POINTER(c_ubyte) )
prototype( 'pru_wait_event',           [c_uint],  c_uint    )
prototype( 'pru_send_event',           [c_uint]             )
prototype( 'pru_clear_event',          [c_uint,c_uint]      )
prototype( 'pru_send_wait_clear_event',[c_uint,   # send_eventnum
                                        c_uint,   # host_interrupt
                                        c_uint] ) # ack_eventnum
prototype( 'exit' )
prototype( 'exec_program',             [c_int, c_char_p]    )

# NOTE:  This function cannot work if the callback is a python function (and the
# non-pthread capable Cython implementation of python is used).
# See prussdrv.InterruptHandler for a valid way to use Python as a callback for
# an interrupt handler.
prototype( 'start_irqthread',          [c_uint, # host_interrupt
                                        c_int,  # priority
                                        prussdrv_function_handler] )
