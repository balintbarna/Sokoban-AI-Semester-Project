import ev3dev.ev3 as ev3
import driver.ports as prt

# convenience functions to get sensor values
def getLeft():
    return left.value()

def getRight():
    return right.value()

# load, check connection, set mode
left = ev3.ColorSensor(prt.leftColorSensorPort)
right = ev3.ColorSensor(prt.rightColorSensorPort)
assert left.connected, "Left Color sensor is not connected"
assert right.connected, "Right Color sensor is not conected"
left.mode = left.MODE_RGB_RAW
right.mode = right.MODE_RGB_RAW