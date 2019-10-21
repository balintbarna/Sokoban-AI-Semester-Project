import ev3dev.ev3 as ev3
import driver.ports as prt

def coast():
    left.stop_action = left.STOP_ACTION_COAST
    right.stop_action = right.STOP_ACTION_COAST
    left.stop()
    right.stop()

def backwards():
    left.polarity = left.POLARITY_INVERSED
    right.polarity = right.POLARITY_INVERSED

def forwards():
    left.polarity = left.POLARITY_NORMAL
    right.polarity = right.POLARITY_NORMAL

def stop():
    setDuty(0)

def setDuty(duty):
    setDutyLR(duty, duty)

def setDutyLR(dutyL, dutyR):
    # this will limit duty values between -100 and 100
    if(abs(dutyL) > 100 or abs(dutyR) > 100):
        ratio = 0.0
        if(abs(dutyL) > abs(dutyR)):
            ratio = dutyL/100
        else:
            ratio = dutyR/100
        
        ratio = abs(ratio)
        dutyL = dutyL / ratio
        dutyR = dutyR / ratio
    
    left.duty_cycle_sp = dutyL
    right.duty_cycle_sp = dutyR

def power():
    left.run_direct()
    right.run_direct()

left = ev3.LargeMotor(prt.leftMotorPort)
right = ev3.LargeMotor(prt.rightMotorPort)
assert left.connected, "Left motor is not connected"
assert right.connected, "Right motor is not connected"

forwards()
power()
stop()