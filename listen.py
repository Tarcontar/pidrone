#!/usr/bin/python
import serial
import time
import sys

ser = serial.Serial(port='/dev/serial0', baudrate = 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout=1000)

print("Start listening:")
print (ser.name)
while True:
	#time.sleep(0.5)
	try:
		print (ser.readline())
	except KeyboardInterrupt:
		print "Canceling listen"
		break
	except: #geht das so? ausprobieren
		#print "Timeout"
		print sys.exc_info()[0]
		continue

ser.close()
