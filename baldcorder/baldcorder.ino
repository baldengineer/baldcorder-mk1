/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for additional acks.
*/

#include "baldcorder.h"

// temporary flags
#define BARGRAPH_DISPLAY true
#define TEMPATURE_DISPLAY false

void process_oleds() {
	static bool vert_state = TEMPATURE_DISPLAY;
	if (millis() - oled_last_update >= oled_update_interval) {
		// vertical oled
		char msg1[16];
		char msg2[16];
		char msg3[16];
	   	
		if (vert_state == TEMPATURE_DISPLAY) {
			dtostrf(temp_reading, 5, 2, msg1);
		    print_oneline_1306(&voled32, msg1, SSD1306_WHITE);	   		
	   	} else {
			int graphy = map(light_reading, 0, 1024, 0, 100);
		    draw_bargraph(&voled32, graphy, SSD1306_WHITE);
		}
	//	if (button_states[2] == true) 
	//		vert_state = !vert_state;
		
		// center oled
		dtostrf(temp_reading, 5, 2, msg1);
		sprintf(msg2, "%d", light_reading);
		sprintf(msg3, "%d", millis());
		print_threeline_1306(&oled64, msg1, msg2, msg3, SSD1306_WHITE);	

		oled_last_update = millis();
	}
}

void process_serial_prints() {
	if (millis() - print_last_millis >= print_interval) {
		Serial.println("**************");
		Serial.print(F("Temp: "));
		Serial.print(temp_reading);
		Serial.println(F("C"));
		Serial.print(F("Light: "));
		Serial.println(light_reading);
		Serial.print("Buttons: ");
	/*	for(int x=0; x<4; x++) {
			if (button_states[x] == true) {
				//button_states[x] = false;
				Serial.print(x);
			}
		}*/
		Serial.println("!");

		Serial.flush();
		print_last_millis = millis();
	}
}

void setup() { 
	Serial.begin(115200);
	while ( (millis() < 3000) || (!Serial) );

	init_sdcard(); // get ready for sound effects!
	init_soundeffects();

	// vertical 128x32 (alpha, beta, gamma)
	init_voled32();
	// horzitonal 128x32 (geo, med, )
	// TODO: Up there, that one!!
	// center display
	init_oled64();

	init_cap_touch();
}

void loop() {
	// check the proxmimity sensor
	
	process_touch();

	process_temp_reading();
	process_light_reading();

	process_oleds();
	// update oled64
	// update the hozitonal oled32

	process_scanner_sound();

	process_serial_prints();
}