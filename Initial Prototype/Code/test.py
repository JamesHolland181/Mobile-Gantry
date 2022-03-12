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

# Clients
left_motor = mqtt.Client("left_motor")
right_motor = mqtt.Client("right_motor")
motor_speed = mqtt.Client("speed")

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))

def left_motor(client,userdata,msg):
    print("Left: ",str(msg.payload.decode()))
    left_vals.append(msg.payload.decode())
    
def right_motor(client,userdata,msg):
    print("Right: ",str(msg.payload.decode()))
    right_vals.append(msg.payload.decode())
    
def motor_speed(client,userdata,msg):       
    print("Speed: ",str(msg.payload.decode()))
    speed_vals.append(msg.payload.decode())

callbacks = [left_motor,right_motor,motor_speed]

left_motor = mqtt.Client("left_motor")
right_motor = mqtt.Client("right_motor")
motor_speed = mqtt.Client("speed")

clients = [left_motor,right_motor,motor_speed]

while 1:
#     explorerhat.motor.one.backwards(abs(left_vals[-1]))
#     explorerhat.motor.two.backwards(abs(left_vals[-1]))
        
    i=-1
    for client in clients:
        i+=1
#         client.on_connect = on_connect
        topics = ["motor/left","motor/right","motor/speed"]
        
        if i>2:
            i=0
        
        if(client.connect('gantry', 1883) == 0):
            client.subscribe(topics[i])
            client.message_callback_add(topics[i],callbacks[i])
            client.loop_start()
