/* 

libraries required: 
  AlmostRandom              <- for rng
  DFRobot_RGBLCD1602        <- for the lcd screen

pinouts required:
  interact button -> digital pin 8  
  speaker         -> digital pin 9
  game inputs     -> digital pins 0,1,2

  LCD inputs      -> analog pins 5,4 -> SCL,SDA
  other two LCD inputs are VCC and GND
  
*/

// preprocessor directives
#include <assert.h>
#include <AlmostRandom.h>
#include "DFRobot_RGBLCD1602.h"


// Create an lcd instance for the DFRobot module
DFRobot_RGBLCD1602 lcd(/*RGBAddr*/0x60 ,/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show

// Create an instance of AlmostRandom
AlmostRandom ar;


// words of encouragement, 16 character max, when the user give a correct input. the function to print these takes the size of the array, encouragement can be added and removed at ease of will
const char* natures[] = {
  "Good!           ", "Good job!     ", "Happy Happy!    ", "Correct!        ", "You're so quick!", "Too fast!     ", "Remember2Breathe", "Happy feet!     ", "WomboCombo!     ", 
  "Yippee!!        ", "Don't give up!", "YEEEAAAAAHHH!!  ", "OOMMMGGGG!!     ", "BOPIT CHAMP!    ", "WOOOOO HOOOO!!", "Keep it goin!   ", "Unstoppable!    ", "Ovaltine King!! "
};

const int8_t spkr    = 9;                                                  // speaker 
const int8_t act_btn = 8;                                                  // interact button
const int8_t size    = sizeof(natures)/8;                                  // quantity of elements in natures, for randomly choosing one string 

// game-input pins
const int8_t inp0 = 0;
const int8_t inp1 = 1;
const int8_t inp2 = 2;                                                    // the thrust will be read as a digital high/low w.r.t to the diode in the input pin


void setup() {

  lcd.init();                                                             // initialize lcd display

  lcd.setCursor(0, 0);                                                    // introduction sequence
  lcd.print("BopIt Test");
  
  lcd.setCursor(0, 1);
  lcd.print("Welcome Player!");
  
  #if defined(CONFIG_IDF_TARGET_ESP32S3)                                  // enable the timers for ESP32-S3 rng
    uint32_t* G0T0_CTRL = (uint32_t*)0x6001F000;                          // Start ESP32S3's timer
    uint32_t* G1T0_CTRL = (uint32_t*)0x60020000;

    *G0T0_CTRL |= (1<<31);
    *G1T0_CTRL |= (1<<31);
  #endif
  
  pinMode(spkr, OUTPUT);                    // speaker is output
  pinMode(act_btn, INPUT_PULLUP);           // reset button, normal high

  pinMode(inp0, INPUT_PULLUP);              // toggle
  pinMode(inp1, INPUT_PULLUP);              // keyboard key
  pinMode(inp2, INPUT_PULLUP);              // thrust
}


void loop() {
  
  int8_t ix,x,input,score = 0;                           // score can be modified here, can also implement a debug mode above to modify the starting value without reprogramming

  int16_t time, wait    = 3000 - score*15,               // input period (time period where the user can respond to the beeps)
                between = 500  - score*5;                // period between inputs (time before beeps of next iteration)

  assert(score >= 0 && score < 100);
  while(!digitalRead(act_btn));                          // wait while/if the interact button is pressed

  lcd.print("Press to Begin  ");

  while(digitalRead(act_btn));                           // wait until the interact button is pressed
  while(!digitalRead(act_btn));                          // wait while/if the interact button is pressed

  lcd.print("                ");                         // clear bottom line, cursor is already on bottom line and print wraps around the screen
  print_score(score);                                    // print score on console

  
  for(; score < 99; wait -= 15, between -= 5){           // game loop

    ix    = play(ar.getRandomByte()%3, 0);                                                  // play beeps and get formatted input, beep quantity, for input validation
    input = (digitalRead(inp2) << 2) | (digitalRead(inp1) << 1) | digitalRead(inp0);        // get current input as a number

    // scan for input for (time)-ms
    for(time = wait, x = input; x == input && --time; delay(1)){
      x = ((digitalRead(inp2) << 2) | (digitalRead(inp1) << 1) | digitalRead(inp0));        // read current input at pins to scan for changes
    }

    // ix corresponds to the bit in "input" that was expected to be toggled
    // if the user-input, x, does not match the value of the correct input, break
    if(x != (input^ix)){break;}
    
    print_score(--score);                                               // print (update) the score on the top line
    lcd.setCursor(0, 1);                                                // set lcd cursor to bottom line
    lcd.print(natures[ar.getRandomByte()%size]);                        // print encouragement
    delay(between);                                                     // delay between next beeps
  }
  
  lcd.setCursor(0, 1);                                                  // set cursor to bottom row
  score == 99 ? win(2500, 4) : lose(400, 4);

  while(digitalRead(act_btn));                                          // grab a seat and sit there until the user starts a new a game
}


// beep (0-3)-times
int8_t play(int8_t xi, int8_t bb){
  for(bb = xi + 1; bb--; delay(300)){
    tone(spkr, 2250, 200);
  }
  return (1 << xi);                                   // xi corresponds to the bit in the formatted number (line 96) that is expected to be toggled
}

// will print the score on the top row
void print_score(int16_t score){
  lcd.setCursor(0, 0);                                 // set cursor to zero
  lcd.print("Score:        ");                         // print score header
  lcd.print(score);                                    // print score
}

// emit "congratulations" noise sequence
void win(int16_t fq, int8_t i){
  lcd.print("Winner! Replay?");                        // print winning statement

  tone(spkr, 3000, 200);
  for(delay(250); --i; fq += 500, delay(150)){
    tone(spkr, fq, 125);
  }
}

// emit "game lost" noise sequence
void lose(int16_t fq, int8_t i){
  lcd.print("Loser! Replay?");                         // print losing statement :(

  for(; --i; fq -= 100, delay(400)){
    tone(spkr, fq, 300);
  }
  tone(spkr, fq, 1000);
}
