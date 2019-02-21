#include <iostream>
#include <math.h>
#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "Game.h"
#include "Utility/Rect.h"

const int BLOCK_NUMBER = 30;

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
BreakoutGame::BreakoutGame() {}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
BreakoutGame::~BreakoutGame()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
}

void BreakoutGame::setUpBlock(int count, float x, float y)
{
  if (blocks[count].addSpriteComponent(renderer.get(),
                                       "Textures/puzzlepack/png/"
                                       "element_yellow_rectangle.png"))
  {
    blocks[count].spriteComponent()->getSprite()->xPos(x);
    blocks[count].spriteComponent()->getSprite()->yPos(y);
    std::cout << "Block " << count << " Sprite Set" << std::endl;
  }
  else
  {
    std::cout << "Block " << count << " Sprite NOT Set" << std::endl;
  }
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool BreakoutGame::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  toggleFPS();
  renderer->setWindowTitle("Breakout!");

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &BreakoutGame::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &BreakoutGame::clickHandler, this);

  // Setup player
  if (player.addSpriteComponent(renderer.get(),
                                "Textures/puzzlepack/png/paddleBlue.png"))
  {
    std::cout << "Player Sprite Set" << std::endl;
    player.spriteComponent()->getSprite()->xPos(280);
    player.spriteComponent()->getSprite()->yPos(850);
    player.speed(300.0f);
  }
  else
  {
    std::cout << "Player Sprite NOT Set" << std::endl;
  }

  // Setup ball
  if (ball.addSpriteComponent(renderer.get(),
                              "Textures/puzzlepack/png/ballBlue.png"))
  {
    std::cout << "Ball Sprite Set" << std::endl;
    ball.spriteComponent()->getSprite()->xPos(320);
    ball.spriteComponent()->getSprite()->yPos(800);
    ball.speed(450.0f);
    vector2 dir = vector2(-1, -1);
    dir.normalise();
    ball.direction(dir.x, dir.y);
  }
  else
  {
    std::cout << "Ball Sprite NOT Set" << std::endl;
  }

  // Setup blocks
  int row = 0;
  int column = 0;
  for (int i = 0; i < BLOCK_NUMBER; i++)
  {
    float x = float(row) * 100 + 35;
    float y = float(column) * 50 + 30;
    setUpBlock(i, x, y);

    row++;
    int extra = row % 6;
    if (extra == 0)
    {
      row = 0;
      column++;
    }
  }

  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void BreakoutGame::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some abritrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
  game_width = 640;
  game_height = 920;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void BreakoutGame::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (key->key == ASGE::KEYS::KEY_ENTER)
  {
    in_menu = false;
  }

  if (!in_menu && key->key == ASGE::KEYS::KEY_A)
  {
    if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player.direction(0, 0);
    }
    else
    {
      player.direction(-1, 0);
    }
  }

  else if (key->key == ASGE::KEYS::KEY_D)
  {
    if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player.direction(0, 0);
    }
    else
    {
      player.direction(1, 0);
    }
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void BreakoutGame::clickHandler(const ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}

void BreakoutGame::calculateNewDirection(float x, float size)
{
  float paddle_middle = player.spriteComponent()->getSprite()->xPos() +
                        player.spriteComponent()->getSprite()->width() / 2;
  float ball_intersect = paddle_middle - (x + (size / 2));
  ball_intersect =
    (ball_intersect / (player.spriteComponent()->getSprite()->width() / 2));
  double bounce_angle = ball_intersect * 1.0472;
  vector2 new_dir = vector2(float(ball.speed() * sin(bounce_angle)),
                            float(ball.speed() * cos(bounce_angle)));
  new_dir.normalise();
  ball.direction(new_dir.x, -new_dir.y);
}

