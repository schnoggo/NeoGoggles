/**
 * Functions to animate the neoPixel LED strips/rings
 * @package neogoggles
 * @author Lon Koenig <lon@lonk.me>
 * @license https://opensource.org/licenses/MIT
 */

/**
 * Begin an animation by calling StartAnimation()
 * Update display by calling UpdateAnimation()
 *
 * Animation IDs are defined in neo_goggles.h
 *
 * @param uint8_t which_animation ID of the animation
 */
void StartAnimation(uint8_t which_animation){
  // globals:
  // animation_frame
  // current_animation
  // pixels_dirty
  current_animation = which_animation;
  animation_frame = 0;
  pixels_dirty = true;
  dprint("StartAnimation:")
  dprintln(which_animation);
  if (SOLID_ANIM == which_animation){
    SolidRing(0x222222, true);

  }
}

/**
 * Background update function to draw
 * next frame of current animation
 */
void UpdateAnimation(){
// globals
// current_animation
// animation_frame
// animation_color

uint16_t frame_duration = 200;

switch(current_animation) {


case COMET_ANIM:
  // streak around the ring with current color
  ClearRings(false);
  DrawRingPixel(animation_frame, animation_color, false);
  if ((++animation_frame)>15) {animation_frame = 0;}

  frame_duration = 1000;
  pixels_dirty =  true;
break;


 case SPARKS_ANIM: // Random sparks - just one LED on at a time
 // ======================================================
  i = random(MAX_PIXELS);
 // pixels.setPixelColor(i, animation_color);
  pixels.setPixelColor(i, pixels.Color(SteppedColor(), SteppedColor(), SteppedColor() ));
  pixels.show();
  BackgroundDelay(10);
  pixels.setPixelColor(i, 0);
  frame_duration = 0;
  pixels_dirty =  false;
  break;

  case FLASH_ANIM:
  // blinky
    this_color = animation_color;
    if (1 == animation_frame){ this_color = 0; }

      for(i=0; i<16; i++) { //count around the ring of pixels
        pixels.setPixelColor( i, this_color); // First eye
        pixels.setPixelColor(16 + i  , this_color); //
      }

    if ((++animation_frame)>1) {animation_frame = 0;}
    frame_duration = 1000;
    pixels_dirty =  true;
  break;


 case SPINNY_ANIM: // Spinny wheels (4 LEDs on at a time)
// ======================================================


  for(i=0; i<8; i++) { //count around the ring of pixels
    this_color = 0; // turn off non-selected pixels
      if(

       (i == NormalizeRingPos(animation_frame))
       or
       (i == NormalizeRingPos(animation_frame + 1) )
       or
       (i == NormalizeRingPos(animation_frame+8))
       or
       (i == NormalizeRingPos(animation_frame + 1 + 8) )
      ){
        this_color = animation_color;
      }


//    if(((animation_frame + i) & 7) < 2) {this_color = animation_color;} // 4 pixels on...

    pixels.setPixelColor(  NormalizeRingPos(i+leftOff), this_color); // First eye
    pixels.setPixelColor(  NormalizeRingPos(i+leftOff + 8), this_color); // First eye

    pixels.setPixelColor(16 + NormalizeRingPos(16-i+rightOff)  , this_color); // Second eye (flipped)
  }

  //animation_frame = animation_frame++ % 16;
  if ((++animation_frame)>15) {animation_frame = 0;}
    frame_duration = 60;
    pixels_dirty =  true;
  break;



  case GOOGLY_ANIM:
 // googly
 // ======================================================

  inertia = ((inertia -  (hires_pos /3 )) * friction) /denom;
 // inertia = moment;
  hires_pos = hires_pos + inertia;
  if (hires_pos >= system_size){
    // FlashRing();
    hires_pos = -(hires_pos - system_size);
    inertia = -inertia;
  } else if (hires_pos < (-system_size)) {
   //  FlashRing();
    hires_pos = system_size + hires_pos;
    inertia = -inertia;
  }
  // + 8  to rotate 0 to bottom
 ring_pos = NormalizeRingPos(8+ (hires_pos / scale2pixel));


  for(i=0; i<16; i++) {
    this_color = 0;
    if(RingDistance(ring_pos, i)<2) this_color = animation_color;
    pixels.setPixelColor(    NormalizeRingPos(i+leftOff )  , this_color); // First eye
    pixels.setPixelColor( 16 +NormalizeRingPos(i+rightOff) , this_color); // Second eye (not flipped)
  }
  pixels.show();

  BackgroundDelay(24);

  // randomly add an impulse:
  if (random(60) == 1){
   inertia = inertia + random(800);

  }


  break;

    case LARSON_SCANNER: // larson scanner:
// ======================================================
// actually, just a spinner in this version
    for(i=0; i<16; i++) {
    this_color = 0; // turn off non-selected pixels
    if(testpos == i) {this_color = 0xFFFF00;} // 4 pixels on...
    pixels.setPixelColor(  NormalizeRingPos(i+leftOff), this_color); // First eye
    pixels.setPixelColor(16 + NormalizeRingPos(i+rightOff)  , this_color); // Second eye (flipped)
  }
  testpos++;
  if (testpos>15){testpos=0;}
  BackgroundDelay(60);
  pixels.show();

  break;


  case HALF_BLINK_ANIM:


  for(i=0; i<MAX_PIXELS; i=i+1){ // light every other pixel
      if ((i%2) == (byte) animation_frame ){
    pixels.setPixelColor(i, animation_color);

  } else {
    pixels.setPixelColor(i, pixels.Color(0,40,0));

  }
    //animation_frame = animation_frame++ % 16;
    if ( 1 == animation_frame) {
      animation_frame = 0;
    } else {
      animation_frame =1;
    }

      frame_duration = 1000;
      pixels_dirty =  true;
  }
  break;

#if USE_FLAME
  case FIRE_ANIM: // fire
   InitFlames();

  for(byte flame_count=0; flame_count<NUMBER_OF_FLAMES; flame_count++) {
    switch(flames[flame_count].state){
      case 0: // reset
        CreateNewFlame(flame_count);
      break;

      case 1: //increasing
        new_brightness = flames[flame_count].brightness + flames[flame_count].step;
        if (new_brightness > flames[flame_count].max_brightness){
          UpdateFlameColor(flame_count, flames[flame_count].max_brightness);
          flames[flame_count].brightness = flames[flame_count].max_brightness;
          flames[flame_count].step = GetStepSize(); // pick a different speed for flame going out
          flames[flame_count].state = 2;
        } else {
          UpdateFlameColor(flame_count, new_brightness);
          flames[flame_count].brightness = new_brightness;
        }
      break;


      case 2: //decreasing
        new_brightness = flames[flame_count].brightness - flames[flame_count].step;
        // chance to flicker/rekindle:
        if (random(new_brightness) < FLICKER_CHANCE){
          // rekindle:
          flames[flame_count].state = 1; //increase again
          flames[flame_count].brightness = max(GetMaxBrightness(), flames[flame_count].brightness);
          flames[flame_count].step = GetStepSize();

        } else {
          if (new_brightness <1){
            flames[flame_count].state = 0; // bottomed out - reset to next flame
            flames[flame_count].brightness = 0;
            UpdateFlameColor(flame_count, 0);
          } else {
            UpdateFlameColor(flame_count, new_brightness);
            flames[flame_count].brightness = new_brightness;
          }
        }
      break;
    }

  } // step through flame_count
  pixels.show();
  delay(22);
  break;
#endif
} // end the big switch
  delay(frame_duration);
  //BackgroundDelay
  if (pixels_dirty){
    pixels.show();
    pixels_dirty = false;
  }
}

