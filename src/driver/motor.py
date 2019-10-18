import ev3dev.ev3 as ev3
import driver.ports as prt

def backwards():
    left.polarity = left.POLARITY_INVERSED
    right.polarity = right.POLARITY_INVERSED

def forwards():
    left.polarity = left.POLARITY_NORMAL
    right.polarity = right.POLARITY_NORMAL

def stop():
    setDuty(0)

def setDuty(duty):
    left.duty_cycle_sp = duty
    right.duty_cycle_sp = duty

def setDutyLR(dutyL, dutyR):
    left.duty_cycle_sp = dutyL
    right.duty_cycle_sp = dutyR


left = ev3.LargeMotor(prt.leftMotorPort)
right = ev3.LargeMotor(prt.rightMotorPort)
assert left.connected, "Left motor is not connected"
assert right.connected, "Right motor is not connected"
left.run_direct()
right.run_direct()
forwards()