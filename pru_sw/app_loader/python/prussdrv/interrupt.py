# vim: ts=2:sw=2:tw=80:nowrap

import multiprocessing as mp
from subprocess import Popen

import clib
from constants_simple import PRU_EVTOUT_0, PRU0_ARM_INTERRUPT

class InterruptHandler(mp.Process):
  """
  Base class for an interrupt handler where the __call__ function of the handler
  is called after each event.  Event handlers are executed in their own process.
  if you want to communicate information back to the original process, you will
  have to use interprocess communications (see multiprocessing package).
  """
  def __init__(self, system_event=PRU0_ARM_INTERRUPT, priority=1, *args, **kwargs):
    mp.Process.__init__(self, *args, **kwargs)
    self.system_event = system_event
    self.host_interrupt = clib.get_event_to_host_map( system_event )
    if self.host_interrupt < 0:
      raise LookupError('system event not mapped to a host interrupt line' )
    self.priority = priority
    self.daemon = True
    self.count = 0

  def start(self):
    mp.Process.start(self)
    p = Popen(['chrt', '-ff', '-p', str(self.priority), str(self.pid)])
    p.wait()

  def run(self):
    while True:
      self.count = clib.pru_wait_event( self.host_interrupt )
      self()
      clib.pru_clear_event( self.host_interrupt, self.system_event )

  def __call__(self):
    raise NotImplementedError('Interrupt handlers should define the __call__ function')


class Example(InterruptHandler):
  def __init__(self):
    super(Example,self).__init__()
    self.calls = 0
  def __call__(self):
    self.calls += 1
