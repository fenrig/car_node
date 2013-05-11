import signal
import sys
import serial


def signal_handler(signal, frame):
    print("ctrl+c")
    fileobject.write("\nend\n")
    fileobject.close()
    sys.exit()

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
fileobject = open('serial_logger.txt', 'a+')

signal.signal(signal.SIGINT, signal_handler)

while(True):
    line = ser.readline()
    if line != "":
        sys.stdout.write(line)
        fileobject.write(line)

fileobject.close()
