void pageTitle()
{
  static int titleup = 0;
  Serial.println((int)(titleup));
  if (!titleup)
  {
    OrbitOledClear(); 
    OrbitOledMoveTo(16, 0);
    OrbitOledDrawString("Whack-A-Mole"); 
    OrbitOledMoveTo(8, 15);
    OrbitOledDrawString("Press a button");
    OrbitOledMoveTo(28,24);
    OrbitOledDrawString("to start.");
    OrbitOledUpdate();
    
    delay(500);
    titleup = 1; 
  }
  if (analogRead(BTN_TOP) > 3000 || analogRead(BTN_BOTTOM) > 3000)
  {
    currentPage = Page::GAME; 
    titleup = 0; 
  }
   
}

void pageGame()
{
  static int gamesetup = 0; 
  if (!gamesetup)
  {
    OrbitOledClear(); 
    gameSetup();
    gamesetup = 1;  
    return; 
  }
  else if (lives == 0)
  {
    delay(300); 
    OrbitOledClear(); 
    OrbitOledMoveTo(128 / 2 - 40, 12);
    OrbitOledDrawString("Game Over!"); 
    OrbitOledUpdate();
    delay(1000);
    currentPage = Page::RESULT; 
    gamesetup = 0; 
    return; 
  }
  else
  {
    if (!thrown)
    {
      spawnMole();
      checkButtonPress(); 
      despawnMole(); 
    }
    updateLives(); 
    moleThrow(); 
    updateScore();
  }
}

void pageResult()
{
  static int resultsup = 0;
  if (!resultsup)
  {
    char s[10] = {0};
    sprintf(s, "%d", score); 
    OrbitOledClear(); 
    OrbitOledMoveTo(0, 0);
    OrbitOledDrawString("Your score: "); 
    OrbitOledMoveTo(0, 12);
    OrbitOledDrawString(s); 
    OrbitOledUpdate(); 
    resultsup = 1; 
    delay(1000);
  }
  if (analogRead(BTN_TOP) > 3000 || analogRead(BTN_BOTTOM) > 3000)
  {
    currentPage = Page::TITLE; 
    resultsup = 0;
  }
}
