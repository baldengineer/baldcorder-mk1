/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for detailed acks.
*/

#include "baldcorder.h"

// temporary flags
#define BARGRAPH_DISPLAY true
#define TEMPATURE_DISPLAY false

void process_serial_prints() {
	if (millis() - print_last_millis >= print_interval) {
		Serial.println("----------------");
		Serial.print(F("Temp: "));
		Serial.print(temp_reading);
		Serial.println(F("C"));
		Serial.print(F("Light: "));
		Serial.println(light_reading);
		Serial.println("Buttons: "); // doesn't work, using pressed_once now.
		for (int x=0; x<4; x++) {
			Serial.print(" ");
			Serial.print(x);
			Serial.print(": ");
			Serial.println(buttons[x].value);
//			if (buttons[x].pressed_once == true) {
//				Serial.print(x);
//				buttons[x].pressed_once = false;
//			}
		}
		Serial.println("!");

		Serial.flush();
		print_last_millis = millis();
	}
}

void power_change() {
	if (power_state) {
		digitalWrite(enable_5v_pin, HIGH);
	} else {
		digitalWrite(enable_5v_pin, LOW);
	}
}

void setup() { 
	pinMode(enable_5v_pin, OUTPUT);
	power_state = false; // turn off the regulator
	power_change();
	Serial.begin(115200);

	while ( (millis() < 3000) && (!Serial) );
//	delay(2500);
//	while (!Serial);

	power_state = true; // turn on the regulator
	power_change();

	init_sdcard(); // get ready for sound effects!

	init_soundeffects();
	init_gndn_blinky();

	init_voled32(); 	// vertical 128x32 (alpha, beta, gamma)
	//init_holed32(); 	// horizontal 128x32 
	init_oled64(); 	// center display
	init_cap_touch();
	init_vl53l0x();
}

void loop() {
	// check the proxmimity sensor
	process_prox_sensor();
//	power_state = true;
	
	if (power_state) {
	//	power_change();
		process_touch();
		process_temp_reading();
		process_light_reading();
		process_oleds();
	// update oled64
	// update the hozitonal oled32
		process_tricorder_chase();
		process_scanner_sound();
		process_serial_prints();
	} else {
	//	power_change();
		turn_off_oled(&voled32);
		//turn_off_oled(&holed32);
		turn_off_oled(&oled64);
		turn_off_neopixels();
		stop_soundeffects();
		
		delay(1000); // should be a sleep
	}

}