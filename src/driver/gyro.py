import ev3dev.ev3 as ev3
import driver.ports as prt

sensor = ev3.GyroSensor(prt.gyroPort)
assert sensor.connected, "Gyro is not connected"

sensor.mode = 'GYRO-ANG'

print('Gyro units: ' + sensor.units)

offset = 0.0

def raw():
    return sensor.value()

def get():
    return sensor.value() - offset

def reset():
    offset = raw()