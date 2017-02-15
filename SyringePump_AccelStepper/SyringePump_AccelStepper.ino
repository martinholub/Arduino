#include <AccelStepper.h>

//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source ( + term)
//connect GRD to power source (- term)

AccelStepper stepper(1, 3, 4);  // driver step direction

// used motor has 200 steps per 1 revolution
// we are using microstepping, i.e. each step is divided into additional 16 substeps
// the emloyed lead screw has pitch = lead = 1mm
// considered syringe has ID = 25 mm and V = 20 ml
// this results in working lenght 40.75 mm

char  userInput[15] = {'d','0','0','0','0','0','v','0','0','0','0','0','d','1','\r'};

int dislocation=0; int mspeed=1;  int mdirection=0;  int data=0; int microstepping = 16;  int   msg_lenght = 0;
int distTraveled = 0;

// Syringe working lenght in mm
int syringeLenght = 40.75;

void setup(){  
  Serial.begin(9600);   // this barely states encoding
  stepper.setMaxSpeed(1000.0);   
  stepper.setAcceleration(1000.0);   
  stepper.setCurrentPosition(0);
  }

void loop() {
    delay(500); // wait for 500 milliseconds before starting
    
    if ( data==0 ) readuserdata();
    
    if ( data==1 ){
      // dislocation is number of steps, number of microsteps is given by dislocation*microstepping
      // therefore 1 revolution should be equal to d00200
      dislocation =  char(userInput[1] - 48)*10000  + char(userInput[2] - 48)*1000  + char(userInput[3] - 48)*100  + char(userInput[4] - 48)*10   + char(userInput[5] - 48);
      // speed is stated in steps / sec (we want low speeds, thats why there is a shift in multipliers)
      // therefore the speed of 1mm / sec should be obtained by setting v20000, this equals one full revolution which is the lead, which is 1 mm
      // consequently this should translate to Q = 490.87 ul/s = 8.18 ul/hr
      // at this rate our syringe will be emptied in approx 40 sec
      mspeed =       char(userInput[7] - 48)*100  + char(userInput[8] - 48)*10  + char(userInput[9] - 48)*1  + char(userInput[10] - 48)*0.1  + char(userInput[11] - 48)*0.01;
      // check the polarity of your motor to ensure that 1 corresponds to forward and 0 to backward direction
      mdirection =   char(userInput[13] - 48);
      
      
      if (mdirection==0) // 0 results in opposite direction
        dislocation=-1*dislocation;   
      else // 1 results in forward direction
        dislocation=dislocation;   

      Serial.println(" "); Serial.print(dislocation, DEC); Serial.print(" "); Serial.print(mspeed, DEC); Serial.print(" "); Serial.println(mdirection, DEC);        
            
      stepper.setSpeed(mspeed*microstepping);        
      stepper.setMaxSpeed(mspeed*microstepping);
      stepper.runToNewPosition(dislocation*microstepping);
      //stepper.setCurrentPosition(0);
      //keep track of travelled distance in mm
      distTraveled = distTraveled + dislocation*0.001*0.005;
      
      data=0;       dislocation=0;      mspeed=0;      mdirection=0;

      for (int i=0 ; i<=15 ; i++)      
          userInput[i]=0;
      }
}

void readuserdata(){
  char tmp;
     if(Serial.available()){     
        msg_lenght = Serial.available(); 
        Serial.print("Message lenght=");
        Serial.print(msg_lenght);
        if( msg_lenght == 14  ){ 
           tmp = Serial.read();
           if( tmp == 'd' ){
              userInput[0] = tmp;
              for (int i=1;i<=msg_lenght;i++){
                tmp=Serial.read(); 
                userInput[i] = char(tmp);                  
              }
              data=1;
              }
        }
     }
}
