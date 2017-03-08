
void spawnMole(Position p); // decides whether a mole will appear
void despawnMole(Position p); // decides whether a mole will disappear on its own
void hitMole(Position p); 
void checkButtonPress();  

double probabilityMoleAppear = 1.0E-3; 
double probabilityMoleThrow = 3.0E-4;

int defaultInterval = 2000;
int moleStatus[4] = {0}; // time in ms when mole came out of its hole
int moleOutTime[4] = {0}; // interval in ms for how long a mole will stay up
int btnPressedStatus[4] = {0};
int score = 0; 
int lives = 3; 
int thrown = 0;

void WireInit();
void ShakeInit();

void spawnMole()
{
  if (rand() < probabilityMoleAppear * RAND_MAX) 
  {
    Position p = (Position)(rand() % 4);
    if (moleStatus[p]) return;  
    moleStatus[p] = millis();
    moleOutTime[p] = defaultInterval - rand() % (score * score + 1);
    if (moleOutTime[p] < 300) 
      moleOutTime[p] = 300;
    animateMolePopUp(p);
    return;
  }
  return;
}

void despawnMole()
{
  for (int i = 0; i < 4; i++)
  {
    if (moleStatus[i])
    {
      if (millis() - moleStatus[i] > moleOutTime[i])
      {
        moleStatus[i] = 0; 
        animateMoleMiss(Position(i));
        lives--;
      }
    }
  }
}

void moleThrow()
{
  static int throwTime; 
  static int throwInterval = 3000;
  static int messageup = 0;
  if (!thrown)
  {
    // there must be at least one mole on screen in order to throw
    if (!moleStatus[0] && !moleStatus[1] && !moleStatus[2] && !moleStatus[3])
      return; 
    if (rand() < probabilityMoleThrow * RAND_MAX)
    {
      throwTime = millis(); 
      thrown = 1; 
      animateMoleThrow(); 
      return;
    }
  }
  if (thrown)
  {
    // immediate game over if player fails to clear the screen on time
    if (millis() - throwTime > throwInterval)
    {
      lives = 0;
      messageup = 0;
      throwInterval = 3000;
      return;
    }

    // put "Hurry!" if more than half the time has passed
    if ((millis() - throwTime) > (throwInterval / 2) && !(messageup))
    {
      Serial.println("works");
      messageup = 1; 
      OrbitOledMoveTo(40, 18); 
      OrbitOledDrawString("Hurry!");
      OrbitOledUpdate();
    }
    
    if (ShakeIsShaking()==true)
    {
      for (int i = 0; i < 4; i++)
      {
        hitMole((Position)i);
      }
      thrown = 0; 
      score++; 
      lives++;
      updateLives();
      lives--;
      OrbitOledClear(); 
      for (int i = 0; i < 4; i++)
      {
        drawHole((Position)i); 
      }
      throwInterval -= score; 
      if (throwInterval < 500)
        throwInterval = 500;
      messageup = 0;
    }
  }
}

void hitMole(Position p)
{
  if (moleStatus[p])
  {
     animateMoleHit(p);
     moleStatus[p] = 0;
     score++; 
  }
}

void checkButtonPress()
{
  if (analogRead(BTN_TOP) > 3000 && !btnPressedStatus[Position::TOP])
  {
    btnPressedStatus[Position::TOP] = 1; 
    hitMole(Position::TOP);
  }
  else if (analogRead(BTN_TOP) < 3000 && btnPressedStatus[Position::TOP])
  {
    btnPressedStatus[Position::TOP] = 0;
  }
  if (analogRead(BTN_BOTTOM) > 3000 && !btnPressedStatus[Position::BOTTOM])
  {
    btnPressedStatus[Position::BOTTOM] = 1; 
    hitMole(Position::BOTTOM);
  }
  else if (analogRead(BTN_BOTTOM) < 3000 && btnPressedStatus[Position::BOTTOM])
  {
    btnPressedStatus[Position::BOTTOM] = 0;
  }
  int swt = digitalRead(SWT_LEFT); 
  if (swt != btnPressedStatus[Position::LEFT])
  {
    btnPressedStatus[Position::LEFT] = swt;
    hitMole(Position::LEFT);
  }
  swt = digitalRead(SWT_RIGHT); 
  if (swt != btnPressedStatus[Position::RIGHT])
  {
    btnPressedStatus[Position::RIGHT] = swt; 
    hitMole(Position::RIGHT);
  }
}

void gameSetup()
{
  for (int i = 0; i < 4; i++)
  {
    drawHole((Position)i);
  }
  score = 0; 
  lives = 3; 
  thrown = 0;
  updateLives(); 
  updateScore();
  for (int i = 0; i < 4; i++)
  {
    moleStatus[i] = 0;
    moleOutTime[i] = 0;
    defaultInterval = 2000;
  } 
}
