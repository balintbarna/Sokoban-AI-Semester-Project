import ev3dev.ev3 as ev3
import driver.motor as mtr

def shutdown():
    """Stop the motors, beep, shut down program"""
    mtr.coast()
    print('Shutting down gracefully')
    ev3.Sound.beep().wait()
    exit(0)