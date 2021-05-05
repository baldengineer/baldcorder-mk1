/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for detailed acks.
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

void toggle_soundeffects() {
	if (soundeffects_are_active) 
		stop_soundeffects();
	else
		play_soundeffects();
}

void stop_soundeffects() {
	if (soundeffects_are_active) {
		AudioPlayer.stopChannel(1);
		soundeffects_are_active = false;		
	}
}

void play_soundeffects() {
	if (soundeffects_are_active == false) {
		AudioPlayer.play(scanning_sound, 1);
		scanning_last_played = millis();
		soundeffects_are_active = true;
	}
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
	if (force_playing_sound || (millis() - scanning_last_played >= scanning_sound_interval)) {
		AudioPlayer.play(scanning_sound, 1); // should 10min file..
		scanning_last_played = millis();
		force_playing_sound = false;
		soundeffects_are_active = true;
	}
}