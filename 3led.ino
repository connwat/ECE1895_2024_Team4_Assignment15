// clinton connelly

int led = 9;
const int btn = 8;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
}

void loop() {
  // if btn is pressed, blink LED
  while(!digitalRead(btn)){

    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);

    ++led = 9 + led%3;    // 9, 10, 11, 9, 10, ...
  }

  digitalWrite(led, LOW);
  led = 9;

}
