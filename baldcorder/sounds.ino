/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for additional acks.
*/

void init_sdcard() {
	Serial.print(F("Opening SD Card..."));
	if ( !memory.begin(YOUR_SD_CS) )  {
    	//the sd card error was found, exit this function and to not attempt to finish
		Serial.println(F("failed, can't start card"));
		while (1);
	}
	Serial.println(F("Done!"));
}

void init_soundeffects() {
	Serial.print(F("Initializing Audio Player..."));
	if (AudioPlayer.begin(sampleRate, NUM_AUDIO_CHANNELS, AUDIO_BUFFER_SIZE) == -1)   {
		Serial.println(F(" failed!"));
		return;
	}
	AudioPlayer.play(scanning_sound, 1); // should 10min file..
	scanning_last_played = millis();
	Serial.println(F(" done."));
}

void process_scanner_sound() {
	if (millis() - scanning_last_played >= scanning_sound_interval) {
		AudioPlayer.play(scanning_sound, 1); // should 10min file..
		scanning_last_played = millis();
	}
}