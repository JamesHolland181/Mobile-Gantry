#!/usr/bin/env python3
import RPi.GPIO as GPIO
from time import sleep
import explorerhat
import paho.mqtt.client as mqtt

# Global variables
rot = 0
speed_vals = [0]


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(("motor/right",0),("motor/speed",0))

def get_rot(client, userdata, msg):   
#     if len(msg.payload.decode())>1:
#         speed=int(msg.payload.decode())
#         speed_vals.append(speed)
#     el
    if len(msg.payload.decode())==1:
        rot = int(msg.payload.decode())
        if rot==0:
            explorerhat.motor.two.backwards(abs(speed_vals[-1]))
            print("Right: ",str(rot))
        elif rot>0:
            explorerhat.motor.two.forwards(abs(speed_vals[-1]))
            print("Right: ",str(rot))
    else:
        print("oops")
        
def speed_control(client, userdata, msg):
    if len(msg.payload.decode())>1:
        speed_vals.append(int(msg.payload.decode()))
        if rot==0:
            explorerhat.motor.two.backwards(abs(speed_vals[-1]))
            print("Speed: ",str(abs(speed_vals[-1])))
        elif rot>0:
            explorerhat.motor.two.forwards(abs(speed_vals[-1]))
            print("Speed: ",str(abs(speed_vals[-1])))
    else:
        print("oops")    

try:
    client = mqtt.Client()
    client.connect("gantry",1883,60)

    client.on_connect = on_connect
    client.on_message = get_rot
    client.message_callback_add("motor/speed",speed_control)
    client.message_callback_add("motor/left",get_rot)

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
    explorerhat.motor.two.forwards(0)
    GPIO.cleanup()
