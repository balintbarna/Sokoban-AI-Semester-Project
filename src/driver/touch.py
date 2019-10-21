import ev3dev.ev3 as ev3
import driver.ports as prt

# load, check connection
sensor = ev3.TouchSensor(prt.touchPort)
assert sensor.connected, "Button is not connected"

# return if the button is pressed
def get():
    return sensor.value()