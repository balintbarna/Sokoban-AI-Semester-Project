#!/usr/bin/python3.4

import ev3dev.ev3 as ev3
from time import sleep

import driver.color_sensor as clr
import driver.motor as mtr
import driver.touch as touch
import driver.gyro as gyro

import signal

def close():
	mtr.coast()
	print('Shutting down gracefully')
	ev3.Sound.beep().wait()
	exit(0)

def signal_handler(sig, frame):
	close()

signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C to exit')

btn = ev3.Button()

def check_exit_condition():
	tou_val = touch.get()

	if tou_val > 0:
		close()

	if btn.any():
		close()

BASE_SPEED = 30
TURN_SPEED = 80

def control_main():
	leftLight = clr.getLeft()
	rightLight = clr.getRight()
	diff = leftLight - rightLight
	# positive diff means turn left
	modifier = 0.2
	leftSpeed = BASE_SPEED + diff * modifier
	rightSpeed = BASE_SPEED - diff * modifier
	mtr.setDutyLR(leftSpeed, rightSpeed)
	#print(diff)

while True:
	#print('gyro val:'+str(gyro.sensor.value()))
	#print('gyro ang:'+str(gyro.sensor.angle))
	#print('gyro rate:'+str(gyro.sensor.rate))
	control_main()
