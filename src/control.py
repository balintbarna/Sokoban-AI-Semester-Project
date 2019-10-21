import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro

from simple_pid import PID


# MOVE STRAIGHT

BASE_SPEED = 60
line_pid = PID(0.2,0.0,0.02,0.0) # PID object for line follower

# this function uses the difference between the two color sensor values as the input for a PID controller
# the output will modify the base speed of the motors to make the robot turn
# this control should make the robot go straight on the line
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

# this function sets up the control for the turning
# it sets the setpoint for the controller (for example 90 degress)
# it resets the controller and zeroes the gyro sensor to prepare
def turn_setup(set_deg = 0.0):
    global turn_pid
    turn_pid.setpoint = set_deg
    turn_pid.reset()
    gyro.reset()

# this function is to be called repeatedly until the turn is finished
# this will use the controller previously set up by calling turn_setup()
# this will turn the robot with an appropriate rate until it is repeatedly called
def turn_control():
    global turn_pid
    val = turn_pid(gyro.get())
    mtr.setDutyLR(val, 0 - val)

# CHECKER

# tells if the sensors are right above an intersection
# constants need to be configured right to detect blackness
def is_intersection():
    leftLight = clr.getLeft()
    rightLight = clr.getRight()

    if(leftLight < 100 and rightLight < 100):
        return True
    else:
        return False

# tells if the turn, which was setup with turn_setup(), is completed, by reading the gyro values
# completion treshhol needs to be configured properly
def is_turn_finished():
    global turn_pid
    setp = turn_pid.setpoint
    actual = gyro.get()
    finished = abs(actual - setp) < 8
    return finished