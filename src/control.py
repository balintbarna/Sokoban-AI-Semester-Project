import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro

from simple_pid import PID


BASE_SPEED = 60

line_pid = PID(0.2,0.0,0.02) # PID object for line follower
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

def turn(set_deg = 0):
    gyro.reset()
    pid_turn = PID(2.0, 0.0, 0.2, set_deg)
    gyro_val = gyro.get()
    while(abs(gyro_val - set_deg) > 5):
        val = pid_turn(gyro_val)
        mtr.setDutyLR(val, 0 - val)
        gyro_val = gyro.get()

def is_intersection():
    leftLight = clr.getLeft()
    rightLight = clr.getRight()

    if(leftLight < 100 and rightLight < 100):
        return True
    else:
        return False