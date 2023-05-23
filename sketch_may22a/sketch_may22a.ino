#include <Gamebuino-Meta.h>

const uint16_t player_image_data[] = {
    // metadata
    3,      // frame width
    4,      // frame height
    4,      // frames
    1,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap

    // frame 1/4
    0xf81f, 0xef7d, 0xf81f,
    0xdefb, 0xeda2, 0xce79,
    0xef7d, 0xbdd7, 0xdefb,
    0xce79, 0xfc02, 0xbdd7,

    // frame 2/4
    0xf81f, 0xef7d, 0xf81f,
    0xdefb, 0xeda2, 0xce79,
    0xef7d, 0xbdd7, 0xdefb,
    0xce79, 0xfac2, 0xbdd7,

    // frame 3/4
    0xf81f, 0xef7d, 0xf81f,
    0xdefb, 0xeda2, 0xce79,
    0xef7d, 0xbdd7, 0xdefb,
    0xce79, 0xf9e2, 0xbdd7,

    // frame 4/4
    0xf81f, 0xef7d, 0xf81f,
    0xdefb, 0xeda2, 0xce79,
    0xef7d, 0xbdd7, 0xdefb,
    0xce79, 0xfb22, 0xbdd7};

Image player(player_image_data);

const int platformCount = 5;
int platformX[platformCount];
int platformY[platformCount];
int platformWidth = 20;
int platformHeight = 2;
int platformGap = 15;
int platformSpeed = 1;

int jump[3] = {2, 3, 4};

int positionX = 32;
int positionY = 35;
int speedY = jump[2];
int ballSize = 2;
bool ballUp = true;
bool onPlatform = false;
bool isDead = false;

int score = 0;
int highScore = 0;

int blink = 4;
int rate = blink * 2;

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
  positionY = 35;
  speedY = -jump[2];
  ballUp = true;
  isDead = true;
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

    // Add new platforms at the top of the screen
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

  if (!isDead)
  {
    // Update horizontal position
    if (gb.buttons.repeat(BUTTON_RIGHT, 0))
    {
      positionX += 2;
    }

    if (gb.buttons.repeat(BUTTON_LEFT, 0))
    {
      positionX -= 2;
    }

    // If the ball reaches the left edge
    if (positionX < 0)
    {
      dead();
    }

    // If the ball reaches the right edge
    if (positionX > gb.display.width() - ballSize)
    {
      dead();
    }

    // Update vertical position
    positionY += speedY;

    // If the ball reaches the top edge
    if (positionY < (gb.display.width() / 8) * 3)
    {
      // Move down
      ballUp = false;
      speedY = jump[0];
      onPlatform = false;
    }

    // If the ball reaches the bottom edge
    if (positionY > gb.display.height() - ballSize && onPlatform)
    {
      // Move up
      ballUp = true;
      speedY = -jump[2];
      onPlatform = false;
    }
    else if (positionY > gb.display.height() - ballSize)
    {
      speedY = 0;
      dead();
    }

    // Bounce
    if (speedY != 0)
    {
      if (positionY > (gb.display.height() / 8) * 3 && positionY < (gb.display.height() / 8) * 4)
      {
        speedY = ballUp ? -jump[0] : jump[0];
      }

      if (positionY > (gb.display.height() / 8) * 4 && positionY < (gb.display.height() / 8) * 5)
      {
        speedY = ballUp ? -jump[1] : jump[1];
      }

      if (positionY > (gb.display.height() / 8) * 5)
      {
        speedY = ballUp ? -jump[2] : jump[2];
      }
    }

    // Check collision with platforms
    onPlatform = checkCollision();

    updatePlatforms();

    if (onPlatform)
    {
      score++;
    }

    gb.display.setColor(RED);
    gb.display.print("Score: " + String(score));
  }
  else
  {

    if (score > highScore || score == highScore)
    {

      gb.display.setColor(RED);
      gb.display.println("Nouveau high score!");
      highScore = score;
    }
    gb.display.setColor(WHITE);

    char textMort[] = "T'es dead man";
    char textScore[] = "Score : ";
    char textHigh[] = "High Score : ";

    char textRejouer[] = "Appuyer sur A";

    gb.display.println(textMort);
    gb.display.println(textScore + String(score));
    gb.display.println(textHigh + String(highScore));

    gb.display.println("");

    if (rate == 0)
    {
      rate = blink * 2;
    }

    if (rate <= blink)
    {
      gb.display.setColor(YELLOW);
    }
    else
    {
      gb.display.setColor(WHITE);
    }
    rate--;
    gb.display.println(textRejouer);

    if (gb.buttons.pressed(BUTTON_A))
    {
      reset();
      isDead = false;
      generatePlatforms();
    }
  }
  gb.display.setColor(WHITE);
  gb.display.drawImage(positionX, positionY, player);
}

void reset()
{
  platformX[platformCount];
  platformY[platformCount];
  platformWidth = 20;
  platformHeight = 2;
  platformGap = 15;
  platformSpeed = 1;

  positionX = 32;
  positionY = 35;
  speedY = jump[2];
  ballSize = 2;
  ballUp = true;
  onPlatform = false;
  isDead = false;

  score = 0;

  blink = 4;
  rate = blink * 2;
}
