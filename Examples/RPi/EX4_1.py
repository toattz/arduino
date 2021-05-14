import RPi.GPIO as GPIO
import time
import moving
def flashing(pin):
    for i in range(10):
        GPIO.output(pin, True)
        time.sleep(0.05)
        GPIO.output(pin, False)
        time.sleep(0.05)        
def keepRecord(flag,no):
    fid = open("log.txt", "a+")
    if flag == 1:
        fid.write("One car entering at ")
    else:
        fid.write("One car exiting at ")
    fid.write(time.asctime()+"\n")
    fid.write("Total no of cars = "+str(no)+"\n")
    fid.close()  
servoPin1 	= 12
servoPin2 	= 13
enterPin 	= 17
exitPin 	= 4
enterLEDPin = 16
exitLEDPin  	= 20
maxDebounce 	= 500
debounce1 = 0
debounce2 = 0
dutyCycle = [2.5, 7.5]
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPin1, GPIO.OUT)
GPIO.setup(servoPin2, GPIO.OUT)
GPIO.setup(enterLEDPin, GPIO.OUT, initial=False)
GPIO.setup(exitLEDPin, GPIO.OUT, initial=False)
GPIO.setup(enterPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(exitPin, GPIO.IN)
servo1 = GPIO.PWM(servoPin1, 50)
servo2 = GPIO.PWM(servoPin2, 50)
servo1.start(0)
servo2.start(0)
time.sleep(1)
fid = open("log.txt", "w")
fid.write("Parking lot counting system\n")
fid.close()
no = 0
try:
    while 1:
        carEnter = GPIO.input(enterPin)
        if carEnter == 0:
            if debounce1 > maxDebounce:
                debounce1 = 0
                flashing(enterLEDPin)
                GPIO.output(enterLEDPin, True)
                moving.toTarget(servo1, dutyCycle[0], dutyCycle[1])
                flashing(enterLEDPin)
                GPIO.output(enterLEDPin, True)
                moving.toTarget(servo1, dutyCycle[1], dutyCycle[0])
                GPIO.output(enterLEDPin, False)
                servo1.ChangeDutyCycle(0)
                no = no + 1
                print("Total no of cars in the parking lot: ");
                print(no)
                keepRecord(1,no)
                while GPIO.input(enterPin) == 0:
                    pass
            else:
                debounce1 += 1     
        carExit = GPIO.input(exitPin)
        if carExit == 0:
            if debounce2 > maxDebounce:
                debounce2 = 0
                flashing(exitLEDPin)
                GPIO.output(exitLEDPin, True)
                moving.toTarget(servo2, dutyCycle[0], dutyCycle[1])
                flashing(exitLEDPin)
                GPIO.output(exitLEDPin, True)
                moving.toTarget(servo2, dutyCycle[1], dutyCycle[0])
                GPIO.output(exitLEDPin, False)
                servo2.ChangeDutyCycle(0)
                no = no - 1
                print("Total no of cars in the parking lot: ");
                print(no)
                keepRecord(-1,no)
                while GPIO.input(exitPin) == 0:
                    pass
            else:
                debounce2 += 1
except KeyboardInterrupt:
    print("Exit!")
finally:
    GPIO.cleanup()
