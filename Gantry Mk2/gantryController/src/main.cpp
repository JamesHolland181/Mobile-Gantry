#include "Arduino.h"
#include "BluetoothSerial.h"
#include "driver/mcpwm.h"

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
int dir = 0;
int speed = 0;

void setup() {
  // put your setup code here, to run once:
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

  // Bluetooth Setup
  Serial.begin(115200);
  SerialBT.begin();
  Serial.println("Bluetooth Started! Ready to pair...");    
}

void loop() {
  while(1){
    // put your main code here, to run repeatedly:
    while(SerialBT.available()){
      String buffer = SerialBT.readString();
      Serial.println(buffer);

      dir = int(buffer[0]);
      String sub = buffer.substring(1,sizeof(buffer));
      speed = sub.toInt();
    }
    
      // Message format: [ Direction, Speed ]
      // Direction: 1-Fwd, 2-Bwd, 3-Lt, 4-Rt

      switch (dir)
      {
      case 1: // Fwd
        digitalWrite(LEN_a,HIGH);
        digitalWrite(LEN_b,HIGH);
        mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
        Serial.printf("Fwd: %d\n",speed);
        break;
      case 2: // Bwd
        digitalWrite(REN_a,HIGH);
        digitalWrite(REN_b,HIGH);
        mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
        Serial.printf("Bwd: %d\n",speed);
        break;
      case 3: // Lt
        digitalWrite(REN_a,HIGH);
        digitalWrite(LEN_b,HIGH);
        mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
        Serial.printf("Left: %d\n",speed);
        break; 
      case 4: // Rt
        digitalWrite(LEN_a,HIGH);
        digitalWrite(REN_b,HIGH);
        mcpwm_set_duty(MCPWM_UNIT_0,MCPWM_TIMER_0,MCPWM_OPR_A,speed);
        Serial.printf("Right: %d\n",speed);
        break;                  
      
      default:
        break;
      }
    vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate (100ms)
  }
}