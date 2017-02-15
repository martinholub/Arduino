/***********************************************************
File name: Lesson7.ino
Description: When you turn the potentiometer you will see
             LED bar graph display changing.
Author: Martin
Date: 2017/13/02

In this version the pin#1 keeps blinking although the supplied 
voltage is indicated as 0. I dont know why.
***********************************************************/
// observe two different ways hot to create global vars
#define MY_LENGTH 10 // by #define
const int potentiometerPin = 0; // as an int, cons int makes it inmutable
int stopper = 0;

void setup()
{  
  Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps. Useful for debugging
  int pin[MY_LENGTH];
  unsigned char ii;
  for(int ii=0; ii<MY_LENGTH;ii++){
    pin[ii] = ii+1; //definition digital 10 pins as pin to control the LED
    pinMode(pin[ii],OUTPUT);
    // if (ii == MY_LENGTH){Serial.println(pin[ii-1]);} else{Serial.print(ii);}
  } 
}

void loop() 
{
   stopper = stopper+1;
   float a = analogRead(potentiometerPin);//Read the potentiometer   
   a = map(a,0,1014,0,MY_LENGTH+1);    //Photoresistor voltage value converted from 0-1023 to 0-11
   
   if (stopper%MY_LENGTH == 0){
    Serial.print(stopper);
    Serial.print(" : The potentiometer voltage is "); // do some basic debugging
    Serial.println(a);
   }
   
   //unsigned char i;
   for(int i=1;i<=a;i++){
      digitalWrite(i,LOW);    //HIGH is set to about 5V PIN8
   }
   //unsigned char j;
   for(int j=MY_LENGTH;j>a+1;j--){
      digitalWrite(j,HIGH);   //HIGH is set to about 5V PIN8
   }   
   delay(50);                 //delay 50ms

//   if (stopper>10){
//    while(1){}
//   }
}
