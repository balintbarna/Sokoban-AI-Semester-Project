import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro
import constants as cnst
import control as ctrl

color_last = False # True is black, False is white
def is_above_intersection():
    global color_last
    leftLight = clr.getLeft()
    rightLight = clr.getRight()
    # both sensor inputs should be darker than lightest black
    if(leftLight < cnst.BLACK_THRESHOLD_MAX and rightLight < cnst.BLACK_THRESHOLD_MAX):
        # black
        color_last = True
    else:
        # white
        color_last = False
    return color_last

def is_start_of_intersection():
    """
    Tells if the sensors are right at the start of an intersection line.
    Constants need to be configured right to detect blackness.
    """
    global color_last

    color_was = color_last
    color_actual = is_above_intersection()

    if(color_was == False and color_actual == True): # if it turned white from back, we say it's intersection
        return True
    else:
        return False

def is_end_of_intersection():
    """
    Tells if the sensors are right above an intersection.
    Constants need to be configured right to detect blackness.
    """
    global color_last

    color_was = color_last
    color_actual = is_above_intersection()

    if(color_was == True and color_actual == False): # if it turned white from back, we say it's intersection
        return True
    else:
        return False

def is_turn_finished():
    """
    Tells if the turn, which was setup with turn_setup(), is completed, by reading the gyro values.
    Completion treshhol needs to be configured properly.
    """
    setp = ctrl.turn_pid.setpoint
    actual = gyro.get()
    finished = abs(actual - setp) < cnst.TURN_OK_ERROR_THRESHOLD
    return finished

def is_can_pushed():
    return is_start_of_intersection()