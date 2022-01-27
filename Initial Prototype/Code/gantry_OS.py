import RPi.GPIO as GPIO
from time import sleep

import pygame
from pygame.locals import *
import os, sys
import threading
import time

import XboxController

"""
Joysticks:
X: -1.0 to 1.0 Left to right
Y: -1.0 to 1.0 Down to up
"""

# Global variables
left_vals = [0]
right_vals = [0]

# Call backs to handle when specific inputs are received
def leftStick(value):   
    # Left or right?
    if value < 0:
#         print("Left")
        GPIO.output(32,1)
    elif value > 0:
#         print("Right")
        GPIO.output(32,0)
    else:
        print("")
        
    left_vals.append(abs(int(value*100)))        

def rightStick(value):    
    # Up or down?
    if value < 0:
#         print("Right")
        GPIO.output(29,1)
    elif value > 0:
#         print("Left")
        GPIO.output(29,0)
    else:
        print("")
    
    right_vals.append(abs(int(value*100)))

# Set up motor shield
def Motor(direction, pwm):
    GPIO.setmode(GPIO.BOARD) # For portability as GPIO pins are referred to by their position on the header rather than SOC channel
    GPIO.setup(direction, GPIO.OUT) 
    GPIO.setup(pwm, GPIO.OUT)
    return GPIO.PWM(pwm,1) 

leftMotor = Motor(29, 31)
rightMotor = Motor(32, 33)

# Instantiate controller~
xboxCont = XboxController.XboxController(controllerCallBack = None, joystickNo = 0, deadzone = 0.1, scale = 1, invertYAxis = True)

# Setup callbacks
# Joysticks
xboxCont.setupControlCallback(xboxCont.XboxControls.LTHUMBY, leftStick)
xboxCont.setupControlCallback(xboxCont.XboxControls.RTHUMBY, rightStick)

# main()
try:
    xboxCont.start()
    print("Controller running")

    while 1:
        leftMotor.start(left_vals[-1])
        rightMotor.start(right_vals[-1])
        
        print("PWM: "+str(left_vals[-1])+", "+str(right_vals[-1]))     

# Ctrl-C         
except KeyboardInterrupt:
    print("User killed program")
#error        
except:
    print("Unexpected error:", sys.exc_info()[0])
    raise
    
finally:
    #stop the controller
    xboxCont.stop()
    #reset GPIOs
    leftMotor.stop()
    rightMotor.stop()
    GPIO.cleanup()