/**
 * Light an LED at specicied location on both eyes
 * acccounting for rotation, offsets and reflection
 *
 * @param uint8_t position which pixel to light up
 * @param uint32_t this_color color of pixel
 * @param boolean reflection true = mirror to other eye. False copy to other eye
 */
void DrawRingPixel(uint8_t position, uint32_t this_color,  boolean reflection){
  // globals:

  pixels.setPixelColor( OffsetLeftPos(position) , this_color); // left eye
  if (reflection){
      pixels.setPixelColor( OffsetRightPos(position) , this_color); // right eye
  } else {
    pixels.setPixelColor( OffsetRightPos(position) , this_color); // right eye
  }

}

/**
 * Convert virtual pixel position to real-world hardware position
 *
 * @param uint8_t v_pos current (virtual) position
 *
 * @return uint8_t translated pixel position in neoPixel strip
 */
uint8_t OffsetLeftPos(uint8_t v_pos){
  // globals:
  // leftOff position of "top" on the physical neoPixel ring
   return (NormalizeRingPos( v_pos + leftOff));
}

/**
 * Convert virtual pixel position to real-world hardware position
 *
 * @param uint8_t v_pos current (virtual) position
 *
 * @return uint8_t translated pixel position in neoPixel strip
 */
uint8_t OffsetRightPos(uint8_t v_pos){
  // globals:
  // rightOff position of "top" on the physical neoPixel ring
   return (RING_SIZE + NormalizeRingPos( v_pos + rightOff));
}


// leftOff position of


/**
 * Turn all the LEDs off
 * @param boolean show true = display immediately. false = don't call show()
 */
void ClearRings(boolean show){
    SolidRing(0, show);
}

/**
 * Turn all the LEDs same color
 * @param uint32_t c packed 32-bit color for neoPixel library
 * @param boolean show true = display immediately. false = don't call show()
 */
void SolidRing(uint32_t c, boolean show){
    for(i=0; i<MAX_PIXELS; i++) pixels.setPixelColor(i, c);
    if (show){
      pixels.show();
    }
}

void FlashRing(){
  SolidRing(0x222222, true);
  BackgroundDelay(100);
  SolidRing(0, true);
}

void NextColor(){
  // globals:
  // animation_color
        animation_color >>= 8;                 // Next color R->G->B
      if(!animation_color) animation_color = 0xFF0000; // Reset to red
}

uint8_t NormalizeRingPos(uint8_t realPos){

  while (realPos < 0) { realPos += 16;}
  while (realPos > 15) { realPos -= 16; }
  return realPos;
}

uint8_t ReflectVertical(uint8_t pos1){
}

uint8_t RingDistance(int8_t pos1, int8_t pos2){
  // given two points on the NeoPixel Ring, return
  // unsigned distance between them.
  int8_t retVal= abs(pos1-pos2);
  if( retVal>8){retVal = 16-retVal;}
  return retVal;
}

 uint8_t SteppedColor(){
   // return a non-continuous value for a color axis
   return random(4)*64;
 }
