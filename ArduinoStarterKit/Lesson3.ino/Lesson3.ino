int ledPin = 11; //led is controled by digital pin 11
int butPin = 2; // button infroms digital pin 2

volatile int state = LOW; // output vaiable

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // we control led
  pinMode(butPin, INPUT); // we let button decide
}

//void loop() {
//  // put your main code here, to run repeatedly:
//  int butMode1 = digitalRead(butPin);
//  if(butMode1 == LOW){ // check that the button is open 
//    delay(10); // wait
//    int butMode2 = digitalRead(butPin); // check again if still open to eliminate jitter
//      if(butMode2 == LOW) {
//        while(digitalRead(butPin) == LOW); // do nothing while waiting for someone to press the button
//        delay(10); // wait 10 ms to eliminate jitter
//        while(digitalRead(butPin) == LOW); // make sure that someone released the button
//        state = !state; // set the inverse state
//        digitalWrite(ledPin, state); // control led by button state
//        }
//      }
//}

void loop() {
  // put your main code here, to run repeatedly:
  int butMode1 = digitalRead(butPin);
  if(butMode1 == LOW){ // check that the button is open 
    delay(10);
    int butMode2 = digitalRead(butPin);
    if(butMode2 == LOW) {
        while(digitalRead(butPin) == LOW); // do nothing while waiting for someone to press the button
        state = !state; // set the inverse state
        digitalWrite(ledPin, state); // control led by button state
        }
      }
}
