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

  switch (which_animation){
    case SPARKS_ANIM:

    break;

    case SOLID_ANIM:
    SolidRing(0x222222, true);
  }
  if (SOLID_ANIM == which_animation){


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
uint8_t delta = 0;
switch(current_animation) {


case COMET_ANIM:
  // streak around the ring with current color
  ClearRings(false);
  for(uint8_t fade_level=0; fade_level<5; fade_level++){
    DrawRingPixel(animation_frame - fade_level , fade_level, true, 2, true);
  }
  if ((++animation_frame)>15) {animation_frame = 0;}
  frame_duration = 40;
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
/*
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

*/
  case SPINNY_ANIM: // Spinny wheels (4 LEDs on at a time)
  // ======================================================
    ClearRings(false);
    DrawRingPixel(animation_frame  , 0, true, 4 , false);
    DrawRingPixel(animation_frame - 1 , 2, true, 4, false);
    DrawRingPixel(animation_frame +1 , 2, true, 4, false);

    if ((++animation_frame)>3) {animation_frame = 0;}
    frame_duration = 80;
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

  ClearRings(false);
  for(i=0; i<RING_SIZE; i++) {
    this_color = 0;
    delta = RingDistance(ring_pos, i);
    if(delta <2 ){
      //  pixels.setPixelColor(    NormalizeRingPos(i+leftOff )  , this_color); // First eye
      //  pixels.setPixelColor( 16 +NormalizeRingPos(i+rightOff) , this_color); // Second eye (not flipped)
    // replace ^
        DrawRingPixel(i  , 1 + delta * 2 , false, 1, false);
    }
  }
  frame_duration = 24;
  pixels_dirty =  true;

  // if inertia settles to low level goose it. --todo
  // randomly add an impulse:
  if (random(60) == 1){
   inertia = inertia + random(800);

  }


  break;
/*
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
*/
/*
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

      frame_duration = 200;
      pixels_dirty =  true;
  }
  break;
*/
#if USE_FLAME
  case FLAME_ANIM: // fire
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
  frame_duration = 22;
  pixels_dirty =  true;

  break;
#endif
} // end the big switch
  BackgroundDelay(frame_duration);
  //BackgroundDelay
  if (pixels_dirty){
    pixels.show();
    pixels_dirty = false;
  }

}

/**
 * Light an LED at specified location on both eyes
 * acccounting for rotation, offsets and reflection
 *
 * @param uint8_t position which pixel to light up
 * @param palette_index index of this color in the palette
 * @param boolean reflection true = mirror to other eye. False copy to other eye
 * @param multiple how many times to draw (recommended values: 1, 2, 4, 8)
 * @param boolean true = alternat color with secondary palette
 */
void DrawRingPixel(
  uint8_t position,
  uint8_t palette_index,
  boolean reflection,
  uint8_t multiple,
  boolean alternate
){
  // globals:
  uint8_t delta = RING_SIZE/multiple;
  uint8_t clone_count;
  uint8_t clone_position;


  for(clone_count = 0; clone_count < multiple; clone_count++){
    this_color = FadedColor((clone_count%2), palette_index); // gives us our 32-bit color
    clone_position = position + clone_count * delta;
    pixels.setPixelColor( OffsetLeftPos(clone_position) , this_color); // left eye
    if (reflection){
        pixels.setPixelColor(  OffsetRightPos((RING_SIZE - clone_position)) , this_color); // right eye
    } else {
      pixels.setPixelColor( OffsetRightPos(clone_position) , this_color); // right eye
    }
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

/**
 * Turn all ring LEDs off
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
/**
 * Specify the animation color (in terms of color wheel index)
 * @param uint8_t new_color color wheen index (8 bit)
 */
void SetAnimationColor(uint8_t new_color){
// globals:
// animation_color

  dprint("SetAnimationColor: ");
  dprintln(new_color);
  animation_color =  NeoWheel(new_color);
  animation_color = new_color; // set the global single color
  CreateFadeValues(new_color); // set faded colors table

}
void NextColor(){
  // globals:
  // animation_color
/*
        animation_color >>= 8;                 // Next color R->G->B
      if(!animation_color) animation_color = 0xFF0000; // Reset to red
*/
  color_wheel_position += COLOR_WHEEL_STEP;
//  if (color_wheel_position > 255) { color_wheel_position = 0;} //no need to wrap with 8-bit var
  SetAnimationColor(color_wheel_position);
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

 /**
  * @param palette numer (0 primary, 1, secondary)
  * @param uint8_t offset into color table
  *
  * @return uint8_t color wheel index
  */
 uint32_t FadedColor(uint8_t  which_palette, uint8_t fade_amount){
   // globals:
   //palette -- array of colors
   return palette[which_palette][fade_amount];
 }

/**
 * initialize the color palette for fade effects
 * @param uint8_t wheel_index color wheel index of starting color
 */
 void CreateFadeValues(uint8_t wheel_index){
  // globals:
  //palette -- array of colors
  uint32_t calc_color;
  uint8_t rgb[3];
  uint8_t channel;
  int channel_value;
  uint8_t palette_index = 0;

  dprint("CreateFadeValues wheel index: ");
  dprintln(wheel_index);

// make 2 palettes:
  while (palette_index < 2){
  calc_color = NeoWheel(wheel_index); // convert index to actual 32-bit color
  // split up original color into channels
  rgb[0] = (uint8_t)(calc_color >> 0);
  rgb[1] = (uint8_t)(calc_color >> 8);
  rgb[2] = (uint8_t)(calc_color >> 16);

  // fill in palette with faded values:
    for(i=0; i<FADE_LENGTH; i++){
      palette[palette_index][i] = pixels.Color(rgb[0], rgb[1], rgb[2]); // base color on current split-up color channels
      // now fade it for next slot in array:
      for(channel=0; channel<3; channel++){ // step through RGB channels
        channel_value = rgb[channel]; // copy it to an int for bigger math
        rgb[channel] = (channel_value * 40) /  100;
      }
    }
    palette_index++;
    wheel_index = wheel_index - 40; // 30 works for sure
  }
}

uint32_t NeoWheel(byte WheelPos) {
/* (From Adafruit sample code)
  Globals:
    pixels Adafruit Neopixel object

  Inputs:
    WheelPos (byte) Position on the color wheel (0 - 255)
    The colours are a transition r - g - b - back to r

  Outputs:
    packed color value as used in Adafruit libraries
*/

/*
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  */
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
  if(WheelPos < 85) {
  //  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    red = WheelPos * 3;
    green = 255 - WheelPos;

  } else if(WheelPos < 170) {
    WheelPos -= 85;
    //  return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    red = 255 - WheelPos * 3;
    blue = WheelPos * 3;

  } else {
    WheelPos -= 170;
    // return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    green = WheelPos * 3;
    blue = 255 - WheelPos * 3;
  }
  return pixels.Color(red, green, blue);
}

/**
 * Turns on the backlight
 * This version is 256-based.
 * Update to 768 when time allows
 *
 * @param uint8_t overall brightness
 */
void SetBacklight( uint8_t brightness, boolean show){
  // globals:
  // pixels - Adafruit neopixel object
  // this_color - temporary color value

  uint8_t white_pos = 0;
  this_color = pixels.Color(brightness,brightness,brightness);
  pixels.setPixelColor( BACKLIGHT_PIXEL_START, this_color );
  pixels.setPixelColor( BACKLIGHT_PIXEL_START - 1, this_color );
  if (show){
    pixels.show();
  }
}
