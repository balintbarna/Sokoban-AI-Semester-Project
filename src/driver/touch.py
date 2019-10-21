import ev3dev.ev3 as ev3
import driver.ports as prt

# load, check connection
sensor = ev3.TouchSensor(prt.touchPort)
assert sensor.connected, "Button is not connected"

def get():
    """return: >0 if the button is pressed"""
    return sensor.value()