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
from external.simple_pid import PID

import constants as cnst
import control as ctrl
import mini_programs as prg
import command_handler as cmd
import state_machine as stm
from driver.shutdown import *

def signal_handler(sig, frame):
	"""Closes program on CTRL+C"""
	shutdown()

signal.signal(signal.SIGINT, signal_handler)
print('Press Ctrl+C to exit')

btn = ev3.Button()
def check_exit_condition():
	"""Checks for exit conditions if the program needs to be stopped. For example button press."""
	tou_val = touch.get()

	if tou_val > 0:
		shutdown()

	if btn.any():
		shutdown()

#  LOGIC  ----------------------------------------

state = "default"

# main control function used for testing right now, called from main loop
def control_main():
	"""
	This is the control function called from the main loop.
	It should not be blocking in the future, but right now it is.
	Control functions are written so that they are not blocking, they return quickly,
	therefore a state machine will need to be implemented to store current state and 
	repeatedly call the non-blocking control functions.
	Right now for testing, we just call the mini programs.
	"""

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

	prg.turn_right_every_time()

def what_direction(lowc):
	if(lowc == 'u'):
		return 0
	if(lowc == 'r'):
		return 1
	if(lowc == 'd'):
		return 2
	if(lowc == 'l'):
		return 3

def get_turn(turn):
	if(turn == 1):
		return cmd.Command.TURN_RIGHT
	if(turn == 2):
		return cmd.Command.TURN_AROUND
	if(turn == 3):
		return cmd.Command.TURN_LEFT

def translate_solution(sol = ""):
	# set starting direction
	d = what_direction(cnst.START_DIRECTION)
	clist = []
	# go through all commands
	sollen = len(sol)
	for i in range (0, sollen):
		c = sol[i]
		lowc = c.lower()
		newd = what_direction(lowc)
		turn = newd - d
		if(turn < 0):
			turn = turn + 4
		if(turn > 0):
			clist.append(get_turn(turn))
			d = newd
		pushing = c != lowc
		clist.append(cmd.Command.GO_STRAIGHT)
		if(pushing):
			nexti = i+1
			if(nexti < sollen and c == sol[nexti]):
				s = 0
			else:
				clist.append(cmd.Command.PUSH_CAN_AND_RETURN)
				clist.append(cmd.Command.TURN_AROUND)
				clist.append(cmd.Command.GO_STRAIGHT)
				d = d + 2
				if d > 3:
					d = d - 4
	return clist

# cmd.cmdlist = cmd.deque([
# cmd.Command.GO_STRAIGHT,
# cmd.Command.TURN_RIGHT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.PUSH_CAN_AND_RETURN,
# cmd.Command.TURN_RIGHT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.TURN_LEFT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.TURN_LEFT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.PUSH_CAN_AND_RETURN,
# cmd.Command.TURN_LEFT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.TURN_LEFT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.GO_STRAIGHT,
# cmd.Command.TURN_AROUND])


cmd.cmdlist = cmd.deque(translate_solution("llllUdrruLdldlluRRRRRdrUUruulldRRdldlluluulldRurDDrdLLdlluRRRRRdrUUruulldRurDurrdLulldddllululDrdLdlluRRRRRdrUUdllulullDrddlluRRRRRdrU"))


# print ("commands")
# for command in cmd.cmdlist:
# 	print (command)

# shutdown()

counter = 0
# main loop
while True:
	if(counter == 50):
		check_exit_condition()
		counter = 0
	# control_main()
	stm.run_states()
	counter = counter + 1
