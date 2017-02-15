/***********************************************************
File name: 06_flowingLed.ino
Description: LED turn lighting control
Date: 2017/10/02 
***********************************************************/
void setup()
{
  unsigned char ledPin;           //ledPin will be set to 1,2,3,4,5,6, 7 and 8.
  for(ledPin=8;ledPin>=1;ledPin--)//In turn set 1 ~ 8 digital pins to output mode 
  {
  pinMode(ledPin,OUTPUT);         //Set the  ledPin pin to output mode
  }
}

void loop(){   
  unsigned char ledPin;               //ledPin will be set to 1,2,3,4,5,6, 7 and 8.
  unsigned char ii;                   // Iterable
  for(ii=1;ii<=4;ii++){               //Every 200ms on in order LED1 ~ 8
    digitalWrite(ii, HIGH);
    delay(500);
    digitalWrite(ii+4, HIGH);
    delay(500);}
  
  unsigned char b;
  int length = 8;
  int pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  
  for(b=1;b<10; b++){
    unsigned char i;
    // char stateHigh = HIGH;
    for (i = 0; i < length; i++){
      digitalWrite(pins[i], LOW);
    }
    delay(100);
    for (i = 0; i < length; i++){
      digitalWrite(pins[i], HIGH);
    }
    delay(100);
  }
  for(ledPin=8;ledPin>=1;ledPin--){     //Every 200ms off in order LED1 ~ 8 
    digitalWrite(ledPin,LOW);           //led off
    delay(500);                        //Delay 200 ms
  } 
}
