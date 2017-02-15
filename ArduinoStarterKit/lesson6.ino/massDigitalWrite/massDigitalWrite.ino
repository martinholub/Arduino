void massDigitalWrite(int pins[8], unsigned char length, unsigned char state)
{
 unsigned char i;
 for (i = 0; i < length; i++)
   digitalWrite(pins, state);
}

void setup()
{
  unsigned char b;
  int pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  for(b=1;b<10; b++){
    massDigitalWrite(pins, 8, LOW);
    delay(20);
    massDigitalWrite(pins, 8, HIGH);
    delay(20);
  }
}
