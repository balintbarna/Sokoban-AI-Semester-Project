import driver.color_sensor as clr
import driver.motor as mtr
import driver.gyro as gyro
import constants as cnst
import control as ctrl
import time

color_last = False # True is black, False is white

def setup_intersection_detection():
    is_above_intersection()

def is_above_intersection():
    global color_last
    # both sensor inputs should be darker than lightest black
    if(clr.leftVal < cnst.BLACK_THRESHOLD_MAX and clr.rightVal < cnst.BLACK_THRESHOLD_MAX):
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
    color_was = color_last
    color_actual = is_above_intersection()

    if(color_was == True and color_actual == False): # if it turned white from black, we say it's intersection
        return True
    else:
        return False

turn_finished_counter = 0
def is_turn_finished():
    """
    Tells if the turn, which was setup with turn_setup(), is completed, by reading the gyro values.
    Completion treshhol needs to be configured properly.
    """ 
    global turn_finished_counter
    diff = abs(gyro.val - ctrl.turn_sp)
    finished = diff < cnst.TURN_OK_ERROR_THRESHOLD
    far_from_finished = diff > cnst.TURN_ACTIVATE_INTEGRAL_THRESHOLD
    if(far_from_finished):
        ctrl.turn_pid._integral = 0
    if(finished):
        turn_finished_counter = turn_finished_counter + 1
    else:
        turn_finished_counter = 0
    return turn_finished_counter == cnst.EXTRA_CYCLE_AFTER_TURN

can_push_timer = time.perf_counter()
def setup_detect_can_push():
    global can_push_timer
    can_push_timer = time.perf_counter()

def is_can_pushed():
    elapsed_time = time.perf_counter() - can_push_timer
    return (elapsed_time > cnst.CAN_PUSH_TIME)


backwards_timer = time.perf_counter()
def setup_detect_go_backwards():
    global backwards_timer
    backwards_timer = time.perf_counter()

# def is_going_backwards_finished():
#     elapsed_time = time.perf_counter() - backwards_timer
#     temp = is_end_of_intersection()
#     if(elapsed_time < cnst.GO_BACK_TRESHOLD_TIME):
#         return False
#     else:
#         return temp

def is_going_backwards_finished():
    elapsed_time = time.perf_counter() - backwards_timer
    if(elapsed_time < cnst.GO_BACK_TRESHOLD_TIME):
        return False
    else:
        return True