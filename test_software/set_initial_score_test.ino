

// preprocessor directives
#include <assert.h>
#include "DFRobot_RGBLCD1602.h"

const int8_t toggle   = 0;        // pin 2
const int8_t key_btn  = 1;        // pin 3
const int8_t throttle = 2;        // pin 4

const int8_t score_sw = A0;       // pin 23

const int8_t spkr    = 9;         // speaker 
const int8_t ibtn    = 8;         // interact button


// Create an lcd instance for the DFRobot module
DFRobot_RGBLCD1602 lcd(/*RGBAddr*/0x60 ,/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show


void setup() {
  
  lcd.init();

  pinMode(toggle, INPUT_PULLUP);
  pinMode(key_btn, INPUT_PULLUP);
  pinMode(throttle, INPUT_PULLUP);

  pinMode(score_sw, INPUT_PULLUP);

  pinMode(spkr, OUTPUT);
  pinMode(ibtn, INPUT_PULLUP);

}

void loop() {

  int16_t score{}, held;

  while(!digitalRead(ibtn));              // while interact button is low (pressed)

  // initial text to lcd
  lcd.setCursor(0, 1);
  lcd.print("Press to Begin  ");

  while(digitalRead(ibtn)){               // while interact button is high (not pressed)

    // while key button is low (pressed -> connected to ground) and has been held for less than three seconds
    for(held = 0; !digitalRead(key_btn) && held < 3000; ++held, delay(1));
    
    // if the button has been held for three seconds, enter the initialize score mode
    if(held == 3000){score = initialize_score(0, 0);}
  }

  while(!digitalRead(ibtn));              // while interact button is low (pressed)

  // print score to lcd
  lcd.setCursor(0, 0);
  lcd.print("Score:        ");
  lcd.print(score);

  while(digitalRead(ibtn));
}

int16_t initialize_score(int16_t volts, int16_t score){
  lcd.print("Initialize Score");                                              // print mode to screen
  lcd.setCursor(0, 0);

  for(; digitalRead(key_btn); delay(250)){
    volts = 5 - analogRead(score_sw);                                         // read volts as a casted-integer
    if(digitalRead(toggle)){
      score += (99 - score > volts ? volts : 99 - score);                     // increment, cap at 99
    } else {
      score -= (score > volts ? volts : score);                               // decrement, floor at 0
    }

    lcd.print("Score:        ");                                              // print updated score to monitor
    lcd.print(score);
  }

  lcd.setCursor(0, 1);
  lcd.print("Score is set    ");

  return score;
}
