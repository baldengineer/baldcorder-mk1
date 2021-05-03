/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for detailed acks.
*/

void init_gndn_blinky() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(neo_brightness); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void turn_off_neopixels() {
  strip.clear();
  strip.show();
}

void process_tricorder_chase() {
  // based on the Adafruit theatreChase() example, converted to use millis() by James
  static uint8_t beat_counter = 0;
  static int8_t pulser_brightness = 0;
  static bool pulser_direction = true;
  static uint8_t pulser_color = 0;
  uint8_t pulser_location = (NEO_COUNT-1);


  if (millis() - previous_neo_scan >= neo_scan_speed) {
    strip.clear();         //   Set all pixels in RAM to 0 (off)

    // handle the chasing sensor bar
    // 'c' counts up from 'b' to end of strip in steps of 4...
    for (int c = beat_counter; c < strip.numPixels(); c += 4) {
      strip.setPixelColor(c, SCAN_COLOR); // Set pixel 'c' to value 'color'
    }
    beat_counter++;
    if (beat_counter >= 4)
      beat_counter = 0;

    // now handle the stand alone (last one on the chain)
    if (pulser_direction) {
      pulser_brightness += pulser_step_size;
      if (pulser_brightness >= 125) {
        pulser_brightness = 125;
        pulser_direction = false;
        pulser_color++;
      }
    } else {
      pulser_brightness -= pulser_step_size;
      if (pulser_brightness <= 0) {
        pulser_brightness = 0;
        pulser_direction = true;
        pulser_color++;
      }
    }
    // clamp the color selector
    if (pulser_color >= 3)
      pulser_color =0;

    switch (pulser_color) {
      case 0:
        // red
        strip.setPixelColor(pulser_location, strip.Color(pulser_brightness,0,0));
        break;
      case 1:
        // green
        strip.setPixelColor(pulser_location, strip.Color(0,pulser_brightness,0));
        break;

      case 2:
        //blue
        strip.setPixelColor(pulser_location, strip.Color(0,0,pulser_brightness));
        break;
    }


    AudioPlayer.criticalON();
    strip.show(); // Update strip with new contents
    AudioPlayer.criticalOFF();

    previous_neo_scan = millis();
  }
}
