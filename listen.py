#!/usr/bin/python
import serial
import time
import sys

ser = serial.Serial(port='/dev/ttyS0', baudrate = 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout=0)

print("Start listening:")
print (ser.name)
while True:
	#time.sleep(0.5)
	try:
		print (ser.read(1))
		#text = ser.readline() #.decode().strip() #[:-2].strip()
		#print len(text)
		#print text.decode('utf-8')
		#if (len(text) > 0):
		#	print len(text) + " -" + text + "-"
			#print text
		#else:
		#	print "#"
	except KeyboardInterrupt:
		print "Canceling listen"
		break
	except: #geht das so? ausprobieren
		#print "Timeout"
		print sys.exc_info()[0]
		continue

ser.close()
