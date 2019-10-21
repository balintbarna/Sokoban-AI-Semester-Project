import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro

import control as ctrl



def go_until_intersection():
    while(ctrl.is_intersection() == False):
        ctrl.line_control()

def turn(set_deg = 0):
    ctrl.turn_setup(set_deg)
    while(ctrl.is_turn_finished() == False):
        ctrl.turn_control()