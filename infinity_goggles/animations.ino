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
      ClearRings(false);
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
int delta = 0;
int calc_pos = 0;
uint32_t this_color = 0;
switch(current_animation) {

case JUST_ZERO:
    ClearRings(false);
      DrawRingPixel(0 , 1, false, 1, false);
      pixels_dirty =  true;

break;

case COMET_ANIM:
  //  2 dots with tails streak around the ring with current color
  ClearRings(false);
  for(uint8_t fade_level=0; fade_level<5; fade_level++){
    DrawRingPixel(animation_frame - fade_level , fade_level, true, 2, true);
  }
  if ((++animation_frame) >  (RING_SIZE-1)) {animation_frame = 0;}
  frame_duration = 40;
  pixels_dirty =  true;
break;


 case SPARKS_ANIM: // Random sparks - just one LED on at a time
 // ======================================================
  i = random(MAX_PIXELS);

  pixels.setPixelColor(i, pixels.Color(RandomSteppedColor(), RandomSteppedColor(), RandomSteppedColor() ));
  pixels.show();
  BackgroundDelay(10);
  pixels.setPixelColor(i, 0);
  frame_duration = 5;
  pixels_dirty =  true;
  break;

  case SPINNY_ANIM: // Spinny wheels (4 pointed star)
  // ======================================================
  #define STAR_POINTS 5
    ClearRings(false);
    DrawRingPixel(animation_frame  , 0, true, STAR_POINTS , false);
    DrawRingPixel(animation_frame - 1 , 2, true, STAR_POINTS, false);
    DrawRingPixel(animation_frame +1 , 2, true, STAR_POINTS, false);

    if ((++animation_frame) >= STAR_POINTS) {animation_frame = 0;}
    frame_duration = 80;
    pixels_dirty =  true;
  break;




    case LARSON_SCANNER: // larson scanner:
// ======================================================
    ClearRings(false);
  delta = 1;
  if (animation_frame < (RING_SIZE )){
      if (animation_frame < RING_SIZE / 2){
        calc_pos = horiz_to_pos[animation_frame][0];
        this_color = FadedColor(0, 0); // gives us our 32-bit color
        pixels.setPixelColor( OffsetLeftPos(calc_pos) , this_color); // left eye
      } else {
        calc_pos = horiz_to_pos[animation_frame - (RING_SIZE / 2)][0];
        this_color = FadedColor(0, 0); // gives us our 32-bit color
        pixels.setPixelColor( OffsetRightPos(calc_pos) , this_color); // right eye

      }
  } else {
    if (animation_frame < ((RING_SIZE / 2) *3) ){
      calc_pos = horiz_to_pos[((RING_SIZE / 2) *3) - animation_frame][0];
      this_color = FadedColor(0, 0); // gives us our 32-bit color
      pixels.setPixelColor( OffsetRightPos(calc_pos) , this_color); // left eye
    } else {
      calc_pos = horiz_to_pos[(RING_SIZE * 2) - animation_frame][0];
      this_color = FadedColor(0, 0); // gives us our 32-bit color
      pixels.setPixelColor( OffsetLeftPos(calc_pos) , this_color); // right eye

    }

  }


    if ((++animation_frame) >= (RING_SIZE  * 2 ) ) {animation_frame = 0;}
    frame_duration = 1000;
    pixels_dirty =  true;
  break;




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
 * @param boolean true = alternate color with secondary palette
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
 * @param int v_pos current (virtual) position
 *
 * @return uint8_t translated pixel position in neoPixel strip
 */
uint8_t OffsetLeftPos(int v_pos){
  // globals:
  // leftOff position of "top" on the physical neoPixel ring
   return (NormalizeRingPos( v_pos + leftOff));
}

/**
 * Convert virtual pixel position to real-world hardware position
 *
 * @param int v_pos current (virtual) position
 *
 * @return uint8_t translated pixel position in neoPixel strip
 */
uint8_t OffsetRightPos(int v_pos){
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
  // color_wheel_position
  color_wheel_position += COLOR_WHEEL_STEP;
  SetAnimationColor(color_wheel_position);
}

uint8_t NormalizeRingPos(int realPos){
  while (realPos < 0) { realPos += RING_SIZE;}
  while (realPos > (RING_SIZE-1)) { realPos -= RING_SIZE; }
  return realPos;
}

uint8_t ReflectVertical(uint8_t pos1){
}

uint8_t RingDistance(int8_t pos1, int8_t pos2){
  // given two points on the NeoPixel Ring, return
  // unsigned distance between them.
  int8_t retVal= abs(pos1-pos2);
  if( retVal > (RING_SIZE/2)){retVal = RING_SIZE-retVal;}
  return retVal;
}

 uint8_t RandomSteppedColor(){
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
