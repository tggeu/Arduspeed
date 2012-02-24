import time
import pyinsim
import serial

ARDUINO_PORT = 'com3:'

arduino = serial.Serial(ARDUINO_PORT, 19200, timeout=1)
time.sleep(3)

def outgauge_packet(outgauge, packet):
  speed_kmh = int(packet.Speed*3.6)
  speed_command = 'S {0}\r'.format(speed_kmh)

  fuel_int = int(packet.Fuel * 100 * 100)
  fuel_commend = 'F {0}\r'.format(fuel_int)

  gear = packet.Gear
  gear_command = 'G {0}\r'.format(gear)

  rpm = int(round(packet.RPM, -2))
  rpm_command = 'R {0}\r'.format(rpm)

  command = speed_command + fuel_commend + gear_command + rpm_command

  arduino.write(command)
  arduino.flushInput()


outgauge = pyinsim.outgauge('127.0.0.1', 30000, outgauge_packet, 30.0)

pyinsim.run()
