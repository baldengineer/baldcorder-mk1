/* Created by James Lewis, 2021
For element14 Present's Build Inside the Box Competiton
MIT License
Please see Acknowledgements.md for additional acks.
*/

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

void print_threeline_1306(Adafruit_SSD1306 *disp, char *line1, char *line2, char *line3, uint16_t color1) {
  // for text bounding box
  int16_t  x1, y1;
  uint16_t w, h;
  
  disp->clearDisplay();  
  disp->setTextColor(color1);
  disp->getTextBounds(line1, 0, 0, &x1, &y1, &w, &h); // how much space we need
  disp->setCursor(0,h); // 0,0 is bottom left of text. 0,h is offsets enough for the text
  disp->println(line1); 
  disp->println(line2);
  disp->print(line3);
//  AudioPlayer.criticalON();
  disp->display();
//  AudioPlayer.criticalOFF();
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
  oled64.clearDisplay();
  oled64.display();
 // char msg[] = {"Hello"};
 // print_oneline_1306(oled64, msg, SSD1306_WHITE);
}