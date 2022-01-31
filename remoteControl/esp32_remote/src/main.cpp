#include <Arduino.h>

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

int speed = 0;

void setup() {
  // Set input pins mode
  // Joystick
  pinMode(forward,INPUT_PULLUP); pinMode(back,INPUT_PULLUP); pinMode(right,INPUT_PULLUP); pinMode(left,INPUT_PULLUP);  

  // Buttons
  pinMode(a,INPUT_PULLDOWN); pinMode(b,INPUT_PULLDOWN);

  // Set up bluetooth or wifi connection
  Serial.begin(115200);

}

void loop() {
  // Monitor inputs 
  // Joystick
  if(digitalRead(forward) == false){
    left_fwd = true; left_bwd = false;
    Serial.println("fwd");
    sleep(1);
  } 
  else if(digitalRead(back) == false){
    left_fwd = false; left_bwd = true;
    Serial.println("bwd");
    sleep(1);
  }
  if(digitalRead(right) == false){
    right_fwd = true; right_bwd = false;
    Serial.println("right");
    sleep(1);
  }
  else if(digitalRead(left) == false){
    right_fwd = false; right_bwd = true;
    Serial.println("left");
    sleep(1);
  }

  // Buttons
  if(digitalRead(a) == true){speed+=1; Serial.println(speed); sleep(1);} // Increase speed
  if(digitalRead(b) == true){speed-=1; Serial.println(speed); sleep(1);} // Decrease speed
  
  // Print to serial monitor

  // Transmit to RPI

  // sleep(1);
}