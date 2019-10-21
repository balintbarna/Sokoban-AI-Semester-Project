#!/usr/bin/python3.4

from time import sleep
import signal

import ev3dev.ev3 as ev3

import driver.color_sensor as clr
import driver.motor as mtr
import driver.touch as touch
import driver.gyro as gyro

from simple_pid import PID

import control as ctrl

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

#  LOGIC  ----------------------------------------

def control_main():
	ctrl.line_follow()

while True:
	check_exit_condition()
	#print('gyro val:'+str(gyro.sensor.value()))
	#print('gyro ang:'+str(gyro.sensor.angle))
	#print('gyro rate:'+str(gyro.sensor.rate))
	control_main()
