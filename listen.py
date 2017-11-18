import serial
import time
import sys

ser = serial.Serial("/dev/ttyAMA0", 9600, timeout=10)

while 1:
	try:
		text = ser.read(1)
		print (text)
		time.sleep(0.05)
	except:
		print("Timeout")
		continue
