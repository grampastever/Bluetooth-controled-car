//The following code is for the remote control vehicle
//
//This file requires the file functions.ino to be in the same directory as BT_basic_slave.ino
//
//mates with BT_basic_master.ino
//
//9 Feb 2022

#include <SoftwareSerial.h>

#define left_IN1  5
#define left_IN2  4
#define left_ENA  9           //enable PWM for the left wheel

//Right wheel red-->OUT4, black-->OUT3
#define right_IN3  3
#define right_IN4  2
#define right_ENB  10         //enable PWM for the right wheel

#define BT_status 8
#define BT_connect_light 11

SoftwareSerial mySerial(6,7); //()RX,TX). The SoftwareSerial object mySerial is for displaying information to the Arduino IDE serial monitor

String in_data = "0"; //Store each line of Serial communication
bool BT_connected = false;

int x_joystick;
int y_joystick;
int min_pwm = 0;
int deadband = 100;
int max_turn_pwm = 128;

int left_pwm_val; //left wheel PWM value
int right_pwm_val; //right wheel PWM value

void setup() 
{
  pinMode(left_IN1, OUTPUT);
  pinMode(left_IN2, OUTPUT);
  pinMode(right_IN3,OUTPUT);
  pinMode(right_IN4,OUTPUT);

  //motors stopped
  digitalWrite(left_IN1,LOW);
  digitalWrite(left_IN2, LOW);
  digitalWrite(right_IN3, LOW);
  digitalWrite(right_IN4, LOW);

  pinMode(BT_status, INPUT);
  pinMode(BT_connect_light, OUTPUT);
  digitalWrite(BT_connect_light, LOW);
  
  mySerial.begin(38400); //serial monitor

  while (!BT_connected)
  { 
    if ( digitalRead(BT_status)==HIGH)
    { 
      BT_connected = true;
      digitalWrite(BT_connect_light, HIGH);
      Serial.begin(38400);                  //bluetooth serial data uses the arduino serial port on tx and rx pins
      mySerial.println("connected");
    }
  }
  delay(1000);
}//end setup()

void loop() 
{
  get_data();
  
  //mySerial.print(x_joystick); mySerial.print("  "); mySerial.println(y_joystick);
  
  motors();

//check BT connection
  if (digitalRead(BT_status) == LOW)
  {
    //motors stopped
    digitalWrite(left_IN1,LOW);
    digitalWrite(left_IN2, LOW);
    digitalWrite(right_IN3, LOW);
    digitalWrite(right_IN4, LOW);
    
    BT_connected = false;
    Serial.end();
    digitalWrite(BT_connect_light, LOW);
    mySerial.println("disconnected");
    
    while (!BT_connected)
    { 
      if ( digitalRead(BT_status)==HIGH)  
      { 
        BT_connected = true;
        digitalWrite(BT_connect_light, HIGH);
        Serial.begin(38400);  //bluetooth serial
        mySerial.println("reconnected");
      }
    }
  }
}//end loop()
