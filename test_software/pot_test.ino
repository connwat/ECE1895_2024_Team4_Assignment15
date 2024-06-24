
/*

pins used:
  digital pins 0,9 -> potentiometer 50k, LED

*/

const int led = 9;
const int pot = 0;

void setup() {

  pinMode(pot, INPUT_PULLUP);
  pinMode(led, OUTPUT);

}

void loop() {

  digitalWrite(led, digitalRead(pot));

}
