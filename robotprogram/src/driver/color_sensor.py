import ev3dev.ev3 as ev3
import driver.ports as prt

leftVal = 0.0
def getLeft():
    """convenience functions to get sensor value"""
    global leftVal
    leftVal = left.value()
    return leftVal

rightVal = 0.0
def getRight():
    """convenience functions to get sensor value"""
    global rightVal
    rightVal = right.value()
    return rightVal

def update():
    """Update cached values"""
    getLeft()
    getRight()

# load, check connection, set mode
left = ev3.ColorSensor(prt.leftColorSensorPort)
right = ev3.ColorSensor(prt.rightColorSensorPort)
assert left.connected, "Left Color sensor is not connected"
assert right.connected, "Right Color sensor is not conected"
left.mode = left.MODE_RGB_RAW
right.mode = right.MODE_RGB_RAW