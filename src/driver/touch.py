import ev3dev.ev3 as ev3

sensor = ev3.TouchSensor('in3')
assert sensor.connected, "Button is not connected"

def get():
    return sensor.value()