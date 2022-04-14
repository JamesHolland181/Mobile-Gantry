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

client = mqtt.Client() # instantiate client
sleep(1)
client.username_pw_set('gantry', 'gantry') # Username and password are both 'gantry' ... vEry seCuRE 
client.on_connect = lambda w,x,y,z: print("Gantry Running") # when client connects, print that it is running

client.subscribe([("motor/left",1),("motor/right",1),("motor/speed",1)])

client.message_callback_add('motor/speed',lambda x, y, msg: speed_vals.append(int(msg)))
client.message_callback_add('motor/left',lambda x, y, msg: left_vals.append(int(msg)))
client.message_callback_add('motor/right',lambda x, y, msg: right_vals.append(int(msg)))

client.connect('gantry', 1883) # connect to MQTT server
client.loop_start()

# main()
try:        
    while 1:
        print("\n\n\nleft: "+str(left_vals[-1]))
        print("right: "+str(right_vals[-1]))
        print("speed: "+str(speed_vals[-1]))
        
        sleep(2)

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