#!/usr/bin/python3.4

from time import sleep
import signal

import ev3dev.ev3 as ev3

import driver.color_sensor as clr
import driver.motor as mtr
import driver.touch as touch
import driver.gyro as gyro

from simple_pid import PID

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

pid = PID(0.1)

def control_main():
	leftLight = clr.getLeft()
	rightLight = clr.getRight()
	diff = leftLight - rightLight
	# positive diff means turn left
	# setpoint is 0
	# error is setpoint - input
	# pid output is opposite sign of input
	# positive val means turn right
	val = pid(diff)
	leftSpeed = BASE_SPEED - val
	rightSpeed = BASE_SPEED + val
	mtr.setDutyLR(leftSpeed, rightSpeed)
	#print(diff)

while True:
	check_exit_condition()
	#print('gyro val:'+str(gyro.sensor.value()))
	#print('gyro ang:'+str(gyro.sensor.angle))
	#print('gyro rate:'+str(gyro.sensor.rate))
	control_main()
