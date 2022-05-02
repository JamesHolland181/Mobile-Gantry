// Purpose: This code is responsible for receiving controller inputs, parsing, and sending them to motors
// Author: James C. Holland

#include <Arduino.h>
#include <esp_now.h>
#include "WiFi.h" // 24:0A:C4:62:19:78
#include <Wire.h>
#include "driver/mcpwm.h"

// Variable to store if sending data was successful
String success;

uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x62, 0x19, 0x78};

// Pins
// MOTOR 1
int RPWM_a = 13; // Reverse speed
int LPWM_a = 12; // Forward Speed
int REN_a = 14;  // Reverse enable
int LEN_a = 27;  // Forward enable

// MOTOR 2
int RPWM_b = 26; // Reverse speed
int LPWM_b = 25; // Forward Speed
int REN_b = 33;  // Reverse enable
int LEN_b = 32;  // Forward enable

// Bluetooth
int dir = 0;
int speed = 0;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  bool left_fwd = false;
  bool left_bwd = false;
  bool right_fwd = false;
  bool right_bwd = false;
  String Dir = "";
  int speed = 0;
} struct_message;

// Create a struct_message called received
struct_message received;

esp_now_peer_info_t peerInfo;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&received, incomingData, sizeof(received));
  Serial.print("Command: ");
  Serial.print(received.Dir);Serial.printf(" @ %d",received.speed);
  dir = received.Dir;
  Serial.println();
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }    

  Serial.println(WiFi.macAddress());
  Serial.println("gantryOS Version 2 Initializing");
  Serial.print(" . ");Serial.print(" . ");Serial.println(" . ");

  pinMode(RPWM_a,OUTPUT); pinMode(LPWM_a,OUTPUT); pinMode(REN_a,OUTPUT); pinMode(LEN_a,OUTPUT); pinMode(RPWM_b,OUTPUT); pinMode(LPWM_b,OUTPUT); pinMode(REN_b,OUTPUT); pinMode(LEN_b,OUTPUT);

  mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM1A,RPWM_a); mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM1B,LPWM_a);  
  mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM2A,RPWM_b); mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM2B,LPWM_b);

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 50; // frequency = 50Hz, i.e. for every servo motor time period should be 20ms
  pwm_config.cmpr_a = 0;     // duty cycle of PWMxA = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);  

  // Disable Motors --> Until user input received
  digitalWrite(REN_a,LOW); digitalWrite(REN_b,LOW); digitalWrite(LEN_a,LOW); digitalWrite(LEN_b,LOW);
}@id:ms-vscode.cpptools-extension-pack


void loop() {
    // Message format: [ Direction, Speed ]
    // Direction: 1-Fwd, 2-Bwd, 3-Lt, 4-Rt
    dir = "";

    if(Serial.available()){
      dir = Serial.parseInt();
    }

    switch (dir)
    {
    case "fwd": // Fwd
      digitalWrite(LEN_a,HIGH);
      digitalWrite(LEN_b,HIGH);
      mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
      Serial.printf("Fwd: %d\n",speed);
      break;
    case "bwd": // Bwd
      digitalWrite(REN_a,HIGH);
      digitalWrite(REN_b,HIGH);
      mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
      Serial.printf("Bwd: %d\n",speed);
      break;
    case "lt": // Lt
      digitalWrite(REN_a,HIGH);
      digitalWrite(LEN_b,HIGH);
      mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
      Serial.printf("Left: %d\n",speed);
      break; 
    case "rt": // Rt
      digitalWrite(LEN_a,HIGH);
      digitalWrite(REN_b,HIGH);
      mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
      Serial.printf("Right: %d\n",speed);
      break;                  
    default:
      digitalWrite(LEN_a,LOW);
      digitalWrite(LEN_b,LOW);
      digitalWrite(REN_a,LOW);
      digitalWrite(REN_b,LOW);        
      mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,0);
      Serial.printf("Stop\n");
      break;
    }
  vTaskDelay(pdMS_TO_TICKS(10)); //Add delay, since it takes time for servo to rotate (100ms)
}