import ev3dev.ev3 as ev3

def getLeft():
    return left.value()

def getRight():
    return right.value()

left = ev3.ColorSensor('in1')
right = ev3.ColorSensor('in4')
assert left.connected, "Left Color sensor is not connected"
assert right.connected, "Right Color sensor is not conected"
left.mode = 'RGB-RAW'
right.mode = 'RGB-RAW'
