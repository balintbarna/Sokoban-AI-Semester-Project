import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro
import constants as cnst

from external.simple_pid import PID


# MOVE STRAIGHT

line_pid = PID(cnst.LINE_PID_P, cnst.LINE_PID_I, cnst.LINE_PID_D, 0.0) # PID object for line follower
BASE_SPEED = cnst.FORWARD_SPEED
def line_control():
    """
    This function uses the difference between the two color sensor values as the input for a PID controller.
    The output will modify the base speed of the motors to make the robot turn.
    This control, called repeatedly, should make the robot follow the line.
    """
    global line_pid
    diff = clr.leftVal - clr.rightVal
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

turn_pid = PID(cnst.TURN_PID_P, cnst.TURN_PID_I, cnst.TURN_PID_D, 0.0)

def turn_setup(set_deg = 0.0):
    """
    This function sets up the control for the turning.
    It sets the setpoint for the controller (for example 90 degress).
    It resets the controller and zeroes the gyro sensor to prepare.
    This should be called right before you start turning at an intersection.
    """
    global turn_pid
    turn_pid.setpoint = set_deg
    turn_pid.reset()
    gyro.reset()


def turn_control():
    """
    This function is to be called repeatedly until the turn is finished.
    This will use the controller previously set up by calling turn_setup().
    This will turn the robot with an appropriate rate until it is repeatedly called.
    """
    global turn_pid
    val = turn_pid(gyro.val)
    mtr.setDutyLR(val, -val)