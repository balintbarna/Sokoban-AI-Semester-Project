import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro

from simple_pid import PID


# MOVE STRAIGHT

BASE_SPEED = 60
line_pid = PID(0.2,0.0,0.02,0.0) # PID object for line follower

def line_control():
    global line_pid
    leftLight = clr.getLeft()
    rightLight = clr.getRight()
    diff = leftLight - rightLight
	# positive diff means turn left
	# setpoint is 0
	# error is setpoint - input
	# pid output is opposite sign of input
	# positive val means turn right
    val = line_pid(diff)
    leftSpeed = BASE_SPEED - val
    rightSpeed = BASE_SPEED + val
    mtr.setDutyLR(leftSpeed, rightSpeed)


# TURNING

turn_pid = PID(2.0, 0.0, 0.2, 0.0)

def turn_setup(set_deg = 0.0):
    global turn_pid
    turn_pid.setpoint = set_deg
    turn_pid.reset()
    gyro.reset()

def turn_control():
    global turn_pid
    val = turn_pid(gyro.get())
    mtr.setDutyLR(val, 0 - val)


# CHECKER

def is_intersection():
    leftLight = clr.getLeft()
    rightLight = clr.getRight()

    if(leftLight < 100 and rightLight < 100):
        return True
    else:
        return False

def is_turn_finished():
    global turn_pid
    setp = turn_pid.setpoint
    actual = gyro.get()
    finished = abs(actual - setp) < 8
    return finished