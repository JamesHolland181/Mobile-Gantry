#include <Arduino.h>
#include "BluetoothSerial.h"

// Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Define joystick pins
int back = 2;
int right = 15;
int left = 4;
int forward = 16;

// Define button pins
int a = 5; 
int b = 17; 

// Command variables
bool left_fwd = false;
bool left_bwd = false;
bool right_fwd = false;
bool right_bwd = false;

// Message formatting
int left_dir = 0;
int right_dir = 0;
int speed = 0;

void setup() {
  Serial.println("gantryOS Version 1 Initializing");
  Serial.print(" . ");Serial.print(" . ");Serial.println(" . ");
  // Set input pins mode
  // Joystick
  pinMode(forward,INPUT_PULLUP); pinMode(back,INPUT_PULLUP); pinMode(right,INPUT_PULLUP); pinMode(left,INPUT_PULLUP);  

  // Buttons
  pinMode(a,INPUT_PULLDOWN); pinMode(b,INPUT_PULLDOWN);

  // Set up bluetooth or wifi connection
  Serial.begin(115200);

  SerialBT.begin("gantryController"); //Bluetooth device name
  Serial.println("System Ready. Enjoy! :D\n\n\n\n\n\n\n\n");
}

void loop() {
  // Monitor inputs 
  // Joystick
  if(digitalRead(forward) == false){
    left_fwd = true; left_bwd = false;
    // Serial.println("fwd");
    sleep(1);
  } 
  else if(digitalRead(back) == false){
    left_fwd = false; left_bwd = true;
    // Serial.println("bwd");
    sleep(1);
  }
  if(digitalRead(right) == false){
    right_fwd = true; right_bwd = false;
    // Serial.println("right");
    sleep(1);
  }
  else if(digitalRead(left) == false){
    right_fwd = false; right_bwd = true;
    // Serial.println("left");
    sleep(1);
  }

  // Buttons
  if(digitalRead(a) == true){speed+=1; sleep(1);} // Increase speed
  if(digitalRead(b) == true){speed-=1; sleep(1);} // Decrease speed
  
  // Print to serial monitor
  // Serial.println("Speed: "+String(speed));

  // Transmit to RPI
  if (SerialBT.available()) {
    Serial.write(left_dir+right_dir+speed);
  }

  Serial.println(left_dir+right_dir+speed);

  // sleep(1);
}