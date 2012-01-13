import time
import pyinsim
import serial

ARDUINO_PORT = 'com3:'

arduino = serial.Serial(ARDUINO_PORT, 19200, timeout=1)
time.sleep(3)

def outgauge_packet(outgauge, packet):
  speed_kmh = int(packet.Speed*3.6)
  speed_command = 'S {0}\r'.format(speed_kmh)
  arduino.write(speed_command)
  arduino.flushInput()


outgauge = pyinsim.outgauge('127.0.0.1', 30000, outgauge_packet, 30.0)

pyinsim.run()
