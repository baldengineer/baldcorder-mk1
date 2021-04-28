/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for additional acks.
*/

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