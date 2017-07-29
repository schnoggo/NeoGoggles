/**
 * Read inputs
 */


 boolean GetButtonState(){
   // globals:
   // last_button_state
  dprint(".");
   boolean retVal = false;

 #if SWITCH_STYLE == VIBRATION
     retVal = last_button_state;
     last_button_state = false;


 #else
 // no inputs - everything is global

   unsigned long now = millis();


     if (digitalRead(BUTTON_PIN) == HIGH){

       button_state = 1;
     } else {
       button_state = 0;

     }

     if(button_state == prev_button_state){
       if (now - button_state_start_time > BUTTON_BOUNCE_TIME){
         // button is stable: update
         if (button_state){ // button pressed
           if (button_seen_up){
             // new press - actually do something
             retVal =  true;
            dprintln("click");
             button_seen_up  = 0;
           }
         } else { // but not pressed:
           button_seen_up =  1;
         }
       }

     } else { // button state changed
       prev_button_state = button_state;
       button_state_start_time = now;

     }
 #endif
   return retVal;
 }
