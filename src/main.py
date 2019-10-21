#!/usr/bin/python3.4

# basics
from time import sleep
import signal

# api
import ev3dev.ev3 as ev3

# drivers
import driver.color_sensor as clr
import driver.motor as mtr
import driver.touch as touch
import driver.gyro as gyro

# pid controller class
from simple_pid import PID

import control as ctrl
import mini_programs as prg

def close():
	"""Stop the motors, beep, shut down program"""
	mtr.coast()
	print('Shutting down gracefully')
	ev3.Sound.beep().wait()
	exit(0)

def signal_handler(sig, frame):
	"""Closes program on CTRL+C"""
	close()

signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C to exit')

btn = ev3.Button()
def check_exit_condition():
	"""Checks for exit conditions if the program needs to be stopped. For example button press."""
	tou_val = touch.get()

	if tou_val > 0:
		close()

	if btn.any():
		close()

#  LOGIC  ----------------------------------------

state = "default"

# main control function used for testing right now, called from main loop
def control_main():
	# global state
	# if(ctrl.detect_intersection()):
	# 	state = "stop"
	# if(state == "default" or state == "go-straight"):
	# 	ctrl.line_follow()
	# if(state == "stop"):
	# 	mtr.stop()
	# 	sleep(1)
	# 	state = "turn-right"
	# if(state == "turn-right"):
	# 	ctrl.turn(90)
	# 	state = "go-straight"

	prg.go_until_intersection()
	mtr.stop()
	prg.turn(90)
	mtr.stop()


# main loop
while True:
	check_exit_condition()
	#print('gyro val:'+str(gyro.sensor.value()))
	#print('gyro ang:'+str(gyro.sensor.angle))
	#print('gyro rate:'+str(gyro.sensor.rate))
	control_main()
