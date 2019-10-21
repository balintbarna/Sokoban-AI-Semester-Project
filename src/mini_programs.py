import control as ctrl



def go_until_intersection():
    while(ctrl.is_intersection() == False):
        ctrl.line_control()