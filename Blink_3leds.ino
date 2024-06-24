//LAMINE MEZALI


#define led_1 7
#define led_2 8
#define led_3 9
#define button 2

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(button, INPUT_PULLUP);  // Initialize digital pin button as an input with internal pull-up resistor
}

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(button) == LOW) {  // Check if the button is pressed
    // Turn on LED 1, then LED 2, then LED 3
    digitalWrite(led_1, HIGH);
    delay(1000);
    digitalWrite(led_1, LOW);
    
    digitalWrite(led_2, HIGH);
    delay(1000);
    digitalWrite(led_2, LOW);
    
    digitalWrite(led_3, HIGH);
    delay(1000);
    digitalWrite(led_3, LOW);
  } else {
    // Ensure all LEDs are off when the button is not pressed
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
  }
}
