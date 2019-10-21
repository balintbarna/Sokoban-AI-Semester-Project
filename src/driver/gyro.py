import ev3dev.ev3 as ev3
import driver.ports as prt

# load, check connection, set mode
sensor = ev3.GyroSensor(prt.gyroPort)
assert sensor.connected, "Gyro is not connected"
sensor.mode = sensor.MODE_GYRO_ANG

# check return value decimals, because value is int
dec = sensor.decimals

print('Gyro units: ' + sensor.units)

# offset will be used to zero the angle value before turning
offset = 0.0

# get raw value as float from sensor
def raw():
    return sensor.value() * 1.0 / pow(10, dec)

# get offseted value, after zeroing
def get():
    return sensor.value() - offset

# do the offseting/zeroing
def reset():
    global offset
    offset = raw()

# return the rotational velocity (derivation of angle)
def velocity():
    return sensor.rate