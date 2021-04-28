/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for additional acks.
*/

/*
struct touch_buttons {
	Adafruit_FreeTouch touch;
	bool last_state=false;
	bool current_state=false;
	int value=0;
	bool pressed_once=false;
} buttons[4];*/

/*
Adafruit_FreeTouch touch_up = Adafruit_FreeTouch(A2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch touch_down = Adafruit_FreeTouch(A3, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch touch_okay = Adafruit_FreeTouch(A4, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch touch_cancel = Adafruit_FreeTouch(A5, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);*/


void init_cap_touch() {
	buttons[UP].touch     = Adafruit_FreeTouch(A2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
	buttons[DOWN].touch   = Adafruit_FreeTouch(A3, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
	buttons[OKAY].touch   = Adafruit_FreeTouch(A4, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
	buttons[CANCEL].touch = Adafruit_FreeTouch(A5, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

	Serial.println(F("Init FreeTouch..."));
	for(int x=0; x<4; x++) {
		if (! buttons[x].touch.begin()) {
			Serial.print(F("Failed to begin "));
			Serial.println(x);
		} 
	}
	Serial.println(F("...done"));
/*
  Serial.println(F("Init FreeTouch..."));
  if (! touch_up.begin())
    Serial.println(F("Failed to begin qt for up"));
  if (! touch_down.begin())
    Serial.println(F("Fail5d to begin qt for down"));
  if (! touch_okay.begin())
    Serial.println(F("Failed to begin qt for okay"));
  if (! touch_cancel.begin())
    Serial.println(F("Failed to begin qt for cancel"));
  Serial.println(F("done."));	

*/
}

void process_touch() {
	int result=0;

	for (int x=0; x<4; x++) {
		buttons[x].value = buttons[x].touch.measure();
		if (buttons[x].value >= touch_threshold) {
			buttons[x].current_state = true;
		} else {
			buttons[x].current_state = false;
		}
		// has the button been pressed down?
		if (buttons[x].last_state != buttons[x].current_state) {
			if (buttons[x].current_state == true) {
				// pressed the button
				buttons[x].pressed_once = true;
			}
		}
	}
}