/**
 * Constants and macros for the neoGoggles projects
 * @package neogoggles
 * @author Lon Koenig <lon@lonk.me>
 * @license https://opensource.org/licenses/MIT
 */

// code compile-time options:

// if true, compile in the flame effect:
#define USE_FLAME false

// How far to step around the color wheel when we change colors:
#define COLOR_WHEEL_STEP 16

// Describe the hardware:
// Size of neopixel ring:
#define RING_SIZE 16
#define MAX_PIXELS RING_SIZE*2
// Data pin for NeoPixels:
#define NEOPIXEL_PIN 0
#define BACKLIGHT_PIXEL_START MAX_PIXELS
// How many white pixel for backlight
#define BACKLIGHT_PIXELS 1

// Input pins for the button and vibration sensor
// comment out if not available on these goggles
#define BUTTON_PIN 4
#define VIBRATION_PIN 1


//

// NeoPixel Ring Animations:
#define SPARKS_ANIM 0
#define SPINNY_ANIM 1
#define GOOGLY_ANIM 2
#define LARSON_SCANNER 3
#define HALF_BLINK_ANIM 4
#define FLASH_ANIM 5
#define SOLID_ANIM 6
#define COMET_ANIM 7
#define FLAME_ANIM 99

// other animation Constants:
#define FADE_LENGTH 5

// #define PRDBG
// macros:
// create Serial.print macros:
#ifdef PRDBG
#define dprint(ccc) Serial.print(ccc);
#define dprintln(ccc) Serial.println(ccc);
#else
#define dprint(ccc) ""
#define dprintln(ccc) ""
#endif
