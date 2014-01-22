# vim: ts=2:sw=2:tw=80:nowrap

PRUNOTOPENED            = -254

errors = {
  PRUNOTOPENED              : 'prussdrv library not opened',
}


class PrussDrvError(RuntimeError):
  def __init__(self, errnum ):
    self.errnum = errnum
    RuntimeError.__init__(
      self, errors.get(errnum,'Unnown prussdrv error: '+str(errnum))
    )

def assert_success(err):
  if err:
    raise PrussDrvError(err)
