import RPi.GPIO as GPIO
from time import sleep
import explorerhat

import pygame
from pygame.locals import *
import os, sys
import threading
import time

import paho.mqtt.client as mqtt

# Clients
left_motor = mqtt.Client("left_motor")
right_motor = mqtt.Client("right_motor")
motor_speed = mqtt.Client("speed")

clients = [left_motor,right_motor,motor_speed]
mqtt.Client.connected_flag=False
i=-1

# Global variables
left_vals = [0]
right_vals = [0]
speed_vals = [0]

def on_message(client,userdata,msg):
    if client == "left_motor":
        print("Left: ",str(msg.payload.decode("utf-8")))
    elif client == "right_motor":
        print("Right: ",str(msg.payload.decode("utf-8")))
    elif client == "speed":
        print("Speed: ",str(msg.payload.decode("utf-8")))
    else:
        print("here")

# main()
try:
 
    
    while 1:
#         print("\n\n\nleft: "+str(left_vals[-1]))
#         print("right: "+str(right_vals[-1]))
#         print("speed: "+str(speed_vals[-1]))
        
        for client in clients:
            i+=1 
            topics = ["motor/left","motor/right","motor/speed"]
#             client.on_connect = lambda w,x,y,z: print("Running") # when client connects, print that it is running
            client.on_message = on_message
            
            if i>2:
                i=0
            
            if(client.connect('gantry', 1883) == 0):
                client.subscribe(topics[i])
#                 print("Success: "+topics[i])
                client.loop_start()
                      
#         print("here")
        if left_vals[-1] < 0:
            explorerhat.motor.one.backwards(abs(left_vals[-1]))
        else:
            explorerhat.motor.one.forwards(left_vals[-1])
            
        if right_vals[-1] < 0:        
            explorerhat.motor.two.backwards(abs(right_vals[-1]))
        else:
            explorerhat.motor.two.forwards(right_vals[-1])
        
#         print("PWM: "+str(left_vals[-1])+", "+str(right_vals[-1]))

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
    client.loop_stop()
    explorerhat.motor.one.forwards(0)
    explorerhat.motor.two.forwards(0)
    GPIO.cleanup()
