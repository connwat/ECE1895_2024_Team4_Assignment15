TODO:

implement debug mode (modify starting score)
analog pin (0-3) will be tied to the same output of the potentiometer that the throttle pin is connected to

there will be a check at the beginning of the game if some sequence of buttons is pressed, then the modify score mode will be entered
- the most simple method I can implement is just pressing the button a few times, and exiting the mode would be to press the keyboard button at the desired score
- I will implement just holding the button down for a few seconds, though, because the user may enter this mode without intent when pressing buttons

once the user is in this mode, the system will increase/decrease the score based on the toggle value and the voltage read at the analog input via the throttle

->
the voltage at the analog input via the throttle will always be positive, and the rate of change will be 250 ms

  toggle:
    1 -> increase the score 
    0 -> decrease the score


=> entering the mode

print "press to begin"

held = 0
while(interact button is not pressed){
  for(held = 0; keyboard button is pressed and held < 3000; ++held, delay 1ms);
  if(held == 3000){
   score <- initialize_score()      // get interactive score    
  }
}
while(interact button is pressed);


=> switching score (will be a separate function)
print "Initialize Score"

score <- 0;   // score will already be zero
volts <- 0;   // 0 - 5

for(; !digitalRead(keyboard_button); delay(250)){
  volts <- 5 - analogRead(throttle);                               // reading will be casted to an integer, the datatype of volts
  if(digitalRead(toggle)){
    score += (99 - score > throttle ? throttle : 99 - score);      // incrementing, cap at 99
  } else {
    score -= (score > throttle ? throttle : score);                // decrementing, floor at 0
  }
  print score on lcd screen
}
