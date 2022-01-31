import RPi.GPIO as GPIO
from time import sleep
import explorerhat

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
    left_vals.append((int(value*100)))        

def rightStick(value):        
    right_vals.append((int(value*100)))

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
        if left_vals[-1] < 0:
            explorerhat.motor.one.backwards(abs(left_vals[-1]))
        else:
            explorerhat.motor.one.forwards(left_vals[-1])
            
        if right_vals[-1] < 0:        
            explorerhat.motor.two.backwards(abs(right_vals[-1]))
        else:
            explorerhat.motor.two.forwards(right_vals[-1])
        
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
    #reset motors
    explorerhat.motor.one.forwards(0)
    explorerhat.motor.two.forwards(0)
    GPIO.cleanup()
