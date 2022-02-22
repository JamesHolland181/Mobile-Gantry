#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

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
String buffer="";

void setup() {
  // put your setup code here, to run once:
  pinMode(RPWM_a,OUTPUT); pinMode(LPWM_a,OUTPUT); pinMode(REN_a,OUTPUT); pinMode(LEN_a,OUTPUT); pinMode(RPWM_b,OUTPUT); pinMode(LPWM_b,OUTPUT); pinMode(REN_b,OUTPUT); pinMode(LEN_b,OUTPUT);

  // Disable Motors --> Until user input received
  digitalWrite(REN_a,LOW); digitalWrite(REN_b,LOW); digitalWrite(LEN_a,LOW); digitalWrite(LEN_b,LOW);

  // Bluetooth Setup
  Serial.begin(115200);
  SerialBT.begin();
  Serial.println("Bluetooth Started! Ready to pair...");    
}

void loop() {
  // put your main code here, to run repeatedly:
  while(SerialBT.available()){
    buffer = SerialBT.read();
    Serial.println(buffer);

    // Message format: [ Direction, Speed ]

    switch (expression)
    {
    case /* constant-expression */:
      /* code */
      break;
    
    default:
      break;
    }
  }
  Serial.println("out of loop");
  delay(20);

}