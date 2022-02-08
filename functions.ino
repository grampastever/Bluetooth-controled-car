//The following code is required in a separate file in the same folder as BT_basic_slave.ino

//################ get_data()

void get_data()
{
 if (Serial.available() > 0)
  {
    in_data = Serial.readStringUntil('\n');
  }
  String payload = ""; 
  if (in_data != "0")
  {
    int offset = in_data.indexOf("U*"); //This is the header (0x552A)
    
    if (offset >= 0)
    {
      payload = in_data.substring(offset + 3, in_data.indexOf('\n'));
      char value = in_data.charAt (offset + 2);
      switch (value)
      {
        case 'A':
          x_joystick = map(payload.toInt(), 0,1023, -512, 512); //with the cable to the left, unmapped joystick x: full left = 0, full right = 1023
                                                                //I want full left = -512, full right = 512
          break;
        case 'B':
          y_joystick = map(payload.toInt(),0, 1023, -512, 512);//with the cable to the left, unmapped joystick y: full aft = 0, full fwd = 1023
                                                               //I want full aft = -512, full fwd = 512
          break;
      }
    }
  }  
}//end get_data()

//################# motors()
void motors()
{
  //case 1: joystick pure fwd
  if ((y_joystick >= deadband) && (y_joystick <= 512) && (x_joystick > -deadband) && (x_joystick < deadband))
  {
    //mySerial.println("case 1: joystick pure fwd");
    //set wheel direction
    digitalWrite(left_IN1, HIGH);   //left wheel forward
    digitalWrite(left_IN2, LOW);
    digitalWrite(right_IN3, HIGH);  //right wheel forward
    digitalWrite(right_IN4, LOW);

    //map joystick values to pwm values
    left_pwm_val = map(y_joystick,deadband,512,min_pwm, 255); 
    right_pwm_val = map(y_joystick,deadband,512,min_pwm, 255);

    //apply pwm
    analogWrite(left_ENA, left_pwm_val);
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 2: joystick pure aft
  else if ((y_joystick < -deadband) && (y_joystick >= -512) && (x_joystick > -deadband) && (x_joystick < deadband))
  {
    //mySerial.println("case 2: joystick pure aft");
    //set wheel direction
    digitalWrite(left_IN1, LOW);   //left wheel reverse
    digitalWrite(left_IN2, HIGH);
    digitalWrite(right_IN3, LOW);  //right wheel reverse
    digitalWrite(right_IN4, HIGH);

    //map joystick values to pwm values
    left_pwm_val = map(y_joystick,-deadband,-512,min_pwm, 255); 
    right_pwm_val = map(y_joystick,-deadband,-512,min_pwm, 255);
    
    //apply pwm
    analogWrite(left_ENA, left_pwm_val);
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 3: joystick pure right
  else if ((x_joystick >= deadband) && (x_joystick <= 512) && (y_joystick > -deadband) && (y_joystick < deadband))
  {
    //mySerial.println("case 3: joystick pure right");
    //set wheel direction
    digitalWrite(left_IN1, HIGH);   //left wheel forward
    digitalWrite(left_IN2, LOW);
    digitalWrite(right_IN3, LOW);  //right wheel reverse
    digitalWrite(right_IN4, HIGH);

    //map joystick values to pwm values
    left_pwm_val = map(x_joystick,+deadband,512,min_pwm, 255); 
    right_pwm_val = map(x_joystick,+deadband,512,min_pwm, 255);
    
    //apply pwm
    analogWrite(left_ENA, left_pwm_val);
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 4: joystick pure left
  else if ((x_joystick < -deadband) && (x_joystick >= -512) && (y_joystick > -deadband) && (y_joystick < deadband))
  {
    //mySerial.println("case 4: joystick pure left");
    //set wheel direction
    digitalWrite(left_IN1, LOW);   //left wheel forward
    digitalWrite(left_IN2, HIGH);
    digitalWrite(right_IN3, HIGH);  //right wheel reverse
    digitalWrite(right_IN4, LOW);

    //map joystick values to pwm values
    left_pwm_val = map(x_joystick,-deadband,-512,min_pwm, 255); 
    right_pwm_val = map(x_joystick,-deadband,-512,min_pwm, 255);
    
    //apply pwm
    analogWrite(left_ENA, left_pwm_val);
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 5: joystick forward and right
  else if ((y_joystick >= deadband) && (y_joystick <= 512) && (x_joystick >= deadband) && (x_joystick <= 512)) 
  {
    //mySerial.println("case 5: joystick forward and right");
    digitalWrite(left_IN1, HIGH); 
    digitalWrite(left_IN2, LOW);
    left_pwm_val = map(y_joystick,deadband,512,min_pwm,255);
    analogWrite(left_ENA, left_pwm_val);

    digitalWrite(right_IN3, HIGH); 
    digitalWrite(right_IN4, LOW);
    //right_pwm_val = map(y_joystick, deadband,512, min_pwm, max_turn_pwm); //make right wheel turn at half the speed as the left wheel
    right_pwm_val = max_turn_pwm;
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 6: joystick forward and left
  else if ((y_joystick >= deadband) && (y_joystick <= 512) && (x_joystick <= -deadband) && (x_joystick >= -512)) 
  {
    //mySerial.println("case 6: joystick forward and left");
    digitalWrite(left_IN1, HIGH); 
    digitalWrite(left_IN2, LOW);
    //left_pwm_val = map(y_joystick,deadband,512,min_pwm,max_turn_pwm);//make left wheel turn at half the speed as the right wheel
    left_pwm_val = max_turn_pwm;
    
    analogWrite(left_ENA, left_pwm_val);

    digitalWrite(right_IN3, HIGH); 
    digitalWrite(right_IN4, LOW);
    right_pwm_val = map(y_joystick, deadband,512, min_pwm, 255); 
    analogWrite(right_ENB, right_pwm_val); 
  }

  //case 7: joystick aft and left
  else if ((y_joystick <= -deadband) && (y_joystick >= -512) && (x_joystick <= -deadband) && (x_joystick >= -512)) 
  {
    //mySerial.println("case 7: joystick aft and left");
    digitalWrite(left_IN1, LOW); 
    digitalWrite(left_IN2, HIGH);
    //left_pwm_val = map(y_joystick,deadband,512,min_pwm,max_turn_pwm);//make left wheel turn at half the speed as the right wheel
    left_pwm_val = max_turn_pwm;
    analogWrite(left_ENA, left_pwm_val);

    digitalWrite(right_IN3, LOW); 
    digitalWrite(right_IN4, HIGH);
    right_pwm_val = map(y_joystick,-deadband,-512, min_pwm, 255); 
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 8: joystick aft and right
  else if ((y_joystick <= -deadband) && (y_joystick >= -512) && (x_joystick >= deadband) && (x_joystick <= 512)) 
  {
    //mySerial.println("case 8: joystick aft and right");
    digitalWrite(left_IN1, LOW); 
    digitalWrite(left_IN2, HIGH);
    left_pwm_val = map(y_joystick,-deadband,-512,min_pwm,255);
    analogWrite(left_ENA, left_pwm_val);

    digitalWrite(right_IN3, LOW); 
    digitalWrite(right_IN4, HIGH);
    //right_pwm_val = map(y_joystick, deadband,512, min_pwm, max_turn_pwm); //make right wheel turn at half the speed as the left wheel
    right_pwm_val = max_turn_pwm;
    analogWrite(right_ENB, right_pwm_val);
  }

  //case 9: joystick neutral
   else if ((y_joystick > -deadband) && (y_joystick < deadband) && (x_joystick > -deadband) && (x_joystick < deadband)) 
  {
    //mySerial.println("case 9: joystick neutral");
    digitalWrite(left_IN1, LOW);   //left wheel stopped
    digitalWrite(left_IN2, LOW);
    digitalWrite(right_IN3, LOW);  //right wheel stopped
    digitalWrite(right_IN4, LOW);

    left_pwm_val = 0;
    right_pwm_val = 0;

    analogWrite(left_ENA, left_pwm_val);
    analogWrite(right_ENB, right_pwm_val);
  } 
}//end motors()
