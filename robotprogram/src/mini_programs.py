import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro

import control as ctrl
import detect as dtct


def go_until_intersection():
    """
    This function will make the robot follow the line until it reaches an intersection.
    The function is blocking and will return when the intersection is reached.
    """
    while(dtct.is_end_of_intersection() == False):
        ctrl.line_control()

def turn(set_deg):
    """
    This function will make the robot turn the degrees given as a parameter.
    It will set up the controller, execute the turn and block until the set result is reached (with a set accuracy)
    """
    ctrl.turn_setup(set_deg)
    while(dtct.is_turn_finished() == False):
        ctrl.turn_control()

def turn_right_every_time():
    """
    This is a mini program, where the robot will follow the line until an intersection, then turn right.
    """
    go_until_intersection()
    mtr.stop()
    turn(90)
    mtr.stop()