/**************************************************************************
       Title:   Touch Demo 2
      Author:   Bruce E. Hall, w8bh.net
        Date:   10 Apr 2021
    Hardware:   Blue Pill Microcontroller, 2.8" ILI9341 TFT display,
    Software:   Arduino IDE 1.8.13; STM32 from github.com/SMT32duino
                TFT_eSPI (install from IDE)
       Legal:   Copyright (c) 2021  Bruce E. Hall.
                Open Source under the terms of the MIT License. 
    
 Description:   Builds on Touch Demo 1, adding the ability to determine if
                the touch press lies within a specified region

                An orange region is displayed on the screen.
                Touch the screen, and small circle will appear where you 
                touched.  It circle be green if within the specified region,
                and red if outside the region.

 **************************************************************************/

#include <TFT_eSPI.h>                              // https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI();                         // display object 

typedef struct {
  int x;                                           // x position (left side of rectangle)
  int y;                                           // y position (top of rectangle)
  int w;                                           // width, such that right = x+w
  int h;                                           // height, such that bottom = y+h
} region;

region rOrange = {20,25,200,100};                  // example region

boolean inRegion (region b, int x, int y) {        // true if regsion contains point (x,y)
  if ((x < b.x ) || (x > (b.x + b.w)))             // x coordinate out of bounds? 
    return false;                                  // if so, leave
  if ((y < b.y ) || (y > (b.y + b.h)))             // y coordinate out of bounds?
    return false;                                  // if so, leave 
  return true;                                     // x & y both in bounds 
}

bool touched() {                                   // true if user touched screen     
  const int threshold = 500;                       // ignore light touches
  return tft.getTouchRawZ() > threshold;
}

void markLocation(int x, int y) {
  int color = (inRegion(rOrange,x,y))?             // is x,y within the region?
     TFT_GREEN:TFT_RED;                            // if yes, green circle. if no, red circle
  tft.fillCircle(x,y,6,color);                     // place a small circle at x,y
}

void checkForTouch() {
  uint16_t x, y;
  if (touched()) {                                 // did user touch the display?
    tft.getTouch(&x,&y);                           // get touch coordinates
    markLocation(x,y);                      
    delay(300);                                    // touch debouncer
  }  
}

void setup() {
  tft.init();
  tft.setRotation(1);                              // portrait screen orientation
  tft.fillScreen(TFT_BLACK);                       // clear the screen
  tft.fillRect(rOrange.x, rOrange.y, 
    rOrange.w, rOrange.h, TFT_ORANGE);
}

void loop() {
  checkForTouch();                                 // act on any user touch                                
}
