/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for detailed acks.
*/

#define UP 3
#define DOWN 2
#define OKAY 0
#define CANCEL 1  

// UP = A5
// DOWN = A4
// OKAY = A2
// CANCEL = A3

void init_cap_touch() {
	buttons[DOWN].touch     = Adafruit_FreeTouch(A4, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
	buttons[UP].touch       = Adafruit_FreeTouch(A5, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
	buttons[CANCEL].touch   = Adafruit_FreeTouch(A3, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
	buttons[OKAY].touch     = Adafruit_FreeTouch(A2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

	Serial.println(F("Init FreeTouch..."));
	for(int x=0; x<4; x++) {
		if (! buttons[x].touch.begin()) {
			Serial.print(F("Failed to begin "));
			Serial.println(x);
		} 
	}
	buttons[OKAY].threshold = 425;
	buttons[CANCEL].threshold = 525;
	buttons[DOWN].threshold = 380;
	buttons[UP].threshold = 380;
	Serial.println(F("...done"));
}

void process_touch() {
	int result=0;

	for (int x=0; x<4; x++) {
		buttons[x].value = buttons[x].touch.measure();
		if (millis() - buttons[x].debounce >= 500) {
			if (buttons[x].value >= buttons[x].threshold) {
				buttons[x].current_state = true;
			} else {
				buttons[x].current_state = false;
			}			
		}

		// has the button been pressed down?
		any_press = false;
		if (buttons[x].last_state != buttons[x].current_state) {
			buttons[x].debounce = millis();
			if (buttons[x].current_state == true) {
				// pressed the button
				buttons[x].pressed_once = true;
				any_press = true;
			}
			buttons[x].last_state = buttons[x].current_state;
		}

		if (buttons[UP].pressed_once) {
			buttons[UP].pressed_once = false;
			if (vert_state == BARGRAPH_DISPLAY)
				vert_state = RANDOM_DISPLAY;
			else
				vert_state = BARGRAPH_DISPLAY;
		}

      	if (buttons[DOWN].pressed_once) {
      		buttons[DOWN].pressed_once = false;
      		if (horiz_state == MULTI_DISPLAY)
      			horiz_state = TEMP_DISPLAY;
      		else
      			horiz_state = MULTI_DISPLAY;
      	}

		if (buttons[OKAY].pressed_once) {
			buttons[OKAY].pressed_once = false;
			play_soundeffects();
			Serial.println("OKAY!");
		}

		if (buttons[CANCEL].pressed_once) {
			buttons[CANCEL].pressed_once = false;
			stop_soundeffects();
			Serial.println("CANCEL!");
		}				
/*		if (any_press) {
			play_soundeffects();
		} */
	}
}