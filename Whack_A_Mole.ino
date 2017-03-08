#include <delay.h> 
#include <FillPat.h> 
#include <LaunchPad.h> 
#include <OrbitBoosterPackDefs.h> 
#include <OrbitOled.h> 
#include <OrbitOledChar.h> 
#include <OrbitOledGrph.h> 
#include <stdlib.h>

#define SWT_LEFT PA_6
#define SWT_RIGHT PA_7
#define BTN_TOP PE_0
#define BTN_BOTTOM PD_2

enum Position {LEFT, RIGHT, TOP, BOTTOM}; 
enum Page {TITLE, GAME, RESULT}; 

void WireInit();
void ShakeInit();
void ShakeTick();


Page currentPage = Page::TITLE; 

void setup() {
  OrbitOledInit(); 

  WireInit();
  Serial.begin(9600);
  delay(100);
  ShakeInit();

  pinMode(SWT_RIGHT, INPUT);
  pinMode(SWT_LEFT, INPUT);
  Serial.begin(9600);
  srand(millis());
  //Serial.println((int)analogRead(0));
}

void loop() {
  ShakeTick();
  if (currentPage == Page::TITLE)
  {
    pageTitle(); 
  }
  else if (currentPage == Page::GAME)
  {
    pageGame(); 
  }
  else if (currentPage == Page::RESULT)
  {
    pageResult(); 
  }
}

