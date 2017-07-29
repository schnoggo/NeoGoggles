/**
 * Constants and macros for the neoGoggles projects
 * @package neogoggles
 * @author Lon Koenig <lon@lonk.me>
 * @license https://opensource.org/licenses/MIT
 */

// code compile-time options:
#define USE_FLAME false
// if true, compile in the flame effect

// Describe the hardware:
#define MOMENTARY 1
#define VIBRATION 2
#define SWITCH_STYLE MOMENTARY
// Switch styles will be MOMENTARY | VIBRATION (Don't use 0 for definitions)



#define RING_SIZE 16
#define MAX_PIXELS RING_SIZE*2
#define NEOPIXEL_PIN 8

#define BUTTON_PIN 3

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

#define PRDBG
// macros:
// create Serial.print macros:
#ifdef PRDBG
#define dprint(ccc) Serial.print(ccc);
#define dprintln(ccc) Serial.println(ccc);
#else
#define dprint(ccc) ""
#define dprintln(ccc) ""
#endif
