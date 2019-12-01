import ev3dev.ev3 as ev3
import driver.ports as prt

# load, check connection, set mode
sensor = ev3.GyroSensor(prt.gyroPort)
assert sensor.connected, "Gyro is not connected"
sensor.mode = sensor.MODE_GYRO_ANG

# check return value decimals, because value is int
dec = sensor.decimals

# print('Gyro units: ' + sensor.units)

# offset will be used to zero the angle value before turning
offset = 0.0

def raw():
    """get raw value as float from sensor"""
    return sensor.value()

val = 0.0
def get():
    """get offseted value, after zeroing"""
    global val
    val = raw() - offset
    return val

def reset():
    """do the offseting/zeroing"""
    global offset
    offset = raw()

def velocity():
    """return the rotational velocity (derivation of angle)"""
    return sensor.rate