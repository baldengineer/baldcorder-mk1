/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for detailed acks.
*/

void init_vl53l0x() {
  Serial.print(F("Starting VL53L0X ..."));
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    power_state = true;
    // while(1); 
  }
  Serial.println(F("done!"));
}

void process_prox_sensor() {

	VL53L0X_RangingMeasurementData_t measure;

	if (millis() - last_prox_check >= prox_check_interval) {	

	 	lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

		if (measure.RangeStatus != 4) {  // phase failures have incorrect data
			prox_reading = measure.RangeMilliMeter;
		} else {
			prox_reading -1;
		}

		if (prox_reading > prox_closed_threshold) {
			// your flap is open and draining power
			power_state = true;
			/*if (power_state != previous_power_state) {
				force_playing_sound = true;
			}*/
		} else {
			power_state = false;
		//	if (power_state != previous_power_state) {
				//AudioPlayer.stopChannel(1);
				//soundeffects_are_active = false;
				stop_soundeffects();
		//	}
		}

		previous_power_state = power_state;

		last_prox_check = millis();

	}
}

void process_light_reading() {
	if (millis() - last_light_read >= light_read_interval) {
		// TODO add min/max/avg value
		light_reading = analogRead(light_sensor_pin);
		last_light_read = millis();
	}
}


void process_temp_reading() {
	const double Voc = 0.4; // 0.4 from the datasheet
	const double Tc = 0.0195; // 0.0195 from the datasheet

	// todo add modified moving average to clean up reading
	if (millis() - last_temp_read >= temp_read_interval) {
		uint32_t adc_reading = analogRead(temp_sensor_pin);
		//Serial.print("*** Temp ADC: ");
	   // Serial.println(adc_reading);
	    double Vout = adc_reading * (3.3 / 1023.0); //
	    double Ta = (Vout - Voc) / Tc;
	    temp_reading = Ta;

		last_temp_read = millis();
	}
}