/* 

libraries required: 
  AlmostRandom              <- for rng
  DFRobot_RGBLCD1602        <- for the lcd screen

pins used:
  analog pins  5,4 -> scl,sda -> LCD
  digital pins 8,9 -> btn, speaker

*/

#include <AlmostRandom.h>
#include "DFRobot_RGBLCD1602.h"

// Create an lcd instance for the DFRobot module
DFRobot_RGBLCD1602 lcd(/*RGBAddr*/0x60 ,/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show

// Create an instance of AlmostRandom
AlmostRandom ar;

const int spkr = 9;
const int btn  = 8;

int fq,i;

// words of encouragement, 16 character max, when the user give a correct input. the function to print these takes the size of the array
// encouragement can be added and removed at ease of will
const char* natures[] = {
  "Good!", "Good job!", "Happy Happy!", "Correct!", "You're so quick!", "Too fast!", "Remember2Breathe", "Happy feet!", "WomboCombo!"
};

void setup() {

  lcd.init();

// introduction sequence
  lcd.setCursor(0, 0);
  lcd.print("Welcome to BopIt");

  delay(2500);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Made by ...");

  lcd.setCursor(0, 1);
  delay(1000);
  lcd.print("Connor Watson");

  lcd.setCursor(0, 1);
  delay(1000);
  lcd.print("Lamine Mezali");

  lcd.setCursor(0, 1);
  delay(1000);
  lcd.print("Clinton Connelly");
  delay(1000);

  
  // enable the timers for ESP32-S3 rng
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
    // Start ESP32S3's timer
    uint32_t* G0T0_CTRL = (uint32_t*)0x6001F000;
    uint32_t* G1T0_CTRL = (uint32_t*)0x60020000;

    *G0T0_CTRL |= (1<<31);
    *G1T0_CTRL |= (1<<31);
  #endif

  pinMode(spkr, OUTPUT);          // speaker is output
  pinMode(btn, INPUT_PULLUP);     // interact button, normal high
}

void loop() {
  lcd.clear();

  clear_row(0);
  print_score(ar.getRandomByte()%99);

  clear_row(1);                                                       // clear the bottom line for the encouragement
  lcd.print(natures[ar.getRandomByte()%(sizeof(natures)/8)]);         // print encouragement
  
  delay(500);
  while(!digitalRead(btn));   // wait if interact button is pressed

  win();

  while(digitalRead(btn));    // wait until interact button is pressed
  while(!digitalRead(btn));   // wait until interact button is released

  lose();

  while(digitalRead(btn));    // wait until interact button is pressed
}

// will clear either the top or bottom row of the LCD
void clear_row(bool i){
  lcd.setCursor(0, i);                  // set cursor to the row
  lcd.print("                ");        // print 16 empty characters
  lcd.setCursor(0, i);                  // normalize cursor
}

void print_score(int score){
  lcd.setCursor(0, 0);
  lcd.print("Score:");

  lcd.setCursor(14, 0);
  lcd.print(score);
}

// emit "congratulations" noise sequence
void win(){
  lcd.setCursor(0, 1);
  lcd.print("Winner! Replay?");

  tone(spkr, 3000, 200);
  delay(250);
  
  for(fq = 2500, i = 4; --i; fq += 500, delay(150)){
    tone(spkr, fq, 125);
  }
}

// emit "game lost" noise sequence
void lose(){
  lcd.setCursor(0, 1);
  lcd.print("Loser! Replay?");

  for(fq = 400, i = 4; --i; fq -= 100, delay(400)){
    tone(spkr, fq, 300);
  }
  tone(spkr, fq, 1000);
}
