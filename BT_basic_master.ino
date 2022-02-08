//The following Arduino code is for the remote controller
//mates with BT_basic_slave.ino
//
//websites used to devlop the systsem:
//
//https://arduino.stackexchange.com/questions/23964/bluetooth-sending-values-from-two-joysticks
//http://www.martyncurrey.com/hc-05-fs-040-state-pin/
//https://www.teachmemicro.com/hc-05-bluetooth-command-list/
//https://howtomechatronics.com/tutorials/arduino/how-to-configure-pair-two-hc-05-bluetooth-module-master-slave-commands/
//https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
//
//
//8 Feb 2022

#include <SoftwareSerial.h>

#define xAxis A0
#define yAxis A1

#define BT_status 8
#define BT_connect_light 9


SoftwareSerial mySerial(2,3); //(RX, TX). The SoftwareSerial object mySerial is for displaying information to the Arduino IDE serial monitor

String in_data = "0"; //Store each line of Serial communication
bool BT_connected = false;

// Creating variables
int x_axis; //joystick x axis value
int y_axis; //joystick y axis value

void setup() 
{
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
}//end setup()

void loop() {
  // reading from the joystick
  x_axis = analogRead(xAxis);
  y_axis = map(analogRead(yAxis),0,1023,1023,0); //invert so that full back is 0 and full fwd is 1023
  
  mySerial.print("x_axis="); mySerial.print(x_axis); mySerial.print(",  y_axis=");mySerial.println(y_axis);

  //Packing the Serial message
  //mySerial.print("U*"); //A header
  Serial.print("U*");
  //mySerial.print("A");  //a token to indicate the message payload
  Serial.print("A");
  //mySerial.print(x_axis);
  Serial.print(x_axis);
  //mySerial.println("");
  Serial.println("");  

  delay(30); 

  //mySerial.print("U*"); //A header
  Serial.print("U*");
  //mySerial.print("B");  //a token to indicate the message payload
  Serial.print("B");
  //mySerial.print(y_axis);
  Serial.print(y_axis);
  //mySerial.println("");  
  Serial.println("");

  delay(30); 

  //check BT connection
  if (digitalRead(BT_status) == LOW)
  {    
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
