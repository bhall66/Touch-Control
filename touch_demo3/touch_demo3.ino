/**************************************************************************
       Title:   Touch Demo 3
      Author:   Bruce E. Hall, w8bh.net
        Date:   10 Apr 2021
    Hardware:   Blue Pill Microcontroller, 2.8" ILI9341 TFT display,
    Software:   Arduino IDE 1.8.13; STM32 from github.com/SMT32duino
                TFT_eSPI (install from IDE)
       Legal:   Copyright (c) 2021  Bruce E. Hall.
                Open Source under the terms of the MIT License. 
    
 Description:   Builds on Touch Demo 2, adding multiple regions to a screen
                and determining which region was touched.

                Muliple blue rectangular regions are displayed on screen.
                Touch one of the regions, and it will flash white.

 **************************************************************************/

#include <TFT_eSPI.h>                              // https://github.com/Bodmer/TFT_eSPI
TFT_eSPI tft = TFT_eSPI();                         // display object 
#define ELEMENTS(x) (sizeof(x) / sizeof(x[0]))     // Handy macro for determining array sizes

typedef struct {
  int x;                                           // x position (left side of rectangle)
  int y;                                           // y position (top of rectangle)
  int w;                                           // width, such that right = x+w
  int h;                                           // height, such that bottom = y+h
} region;

region rScreen[] = {                               // Multiple-region Clock Screen:
  {1,1,320,35},                                    // Title bar region
  {20,50,200,100},                                 // Time region
  {240,60,80,35},                                  // Time Zone region
  {240,110,80,35},                                 // AM/PM screen region
  {1,180,140,40},                                  // Clock status1 region
  {180,180,140,40}                                 // Clock status2 region
};

void fillRegion (int ID, int color) {              // display a region on screen
  tft.fillRect(rScreen[ID].x,rScreen[ID].y,
    rScreen[ID].w, rScreen[ID].h, color);    
}

void displayScreenRegions() {                      // display all regions on screen 
  for (int i=0; i<ELEMENTS(rScreen); i++)
    fillRegion(i,TFT_BLUE);  
}

int regionID(int x, int y) {                       // return region# of touch (-1=none)
  for (int i=0; i<ELEMENTS(rScreen); i++)
    if (inRegion(rScreen[i],x,y))
      return i;
  return -1;
}

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

void handleTouchEvent (int x, int y) {
  int ID = regionID(x,y);                          // which region was pressed?
  if (ID<0) return;                                // press wasnt inside a region
  switch (ID) {      
    case 0: ;                                      // code for region #0   
    case 1: ;                                      // code for region #1
    case 2: ;                                      // code for region #2
    default: {                                     // but for demonstration purposes only:
      fillRegion(ID,TFT_WHITE);                    // turn the region white
      delay(500);                                  // for half a second,
      fillRegion(ID,TFT_BLUE);                     // then back to blue
    }
  }
}

void checkForTouch() {
  uint16_t x, y;
  if (touched()) {                                 // did user touch the display?
    tft.getTouch(&x,&y);                           // get touch coordinates
    handleTouchEvent(x,y);                                            
    delay(300);                                    // touch debouncer
  }  
}

void setup() {
  tft.init();
  tft.setRotation(1);                              // portrait screen orientation
  tft.fillScreen(TFT_BLACK);                       // clear the screen
  displayScreenRegions();
}

void loop() {
  checkForTouch();                                 // act on any user touch                                
}
