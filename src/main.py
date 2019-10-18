#!/usr/bin/python3.4

import ev3dev.ev3 as ev3
from time import sleep

import driver.color_sensor as clr
import driver.motor as mtr
import driver.touch as touch
import driver.gyro as gyro

import signal

def close():
	ev3.Sound.beep().wait()
	mtr.stop()
	exit(0)

btn = ev3.Button()


THRESHOLD_LEFT = 30 
THRESHOLD_RIGHT = 350

BASE_SPEED = 30
TURN_SPEED = 80


def signal_handler(sig, frame):
	print('Shutting down gracefully')
	close()

signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C to exit')

mtr.setDuty(BASE_SPEED)
while True:
	
	tou_val = touch.get()

	if tou_val > 0:
		close()

	if btn.any():
		close()



	leftLight = clr.getLeft()
	rightLight = clr.getRight()
	diff = leftLight - rightLight
	# positive diff means turn left
	modifier = 0.2
	leftSpeed = BASE_SPEED + diff * modifier
	rightSpeed = BASE_SPEED - diff * modifier
	mtr.setDutyLR(leftSpeed, rightSpeed)
	print(diff)

