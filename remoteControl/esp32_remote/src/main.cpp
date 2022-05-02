#include <Arduino.h>
#include <esp_now.h>
#include "WiFi.h" // 24:0A:C4:62:29:90
#include <Wire.h>

// Variable to store if sending data was successful
String success;

uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x62, 0x19, 0x78};

// Structure definition of messages to be sent to robotTM
// Must match the receiver structure
typedef struct struct_message {
  bool left_fwd = false;
  bool left_bwd = false;
  bool right_fwd = false;
  bool right_bwd = false;
  int Dir = 0;
  int speed = 0;
} struct_message;

// Create a struct_message called command
struct_message command;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


// Define joystick pins --> All joystick switches act as momentary switches
int back = 2;
int right = 15;
int left = 4;
int forward = 16;

// Define button pins
int plus = 5; 
int minus = 17; 

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
  // Init Serial Monitor
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }    

  Serial.println(WiFi.macAddress());

  Serial.println("Connecting to Gantry V2");
  Serial.print(" . ");Serial.print(" . ");Serial.println(" . ");
  // Set input pins mode
  // Joystick
  pinMode(forward,INPUT_PULLUP); pinMode(back,INPUT_PULLUP); pinMode(right,INPUT_PULLUP); pinMode(left,INPUT_PULLUP);  

  // Buttons
  pinMode(plus,INPUT_PULLDOWN); pinMode(minus,INPUT_PULLDOWN);

  // Set up bluetooth or wifi connection
  Serial.begin(115200);
  Serial.println("System Ready. Enjoy! :D\n\n");
}

void loop() {
  // // Monitor inputs 
  // // Joystick
  if(digitalRead(forward) == LOW){
    command.left_fwd = true; command.left_bwd = false; command.Dir = 1;
    // Serial.println("fwd");
  } 
  else if(digitalRead(back) == LOW){
    command.left_fwd = false; command.left_bwd = true; command.Dir = 2;
    // Serial.println("bwd");
  }
  if(digitalRead(right) == LOW){
    command.right_fwd = true; command.right_bwd = false; command.Dir = 3;
    // Serial.println("right");
  }
  else if(digitalRead(left) == LOW){
    command.right_fwd = false; command.right_bwd = true; command.Dir = 4;
    // Serial.println("left");
  }

  // Buttons
  if(digitalRead(plus) == true){speed+=1; command.speed = speed; } // Increase speed
  if(digitalRead(minus) == true){speed-=1; command.speed = speed; } // Decrease speed
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &command, sizeof(command));

  if (result == ESP_OK) {
    Serial.println("Speed: "+String(speed));
    Serial.println(left_dir+right_dir+speed);
  }
  else {
    // Serial.println("Error sending the data");
  }
  
  delay(1000);
}