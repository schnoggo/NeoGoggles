/**
 * Read inputs
 */


void ClearButtonState(){
  button_state_start_time = millis(); // reset the stablility timer
  current_button_state = false;
  prev_button_state = hardware_button_state;
  button_seen_up = false;
}

void UpdateButtonState(){
// globals:
unsigned long now = millis();

#if SWITCH_STYLE == VIBRATION
//  retVal = last_button_state;
  last_button_state = false;
#else

  if (digitalRead(BUTTON_PIN) == HIGH){
    hardware_button_state = 1;
  } else {
     hardware_button_state = 0;
  }
  if(hardware_button_state == prev_button_state){ // state is stable over multiple reads
    if (now - button_state_start_time > BUTTON_BOUNCE_TIME){ // is it stable over time?
      // button is stable: update global information
      if (hardware_button_state){ // was it stable in pressed position?
        if (button_seen_up){ // yes, has it been released?
          // yes, this is a new press - actually do something
          current_button_state =  true;
          dprintln("click");
          button_seen_up  = false; // force system to wait until release before registering another click
        } // button_seen_up
      } else { // stable, but not pressed:
        button_seen_up =  true; // mark that button has been released
      }
    } // stable over time

  } else { // button state doesn't match previous read
    prev_button_state = hardware_button_state;
    button_state_start_time = now; // reset the stablility timer
    current_button_state = false;
  }
#endif


}


/**
 * Returns the current computed state of the button and
 * resets the system
 *
 * @param Boolean current state of debounced (or latched) button
 */
 boolean GetButtonState(){
   // globals:
   // current_button_state

   boolean retVal = false;

   if (current_button_state){
    retVal = false;
    ClearButtonState();
   }
   return retVal;
 }