bool BreakoutGame::collisionDetection(float x, float y, float size)
{
  // Wall Collision
  if (x < 0)
  {
    x = 0;
    ball.direction(-ball.direction().x, ball.direction().y);
  }
  else if (x > float(game_width) - size)
  {
    x = float(game_width) - size;
    ball.direction(-ball.direction().x, ball.direction().y);
  }
  if (y < 0)
  {
    y = 0;
    ball.direction(ball.direction().x, -ball.direction().y);
  }
  else if (y > float(game_height) - size)
  {
    lives--;
    return false;
  }

  // Paddle Collision
  if (player.spriteComponent()->getBoundingBox().isInside(x, y + size))
  {
    calculateNewDirection(x, size);
  }

  // Block Collision
  for (int i = 0; i < BLOCK_NUMBER; i++)
  {
    if (blocks[i].visibility() &&
        blocks[i].spriteComponent()->getBoundingBox().isInside(x, y))
    {
      blocks[i].visibility(false);

      if (y >= blocks[i].spriteComponent()->getSprite()->yPos() +
                 blocks[i].spriteComponent()->getSprite()->height())
      {
        ball.direction(ball.direction().x, -ball.direction().y);
      }
      else
      {
        ball.direction(-ball.direction().x, ball.direction().y);
      }
    }
    else if (blocks[i].visibility() &&
             blocks[i].spriteComponent()->getBoundingBox().isInside(x + size,
                                                                    y + size))
    {
      blocks[i].visibility(false);

      if (y <= blocks[i].spriteComponent()->getSprite()->yPos())
      {
        ball.direction(ball.direction().x, -ball.direction().y);
      }
      else
      {
        ball.direction(-ball.direction().x, ball.direction().y);
      }
    }
  }
  return true;
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void BreakoutGame::update(const ASGE::GameTime& game_time)
{
  if (!in_menu && !gameover && !gamewon)
  {
    if (lives <= 0)
    {
      gameover = true;
    }

    gamewon = true;
    for (int i = 0; i < BLOCK_NUMBER; i++)
    {
      if (blocks[i].visibility())
      {
        gamewon = false;
        break;
      }
    }

    // Move Player
    float new_x = player.spriteComponent()->getSprite()->xPos();
    if (player.direction().x == -1 &&
        player.spriteComponent()->getSprite()->xPos() > 0)
    {
      new_x =
        new_x - float(player.speed() * (game_time.delta_time.count() / 1000.f));
    }
    else if (player.direction().x == 1 &&
             player.spriteComponent()->getSprite()->xPos() <
               float(game_width - 100))
    {
      new_x =
        new_x + float(player.speed() * (game_time.delta_time.count() / 1000.f));
    }

    player.spriteComponent()->getSprite()->xPos(new_x);

    // Move Ball
    float ball_x = ball.spriteComponent()->getSprite()->xPos();
    float ball_y = ball.spriteComponent()->getSprite()->yPos();

    ball_x += float(ball.direction().x * ball.speed() *
                    (game_time.delta_time.count() / 1000.f));
    ball_y += float(ball.direction().y * ball.speed() *
                    (game_time.delta_time.count() / 1000.f));

    if(!collisionDetection(ball_x, ball_y,
         ball.spriteComponent()->getSprite()->width()))
    {
      ball_x = 320;
      ball_y = 800;

      vector2 dir = vector2(-1, -1);
      dir.normalise();
      ball.direction(dir.x, dir.y);
    }

    // Set new ball position
    ball.spriteComponent()->getSprite()->xPos(ball_x);
    ball.spriteComponent()->getSprite()->yPos(ball_y);
  }

  // auto dt_sec = game_time.delta_time.count() / 1000.0;
  // make sure you use delta time in any movement calculations!
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void BreakoutGame::render(const ASGE::GameTime&)
{
  renderer->setFont(0);

  if (in_menu)
  {
    renderer->renderText("Press ENTER to start the game", 180, 460);
  }
  else
  {
    if (gamewon)
    {
       renderer->renderText("Congratulations! You've won!", 170, 460);
    }
    else if (gameover)
    {
       renderer->renderText("You Lose", 300, 460);
    }
    renderer->renderSprite(*player.spriteComponent()->getSprite());
    renderer->renderSprite(*ball.spriteComponent()->getSprite());

    std::string lives_txt = "Lives: ";
    lives_txt += std::to_string(lives);
    renderer->renderText(lives_txt, 550, 25, 1, ASGE::COLOURS::WHITE);

    for (int i = 0; i < 30; i++)
    {
      if (blocks[i].visibility())
      {
        renderer->renderSprite(*blocks[i].spriteComponent()->getSprite());
      }
    }
  }
}
