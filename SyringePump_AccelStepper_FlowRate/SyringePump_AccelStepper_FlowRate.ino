#include <AccelStepper.h>

//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source ( + term)
//connect GRD to power source (- term)

AccelStepper stepper(1, 3, 4);  // (driver, step direction)

// Project constants
const float syringeDiameter = 25.00; // considered syringe has ID = 25 mm
const float syringeVolume = 20.00; // and V = 20 ml
const float syringeLenght = 40.75; // this results in working lenght 40.75 mm
const float pi = 3.14159;
const int stepsPerRev = 200; // used motor has 200 steps per 1 revolution
const float syringeCrossSection = pi*pow(syringeDiameter/2, 2);
const int lead = 1; // the emloyed lead screw has pitch = lead = 1mm
const int microstepping = 16; // we are using microstepping, i.e. each step is divided into additional 16 substeps
// this is done via hardware connection

// Project Variables
int dislocation = 0;
float mspeed = 0.0;
float vspeed = 0.0;
int mdirection = 0;
int data = 0;
int msg_lenght = 0;
float distTraveled = 0.0;
float distance = 0;
float flowRate = 0.0;
float volume = 0.0;

// user defined input. Done via Serial Monitor
char  userInput[15] = {'V','0','0','0','0','0','Q','0','0','0','0','0','d','1','\r'};

void setup(){  
  Serial.begin(9600);   // this barely states encoding
  stepper.setMaxSpeed(1000.0);   
  stepper.setAcceleration(1000.0);   
  stepper.setCurrentPosition(0);
  Serial.println("Example calling V10000Q05000d1 will result in the pump pumping ");
  Serial.println(" 10'000 ul (~V10000) with flow rate 50 ul/s (~Q05000)");
  Serial.println("!There are two decimals in the flow rate!");
  Serial.println("!Volume is in ul!");
  Serial.println(" ");
  }

void loop() {
    delay(500); // wait for 500 milliseconds before starting
    
    if ( data==0 ) readuserdata();
    
    if ( data==1 ){
      // estimate volume to be dispensed in ul
      volume = char(userInput[1] - 48)*10000  + char(userInput[2] - 48)*1000  + char(userInput[3] - 48)*100  + char(userInput[4] - 48)*10   + char(userInput[5] - 48);
      // provide desired flow rate in ul/s
      // for example V?????Q05000d? will result in approx. 0.1Hz = 0.1 mm/s speed
      flowRate =  char(userInput[7] - 48)*100  + char(userInput[8] - 48)*10  + char(userInput[9] - 48)*1  + char(userInput[10] - 48)*0.1   + char(userInput[11] - 48)*0.01;
      // check the polarity of your motor to ensure that 1 corresponds to forward and 0 to backward direction
      mdirection =   char(userInput[13] - 48);

      distance = volume/syringeCrossSection;
      dislocation = distance*stepsPerRev; // calculate number of steps
      vspeed = (flowRate/syringeCrossSection); // mm / sec
      mspeed = vspeed*(stepsPerRev/lead);  // calculate speed in steps per sec
      
      if (mdirection==0) // 0 results in reverse direction
        dislocation = -1*dislocation;
      else // 1 results in forward direction
        dislocation = dislocation;
      
      ////Crash Check - not tuned yet
      //distTraveled = distTraveled + distance; //keep track of travelled distance in mm
      //if ( distTraveled > 40 || mspeed > 500) {
      //  Serial.println(" "); Serial.println("You requestued a crash or too high a speed and the program started over");
      ////Clear variables
      //data=0; dislocation=0;  mspeed=0; mdirection=0; flowRate = 0; distTraveled = 0;
      //  for (int i=0 ; i<=12 ; i++){
      //    userInput[i]=0;
      //}
      // }
      // else {
      
      // Communicate what you are doing with user
      Serial.println(" "); Serial.print("The distance to travel is "); Serial.print(distance, DEC); Serial.println(" mm ");
      Serial.println(" "); Serial.print("The volume to dispense is "); Serial.print(volume, DEC); Serial.println(" ul ");
      Serial.print("Flow rate is "); Serial.print(flowRate, DEC); Serial.println(" ul/s ");
      Serial.print("The direction is "); Serial.println(mdirection, DEC);
      Serial.print("Number of steps to do = "); Serial.println(dislocation, DEC);
      Serial.print("Speed in steps/sec = "); Serial.println(mspeed, DEC);
      Serial.print("Speed in mm/sec = "); Serial.println(vspeed, DEC);
      Serial.println(" ");
           
      // Set values for AccelStepper lib, include chosen microstepping mode      
      stepper.setSpeed(mspeed*microstepping);        
      stepper.setMaxSpeed(mspeed*microstepping);
      stepper.runToNewPosition(dislocation*microstepping);
      stepper.setCurrentPosition(0); // current code requires this line to be functional

      // Clear variables
      data = 0;
      dislocation = 0; distance = 0.0;
      mspeed = 0.0; vspeed = 0.0;
      flowRate = 0.0;
      mdirection = 0;
      volume = 0.0;
      
      for (int i=0 ; i<=15 ; i++)      
          userInput[i] = 0;
      // }
    }
}

void readuserdata(){
  char tmp;
     if(Serial.available()){     
        msg_lenght = Serial.available(); 
        Serial.print("Message lenght= "); Serial.print(msg_lenght);
        if( msg_lenght == 14  ){ 
           tmp = Serial.read();
           if( tmp == 'V' ){
              userInput[0] = tmp;
              for (int i = 1; i <= msg_lenght;i++){
                tmp=Serial.read(); 
                userInput[i] = char(tmp);                  
              }
              data = 1;
              }
        }
     }
}
