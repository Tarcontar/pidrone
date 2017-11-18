import serial
import time
import sys

ser = serial.Serial("/dev/ttyAMA0", 9600, timeout=10)

while 1:
	try:
		text = ser.readlin()[:-2].decode().strip()
		if (len(text) > 0):
			print (text)
		time.sleep(0.05)
	except:
		continue
