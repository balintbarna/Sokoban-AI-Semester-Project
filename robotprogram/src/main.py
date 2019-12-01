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

# interpret solution

def what_direction(lowc = 'u'):
	"""Returns number for direction char, used to determine how much robot should turn"""
	if(lowc == 'u'):
		return 0
	if(lowc == 'r'):
		return 1
	if(lowc == 'd'):
		return 2
	if(lowc == 'l'):
		return 3

def get_turn(turn = 1):
	"""Returns command matching for how much the robot should turn"""
	if(turn == 1):
		return cmd.Command.TURN_RIGHT
	if(turn == 2):
		return cmd.Command.TURN_AROUND
	if(turn == 3):
		return cmd.Command.TURN_LEFT

def translate_solution(sol = ""):
	"""Translates solution string to commands the robot can do"""
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
			last = nexti == sollen
			if(not last and c == sol[nexti]):
				s = 0
			else:
				clist.append(cmd.Command.PUSH_CAN_AND_RETURN)
				if(not last):
					clist.append(cmd.Command.TURN_AROUND)
					clist.append(cmd.Command.GO_STRAIGHT)
				d = d + 2
				if d > 3:
					d = d - 4
	return clist

cmd.cmdlist = cmd.deque(translate_solution("llllUdrruLdldlluRRRRRdrUUruulldRRlddllluuulldRurDDrdLLdlluRRRRRdrUUruulldRurDurrdLulldddllluulDrdLdlluRRRRRdrUUdllluullDrddlluRRRRRdrU"))

# main loop
# runs until there's no more steps and the program comes to a halt
while True:
	stm.run_states()
