

//#define PRDBG
// macros:
// create Serial.print macros:
#ifdef PRDBG
#define dprint(ccc) Serial.print(ccc);
#define dprintln(ccc) Serial.println(ccc);
#else
#define dprint(ccc) ""
#define dprintln(ccc) ""
#endif



// NeoPixel Ring Animations:
#define SPARKS_ANIM 0
#define SPINNY_ANIM 1
#define GOOGLY_ANIM 2
#define LARSON_SCANNER 3
#define HALF_BLINK_ANIM 4
#define FLASH_ANIM 5
#define SOLID_ANIM 6
