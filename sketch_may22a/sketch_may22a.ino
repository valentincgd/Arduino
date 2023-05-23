#include <Gamebuino-Meta.h>

const int platformCount = 5;
int platformX[platformCount];
int platformY[platformCount];
int platformWidth = 20;
int platformHeight = 2;
int platformGap = 15;
int platformSpeed = 1;

int positionX = 32;
int positionY = 35;
int speedY = 5;
int ballSize = 2;
bool ballUp = true;
bool onPlatform = false;

int score = 0;

void setup()
{
  gb.begin();
  generatePlatforms();
}

void generatePlatforms()
{
  int x = gb.display.width() / 2 - platformWidth / 2;
  int y = gb.display.height() - platformGap;
  for (int i = 0; i < platformCount; i++)
  {
    platformX[i] = x;
    platformY[i] = y;
    x = random(0, gb.display.width() - platformWidth);
    y -= platformGap;
  }
}

bool checkCollision()
{
  if (ballUp)
  {
    return false;
  }
  for (int i = 0; i < platformCount; i++)
  {
    if (positionX + ballSize >= platformX[i] && positionX <= platformX[i] + platformWidth &&
        positionY + ballSize >= platformY[i] && positionY <= platformY[i] + platformHeight)
    {
      return true;
    }
  }
  return false;
}

void dead()
{
  positionX = 32;
  positionY = 60;
  speedY = 5;
  score = 0;
  generatePlatforms();
  gb.display.clear();
}

void updatePlatforms()
{
  for (int i = 0; i < platformCount; i++)
  {
    if (onPlatform)
    {
      platformY[i] += speedY;
    }

    gb.display.setColor(WHITE);
    gb.display.fillRect(platformX[i], platformY[i], platformWidth, platformHeight);

    // Ajouter de nouvelles plates-formes en haut de l'écran
    if (platformY[i] >= gb.display.height())
    {
      platformY[i] = -platformGap;
      platformX[i] = random(0, gb.display.width() - platformWidth);
    }
  }
}

void loop()
{
  while (!gb.update())
    ;
  gb.display.clear();

  // Mise à jour de la position horizontale
  if (gb.buttons.repeat(BUTTON_RIGHT, 0))
  {
    positionX = positionX + 2;
  }

  if (gb.buttons.repeat(BUTTON_LEFT, 0))
  {
    positionX = positionX - 2;
  }

  // Si la balle atteint le bord gauche
  if (positionX < 0)
  {
    dead();
  }

  // Si la balle atteint le bord droit
  if (positionX > gb.display.width() - ballSize)
  {
    dead();
  }

  // Mise à jour de la position verticale
  positionY = positionY + speedY;

  // Si la balle atteint le bord haut
  if (positionY < ((gb.display.width() / 8) * 3))
  {
    // On part en bas
    ballUp = false;
    speedY = 3;
    onPlatform = false;
  }

  // Si la balle atteint le bord bas
  if (positionY > gb.display.height() - ballSize)
  {
    // On part en haut
    ballUp = true;
    speedY = -5;
    onPlatform = false;
  }

  // Rebond
  if (positionY > ((gb.display.width() / 8) * 3) && positionY < ((gb.display.width() / 8) * 4))
  {
    speedY = ballUp ? -3 : 3;
  }

  if (positionY > ((gb.display.width() / 8) * 4) && positionY < ((gb.display.width() / 8) * 5))
  {
    speedY = ballUp ? -4 : 4;
  }

  if (positionY > ((gb.display.width() / 8) * 5))
  {
    speedY = ballUp ? -5 : 5;
  }

  // Vérifier la collision avec les plates-formes
  onPlatform = checkCollision();

  updatePlatforms();

  if (!onPlatform && positionY > gb.display.height() - ballSize)
  {
    // dead();
    gb.display.print("MORT");
  }

  if (onPlatform)
  {
    score++;
  }

  gb.display.setColor(WHITE);
  // gb.display.print("Score: " + String(score));

  gb.display.setColor(WHITE);
  gb.display.fillRect(positionX, positionY, ballSize, ballSize);
}
