/* 
libraries required: 
  AlmostRandom              <- for this file

pins required:
  digital pins 8,9          -> speaker, interact button
*/

#include <AlmostRandom.h>

// Create an instance of AlmostRandom
AlmostRandom ar;

const int spkr = 9;
const int btn  = 8;

int fq,i;

void setup() {
  
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
  
  delay(500);
  while(!digitalRead(btn));   // wait if interact button is pressed

  play();

  while(digitalRead(btn));    // wait until interact button is pressed
  while(!digitalRead(btn));   // wait if interact button is pressed

  win();
  
  while(digitalRead(btn));    // wait until interact button is pressed
  while(!digitalRead(btn));   // wait if interact button is pressed

  lose();
  
  while(digitalRead(btn));    // wait until interact button is pressed
}

// beep (0-3)-times
void play(){
  for(int i = ar.getRandomByte()%3 + 1; i--; delay(300)){
    tone(spkr, 2250, 200);
  }
}

// emit "congratulations" noise sequence
void win(){
  tone(spkr, 3000, 200);
  delay(250);
  
  for(fq = 2500, i = 3; i--; fq += 500, delay(150)){
    tone(spkr, fq, 125);
  }
}

// emit "game lost" noise sequence
void lose(){
  for(fq = 400, i = 4; --i; fq -= 100, delay(400)){
    tone(spkr, fq, 300);
  }
  tone(spkr, fq, 1000);
}
