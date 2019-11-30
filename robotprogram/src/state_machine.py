import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro
import constants as cnst

import control as ctrl
import detect as dtct
from command_handler import cmdlist
import command_handler as cmd
from command_handler import Command as Cmd
from driver.shutdown import *

from enum import Enum

class States(Enum):
    TURNING = 1
    FORWARD = 2
    DEFAULT = 3
    STOPPING = 4
    BACKWARD = 5
    PUSHING_CAN = 6

act_st = States.DEFAULT

def setup_next_command():
    global act_st
    if(len(cmd.cmdlist) > 0):
        command = cmd.cmdlist.popleft()
        print("Next command: " + repr(command))
        if(command == Cmd.GO_STRAIGHT):
            act_st = States.FORWARD
        elif(command == Cmd.TURN_RIGHT):
            ctrl.turn_setup(90)
            act_st = States.TURNING
        elif(command == Cmd.TURN_LEFT):
            ctrl.turn_setup(-90)
            act_st = States.TURNING
        elif(command == Cmd.TURN_AROUND):
            ctrl.turn_setup(180)
            act_st = States.TURNING
        elif(command == Cmd.PUSH_CAN_AND_RETURN):
            dtct.setup_detect_can_push()
            act_st = States.PUSHING_CAN
        else:
            print("Error, unrecognized command")
            shutdown()
    else:
        print("No command, stopping...")
        shutdown()
        

def run_states():
    global act_st
    if(act_st == States.FORWARD):
        if(dtct.is_end_of_intersection() == False):
            ctrl.line_control()
        else:
            setup_next_command()

    elif(act_st == States.TURNING):
        if(dtct.is_turn_finished() == False):
            ctrl.turn_control()
        else:
            setup_next_command()

    elif(act_st == States.PUSHING_CAN):
        if(dtct.is_can_pushed() == False):
            ctrl.line_control()
        else:
            dtct.setup_detect_go_backwards()
            act_st = States.BACKWARD

    elif(act_st == States.BACKWARD):
        if(dtct.is_going_backwards_finished() == False):
            # mtr.backwards()
            # ctrl.line_control()
	        mtr.setDuty(cnst.BACKWARD_SPEED)
        else:
            # mtr.forwards()
            # setup_next_command()
            act_st = States.FORWARD
    
    elif(act_st == States.STOPPING):
        mtr.stop()
        setup_next_command()
    else:
        setup_next_command()

