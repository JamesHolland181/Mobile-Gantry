import multiprocessing as mp

import RPi.GPIO as GPIO
from time import sleep
import explorerhat

import pygame
from pygame.locals import *
import os, sys
import threading
import time

import paho.mqtt.client as mqtt

# Global variables
left_vals = [0]
right_vals = [0]
speed_vals = [0]

# Call backs to handle when specific inputs are received
def motorController(motor,speed):
    if motor == "left":
        print("left")
        if speed < 0:
            explorerhat.motor.one.backwards(abs(speed_vals[-1]))
#             break
        if speed > 0:
            explorerhat.motor.one.forwards(abs(speed_vals[-1]))
#             break

    if motor == "right":
        print("right")
        if speed < 0:
            explorerhat.motor.one.backwards(abs(speed_vals[-1]))
#             break
        if speed > 0:
            explorerhat.motor.one.forwards(abs(speed_vals[-1]))
#             break

def motorSpeed():
    client = mqtt.Client() # instantiate client
    client.username_pw_set('gantry', 'gantry') # Username and password are both 'gantry' ... vEry seCuRE 
    client.on_connect = lambda w,x,y,z: print("Motor Speed Running") # when client connects, print that it is running
    client.on_message = lambda x, y, msg: speed_vals.append(int(msg)) # when messages received, append them to the spee_vals array
    client.connect('gantry', 1883) # connect to MQTT server
    client.subscribe('motor/speed') # subscribe to speed topic
    client.loop_forever() # loop ad infinitum 

def leftMotor():
    client = mqtt.Client()
    client.username_pw_set('gantry', 'gantry')
    client.on_connect = lambda w,x,y,z: print("Left Motor Running")
    client.on_message = lambda x, y, msg: motorController("left",msg)
    client.connect('gantry', 1883)
    client.subscribe('motor/left')
    client.loop_forever()    

def rightMotor():
    client = mqtt.Client()
    client.username_pw_set('gantry', 'gantry')
    client.on_connect = lambda w,x,y,z: print("Right Motor Running")
    client.on_message = lambda x, y, msg: motorController("right",msg)
    client.connect('gantry', 1883)
    client.subscribe('motor/right')
    client.loop_forever()    

# main()
try:
    # Set up parallel
    pool = mp.Pool(processes=3)
        
    while 1:
        pool.apply_async(leftMotor)
        pool.apply_async(rightMotor)
        pool.apply_async(motorSpeed)
        print("Controller running")


        if left_vals[-1] < 0:
            explorerhat.motor.one.backwards(abs(left_vals[-1]))
        else:
            explorerhat.motor.one.forwards(left_vals[-1])
            
        if right_vals[-1] < 0:        
            explorerhat.motor.two.backwards(abs(right_vals[-1]))
        else:
            explorerhat.motor.two.forwards(right_vals[-1])
        
#         print("PWM: "+str(left_vals[-1])+", "+str(right_vals[-1]))
        pool.close()
        pool.join()

# Ctrl-C         
except KeyboardInterrupt:
    print("User killed program")
#error        
except:
    print("Unexpected error:", sys.exc_info()[0])
    raise
    
finally:
    #stop the controller
    #reset motors
    pool.close()
    pool.join()
    explorerhat.motor.one.forwards(0)
    explorerhat.motor.two.forwards(0)
    GPIO.cleanup()

