################################################################################
# dmxController.py
# ----------------
# 
# An example script that cycles through each DMX channel, turning each fully on
# one at a time
################################################################################

import socket
import time

# address of the BeagleBone DMX
IP = "127.0.0.1"
PORT = 9930

# number of DMX channels
CHANNELS = 4

# how many seconds to wait between sending a DMX update
DELAY = 0.5

def constructPayload(ch):
  res = "%3d " % (CHANNELS)

  for i in range(CHANNELS):
    x = 255 if i == ch else 0
    res += "%3d " % (x)

  return res

def loop():
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  sock.connect((IP, PORT))

  channel = 0
  while True:
    payload = constructPayload(channel)
    print "payload %s" % (payload)
    sock.send(payload)
    channel = (channel + 1) % CHANNELS
    time.sleep(DELAY)

if __name__ == "__main__":
  loop()
  sock.close()

