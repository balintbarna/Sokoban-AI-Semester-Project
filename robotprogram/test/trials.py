#!/usr/bin/python3.4

import ev3dev.ev3 as ev3
from time import sleep

import signal


btn = ev3.Button()

mA = ev3.LargeMotor('outA')
mB = ev3.LargeMotor('outB')

THRESHOLD_LEFT = 30 
THRESHOLD_RIGHT = 350

BASE_SPEED = 30
TURN_SPEED = 80

#lightSensorLeft = ev3.ColorSensor('in1')
#lightSensorRight = ev3.LightSensor('in2') 

TouchSensor = ev3.TouchSensor('in3')

#assert lightSensorLeft.connected, "LightSensorLeft(ColorSensor) is not connected"
#assert lightSensorRight.connected, "LightSensorRight(LightSensor) is not conected"

assert TouchSensor.connected, "Touch sensor is not connected"

mB.run_direct()
mA.run_direct()



def set_reverse():
    mA.polarity = "inversed"
    mB.polarity = "inversed"

def set_forward():
    mA.polarity = "normal"
    mB.polarity = "normal"

def signal_handler(sig, frame):
	print('Shutting down gracefully')
	mA.duty_cycle_sp = 0
	mB.duty_cycle_sp = 0

	exit(0)

signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C to exit')

def stop():
    ev3.Sound.beep().wait()
    mA.duty_cycle_sp = 0
    mB.duty_cycle_sp = 0
    exit()

set_forward()

while True:
	mA.duty_cycle_sp = BASE_SPEED
	mB.duty_cycle_sp = BASE_SPEED
	tou_val = TouchSensor.value()

	if tou_val > 0:
		stop()

	if btn.any():
		ev3.Sound.beep().wait()
		mA.duty_cycle_sp = 0
		mB.duty_cycle_sp = 0
		exit()
	else:
		print("Touch sensor value: ", tou_val)
#	sensorLeft = lightSensorLeft.value()
#	sensorRight = lightSensorRight.value()

#	print("sensorLeft: ", sensorLeft, " sensorRight: ", sensorRight)
#	if sensorRight < THRESHOLD_RIGHT:
#		mA.duty_cycle_sp = TURN_SPEED
#	else:
#		mA.duty_cycle_sp = BASE_SPEED
	

#	if sensorLeft < THRESHOLD_LEFT:
	
#	else:
	

