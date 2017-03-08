#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

#define bmpWidth 32
#define bmpHeight 16

#define splatWidth 80
#define splatHeight 24

// Sets the position for drawing a mole and hole
void setPosition(Position p)
{
  switch(p)
  {
    case TOP:
      OrbitOledMoveTo(ccolOledMax / 2 - bmpWidth / 2 - 4, 0);
      break;
    case BOTTOM:
      OrbitOledMoveTo(ccolOledMax / 2 - bmpWidth / 2 + 4, crowOledMax/ 2);
      break;
    case LEFT:
      OrbitOledMoveTo(4, crowOledMax / 2 - bmpHeight / 2 + 2);
      break;
    case RIGHT:
      OrbitOledMoveTo(ccolOledMax - 4 - bmpWidth, crowOledMax / 2 - bmpHeight / 2 - 2);
      break;
  }
}

// Erases a 32 x 16 block at p and draws a hole
void drawHole(Position p)
{
  OrbitOledSetDrawMode(modOledSet);
  setPosition(p);
  OrbitOledPutBmp(bmpWidth, bmpHeight, holeBmp);
  OrbitOledUpdate();
}

// Parameters:
// p        Which hole to draw the mole on
// stage    Which frame of the mole (0 for first frame)
void drawMole(Position p, int stage)
{
  OrbitOledSetDrawMode(modOledOr);
  setPosition(p);
  if (stage == 0)
    OrbitOledPutBmp(bmpWidth, bmpHeight, moleBmpHalf);
  else
    OrbitOledPutBmp(bmpWidth, bmpHeight, moleBmp);
  OrbitOledUpdate();
}

void animateMolePopUp(Position p)
{
  drawMole(p, 0);
  delay(30);
  drawHole(p);
  drawMole(p, 1);
}

void animateMoleHit(Position p)
{
  setPosition(p);
  OrbitOledSetDrawMode(modOledSet);
  drawHole(p);
  OrbitOledDrawString("HIT!");
  OrbitOledUpdate();
  delay(90);
  drawHole(p);
}

void animateMoleMiss(Position p)
{
  setPosition(p);
  drawHole(p);
  drawMole(p, 0);
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledDrawString("miss");
  OrbitOledUpdate();
  delay(90);
  drawHole(p);
}

void animateMoleThrow()
{
  static char splat1[240] = {0};
  static char splat2[512] = {0}; 
  if (!(splat1[0]))
  {
    for (int i = 0; i < 240; i++)
    {
      splat1[i] = 255; 
      splat2[i] = 255; 
    }
    for (int i = 240; i < 512; i++)
    {
      splat2[i] = 255;
    }
  }
  OrbitOledSetDrawMode(modOledSet); 
  OrbitOledMoveTo(23, 4);
  OrbitOledPutBmp(splatWidth, splatHeight, splat1);
  OrbitOledUpdate(); 
  delay(300); 
  OrbitOledMoveTo(0,0);
  OrbitOledPutBmp(128, 32, splat2); 
  OrbitOledMoveTo(5, 6);
  OrbitOledDrawString("Shake to clear!");
  OrbitOledUpdate(); 
}

void updateLives()
{
  static int prevLives = 0;
  if (lives != prevLives)
  {
    OrbitOledMoveTo(120, 24);
    char s[1] = {0}; 
    sprintf(s, "%d", lives); 
    OrbitOledDrawString(s); 
    OrbitOledUpdate();
   prevLives = (lives != 0 ? lives : -1); 
  }
}

void updateScore()
{
  static int prevScore = -1; 
  if (score != prevScore)
  {
    OrbitOledMoveTo(0, 0); 
    char s[10] = {0}; 
    sprintf(s, "%d", score);
    OrbitOledDrawString(s); 
    OrbitOledUpdate();
    prevScore = score; 
  }
}


