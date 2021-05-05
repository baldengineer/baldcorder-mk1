/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for additional acks.
*/

// ****************************
// General
// ****************************
#include <Wire.h>
#include <SPI.h>
#include <avr/dtostrf.h>


const uint8_t enable_5v_pin = 13;
bool power_state = true;
bool previous_power_state = false;
bool any_press = false; // to detect any button press


// ****************************
// Sound Libraries
// ****************************
#include <SdFat.h>
#include "SamdAudio.h"
SdFat memory;
SamdAudio AudioPlayer;
#define NUM_AUDIO_CHANNELS 2 //could be 1,2 or 4 for sound
#define AUDIO_BUFFER_SIZE 512 //512 works fine for 22.05kh, 1024 for 32khz and 44.1khz for Adafruit M0 with QUAD Flash//indicate sample rate here (use audacity to convert your wav)
const unsigned int sampleRate = 22050; //hz - for 1s file it is 40960
const char *scanning_sound = "tng_14m.wav";
#define YOUR_SD_CS 28 // CS on MKRZERO is "pin" 28
bool force_playing_sound = false;
bool soundeffects_are_active = false;

unsigned long scanning_last_played = 0;
unsigned long scanning_sound_interval = (1000UL * 60UL * 10UL);

// ****************************
// VL53L0X
// ****************************
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
unsigned long last_prox_check = 0;
unsigned long prox_check_interval = 1000;
long prox_reading = 0; // -1 is out of range
unsigned long prox_closed_threshold = 60;



// ****************************
// Capacitive Touch
// ****************************
#include "Adafruit_FreeTouch.h"
//enum functions {DOWN=0, UP=1, CANCEL=2, OKAY=3};
struct touch_buttons {
	Adafruit_FreeTouch touch;
	bool last_state=false;
	bool current_state=false;
	int value=0;
	int threshold=0;
	bool pressed_once=false;
	unsigned long debounce;
} buttons[4];

int touch_threshold = 425;

// ****************************
// "Light" Sensor -- Modified Phototransistor
// ****************************
const byte light_sensor_pin = A6;
unsigned long light_reading = 0;
unsigned long last_light_read = 0;
unsigned long light_read_interval = 250;

// ****************************
// Temperature Sensor - 
// ****************************
const byte temp_sensor_pin = A1;
double temp_reading = 0.0;
unsigned long last_temp_read = 0;
unsigned long temp_read_interval = 500;

// ****************************
// Display libraries
// ****************************
#include <Adafruit_GFX.h>   
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

unsigned long oled_last_update = 0;
unsigned long oled_update_interval = 1000;

// Large Center Display
#define OLED64_SCREEN_WIDTH 128 // OLED display width, in pixels
#define OLED64_SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED64_RESET -1  // no has this
Adafruit_SSD1306 oled64(OLED64_SCREEN_WIDTH, OLED64_SCREEN_HEIGHT, &Wire, OLED64_RESET);

// Vertical Display 
#define OLED32_DC     7
#define OLED32_RESET  6 // make -1 when second in place
#define VOLED32_CS    14
//#define HOLED32_CS	  14 // need to verify pin
#define OLED32_SCREEN_WIDTH 128 // OLED display width, in pixels
#define OLED32_SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 voled32(OLED32_SCREEN_WIDTH, OLED32_SCREEN_HEIGHT, &SPI, OLED32_DC, OLED32_RESET, VOLED32_CS);
//Adafruit_SSD1306 holed32(OLED32_SCREEN_WIDTH, OLED32_SCREEN_HEIGHT, &SPI, OLED32_DC, -1, HOLED32_CS);

#define BARGRAPH_DISPLAY 0
#define MULTI_DISPLAY 1
#define TEMP_DISPLAY 2
#define RANDOM_DISPLAY 3

uint8_t horiz_state = MULTI_DISPLAY;
uint8_t vert_state = BARGRAPH_DISPLAY;


// ****************************
// GNDN Blinky Lights
// ****************************
#include <Adafruit_NeoPixel.h>

#define NEO_PIN   5
#define NEO_COUNT 9

uint8_t neo_brightness = 128; // remember, the battery!
uint32_t neo_scan_speed = 50; // millisecond wait time
uint32_t previous_neo_scan = 0;
uint8_t pulser_step_size = 5;
#define SCAN_COLOR strip.Color(0,127,0)

Adafruit_NeoPixel strip(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);


// ****************************
// Debug related stuff
// ****************************
unsigned long print_last_millis = 0;
unsigned long print_interval = 1000;