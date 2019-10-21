import ev3dev.ev3 as ev3
import driver.ports as prt

sensor = ev3.GyroSensor(prt.gyroPort)
assert sensor.connected, "Gyro is not connected"

sensor.mode = sensor.MODE_GYRO_ANG
dec = sensor.decimals

print('Gyro units: ' + sensor.units)

offset = 0.0

def raw():
    return sensor.value() / pow(10, dec)

def get():
    return sensor.value() - offset

def reset():
    global offset
    offset = raw()

def rate():
    return sensor.rate