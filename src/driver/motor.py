import ev3dev.ev3 as ev3

def backwards():
    left.polarity = "inversed"
    right.polarity = "inversed"

def forwards():
    left.polarity = "normal"
    right.polarity = "normal"

def stop():
    setDuty(0)

def setDuty(duty):
    left.duty_cycle_sp = duty
    right.duty_cycle_sp = duty

def setDutyLR(dutyL, dutyR):
    left.duty_cycle_sp = dutyL
    right.duty_cycle_sp = dutyR


left = ev3.LargeMotor('outB')
right = ev3.LargeMotor('outA')
left.run_direct()
right.run_direct()
forwards()