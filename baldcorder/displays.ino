/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for detailed acks.
*/


//***********************
// 128x32 (Vertical Display) Alpha/Beta/Delta/Gamma
//***********************
void init_voled32() {
  Serial.print(F("Init OLED32 display..."));
  if (!voled32.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("Vertical SSD1306 allocation failed"));
    while(1);
  }
  Serial.println(F("Done!"));

  voled32.setFont(&FreeSansBold18pt7b); // should be 35px high
  voled32.clearDisplay();
  voled32.display();
  //char msg[] = {"Hello"};
  //print_oneline_1306(voled32, msg, SSD1306_WHITE);
}


//***********************
// 128x32 (Horizontal Display) Temperature Read-Out
//***********************
void init_holed32() {
  Serial.print(F("Init Hori OLED32 display..."));
  if (!holed32.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("Horizontal OLED32 allocation failed"));
    while(1);
  }
  Serial.println(F("Done!"));

  holed32.setFont(&FreeSansBold18pt7b); // should be 35px high
  holed32.clearDisplay();
  holed32.display();
  //char msg[] = {"Hello"};
  //print_oneline_1306(voled32, msg, SSD1306_WHITE);
}

//***********************
// 128x64 (Center Display)
//***********************
void init_oled64() {
  Serial.print(F("Init OLED64 display..."));
  if (!oled64.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED64 allocation failed"));
    while(1); 
  }
  Serial.println(F("Initialized"));

 // oled64.setFont(&FreeSansBold12pt7b); 
  oled64.setFont(&FreeSans9pt7b);
  oled64.setRotation(2); // which way is up?
  oled64.clearDisplay();
  oled64.display();
 // char msg[] = {"Hello"};
 // print_oneline_1306(oled64, msg, SSD1306_WHITE);
}

void print_oneline_1306(Adafruit_SSD1306 *disp, char *line1, uint16_t color1) {
  // for text bounding box
  int16_t  x1, y1;
  uint16_t w, h;
  
  disp->clearDisplay();
  disp->setTextColor(color1);
  disp->getTextBounds(line1, 0, 0, &x1, &y1, &w, &h); // how much space we need
  disp->setCursor( (((disp->width()-1)-w)/2), (((disp->height()-1)-h)/2)+h ); // rj math says 18pt = 35px
  disp->print(line1); // 0,0 is bottom left
 // AudioPlayer.criticalON();
  disp->display();
 // AudioPlayer.criticalOFF();
}

void print_threeline_1306(Adafruit_SSD1306 *disp, char *line1, char *line2, char *line3, uint16_t color1) {
  // for text bounding box
  int16_t  x1, y1;
  uint16_t w, h;
  
  disp->clearDisplay();  
  disp->setTextColor(color1);
  disp->getTextBounds(line1, 0, 0, &x1, &y1, &w, &h); // how much space we need

 /* disp->setCursor(4,h+2); // 0,0 is bottom left of text. offet accounts for bezel
  disp->print(line1); 

  disp->setCursor(4,(h+h+8)+2); // 0,0 is bottom left of text. offet accounts for bezel
  disp->print(line2);

  disp->setCursor(4,(h+h+h+8)+2); // 0,0 is bottom left of text. offet accounts for bezel
  disp->print(line3);*/

  disp->setCursor(0,h+2); // 0,0 is bottom left of text. offet accounts for bezel
  disp->print(" ");
  disp->println(line1); 
  disp->print(" ");
  disp->println(line2); 
  disp->print(" ");
  disp->print(line3); 


//  AudioPlayer.criticalON();
  disp->display();
//  AudioPlayer.criticalOFF();
}

void draw_bargraph(Adafruit_SSD1306 *disp, int amt, int color) {
  // bound the value
  if (amt < 0) amt = 0;
  if (amt > 100) amt = 100;

  int rect_width = map(amt, 0, 100, 0, (disp->width()));
  disp->clearDisplay();
  disp->drawRect(0,0, rect_width, disp->height(), color);
  disp->fillRect(0,0, rect_width, disp->height(), color);
  disp->display();
}

void turn_off_oled(Adafruit_SSD1306 *disp) {
  disp->clearDisplay();
  disp->display();
}

void process_oleds() {
  static bool vert_state = TEMPATURE_DISPLAY;
  if (millis() - oled_last_update >= oled_update_interval) {
    // vertical oled
    char msg1[16];
    char msg2[16];
    char msg3[16];

    // horizontal
    dtostrf(temp_reading, 5, 2, msg1);
    print_oneline_1306(&holed32, msg1, SSD1306_WHITE);      

    // vertical
   int graphy = map(light_reading, 0, 1024, 0, 100);
   draw_bargraph(&voled32, graphy, SSD1306_WHITE);      
/*  if (vert_state == TEMPATURE_DISPLAY) {
      dtostrf(temp_reading, 5, 2, msg1);
        print_oneline_1306(&holed32, msg1, SSD1306_WHITE);        
      } else {
      int graphy = map(light_reading, 0, 1024, 0, 100);
        draw_bargraph(&holed32, graphy, SSD1306_WHITE);
    }
*/
  //  if (button_states[2] == true) 
  //    vert_state = !vert_state;
    
    // center oled
    dtostrf(temp_reading, 5, 2, msg1);
    sprintf(msg2, "%d", light_reading);
    sprintf(msg3, "%d", millis());
    print_threeline_1306(&oled64, msg1, msg2, msg3, SSD1306_WHITE); 

    oled_last_update = millis();
  }
}
