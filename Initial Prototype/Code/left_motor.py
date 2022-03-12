#!/usr/bin/env python3
import RPi.GPIO as GPIO
from time import sleep
import explorerhat
import paho.mqtt.client as mqtt

# Global variables
left_vals = [0]

# This is the Subscriber


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("motor/left")

def on_message(client, userdata, msg):   
    if len(msg.payload.decode())>1:
        speed=int(msg.payload.decode())
        leftt_vals.append(speed)
    elif len(msg.payload.decode())==1:
        rot = int(msg.payload.decode())
        if rot==0:
            explorerhat.motor.one.backwards(abs(30))
            print("Right: ",str(rot))
        elif rot>0:
            explorerhat.motor.one.forwards(abs(30))
            print("Right: ",str(rot))
    else:
        print("oops")

try:
    client = mqtt.Client()
    client.connect("gantry",1883,60)

    client.on_connect = on_connect
    client.on_message = on_message

    client.loop_forever()

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
    GPIO.cleanup()
