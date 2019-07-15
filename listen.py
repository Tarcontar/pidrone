#!/usr/bin/python
import serial
import time
import sys

ser = serial.Serial(port='/dev/ttyS0', baudrate = 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout=10)

print("Start listening:")
while 1:
	try:
		text = ser.readline() #[:-2].decode().strip()
		if (len(text) > 0):
			print (text)
			time.sleep(0.05)
	except KeyboardInterrupt:
		print("Canceling listen")
		break;
	except: #geht das so? ausprobieren
		print("Timeout")
	#	continue
