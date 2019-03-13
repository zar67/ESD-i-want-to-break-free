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

/* MARKING FEEDBACK
 * - Good layout and readability
 * - Casts should use "static" casts instead of C version casts due to exception
 *   handlers etc
 * - Breakup into more, smaller functions, maybe have a collision or update
 *   gameobject class to break up the update function
 * - Collision detection should detect collisions, not resolve them as-well
 */

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

void BreakoutGame::setUpBlock(int count,
                              float x,
                              float y,
                              const std::string& sprite)
{
  if (blocks[count].addSpriteComponent(renderer.get(), sprite))
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
  std::srand(static_cast<unsigned int>(time(nullptr)));
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

    if (i % 8 == 0)
    {
      setUpBlock(
        i, x, y, "Textures/puzzlepack/png/element_purple_rectangle.png");
    }
    else
    {
      setUpBlock(
        i, x, y, "Textures/puzzlepack/png/element_yellow_rectangle.png");
    }

    row++;
    int extra = row % 6;
    if (extra == 0)
    {
      row = 0;
      column++;
    }
  }

  // Setup gems
  for (int i = 0; i < GEM_NUMBER; i++)
  {
    if (gems[i].addSpriteComponent(renderer.get(),
                                   "Textures/puzzlepack/png/"
                                   "element_red_polygon.png"))
    {
      std::cout << "Gem Sprite set" << std::endl;
      gems[i].spriteComponent()->getSprite()->xPos(0);
      gems[i].spriteComponent()->getSprite()->yPos(-50);

      gems[i].speed(150);
      vector2 dir = vector2(0, -1);
      dir.normalise();
      gems[i].direction(dir.x, dir.y);
      gems[i].visibility(false);
    }
    else
    {
      std::cout << "Gem Sprite NOT set" << std::endl;
    }
  }

  // Setup Power-ups
  for (int i = 0; i < POWER_UP_NUMBER; i++)
  {
    if (power_ups[i].addSpriteComponent(renderer.get(),
                                        "Textures/puzzlepack/png/"
                                        "element_green_square.png"))
    {
      std::cout << "Power Up Sprite set" << std::endl;
      power_ups[i].spriteComponent()->getSprite()->xPos(0);
      power_ups[i].spriteComponent()->getSprite()->yPos(0);
      power_ups[i].speed(150);
      vector2 dir = vector2(0, -1);
      dir.normalise();
      power_ups[i].direction(dir.x, dir.y);
      power_ups[i].visibility(false);
    }
    else
    {
      std::cout << "Power Up Sprite NOT set" << std::endl;
    }
  }

  // Shots setup
  for (int i = 0; i < SHOT_NUMBER; i++)
  {
    if (shots[i].addSpriteComponent(renderer.get(),
                                    "Textures/puzzlepack/png/particleStar.png"))
    {
      std::cout << "Shot Sprite set" << std::endl;
      shots[i].spriteComponent()->getSprite()->xPos(0);
      shots[i].spriteComponent()->getSprite()->yPos(0);
      shots[i].speed(200);
      vector2 dir = vector2(0, 1);
      dir.normalise();
      shots[i].direction(dir.x, dir.y);
      shots[i].visibility(false);
    }
    else
    {
      std::cout << "Shot Sprite NOT set" << std::endl;
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
  // here are some arbitrary values for you to adjust as you see fit
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

  else if (in_menu && key->key == ASGE::KEYS::KEY_ENTER)
  {
    in_menu = false;
  }

  else if (!in_menu && key->key == ASGE::KEYS::KEY_ENTER)
  {
    restartGame();
  }

  else if (!in_menu && key->key == ASGE::KEYS::KEY_A)
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

  else if (key->key == ASGE::KEYS::KEY_SPACE)
  {
    if (key->action == ASGE::KEYS::KEY_PRESSED && player.canShoot())
    {
      for (int i = 0; i < SHOT_NUMBER; i++)
      {
        if (!shots[i].visibility())
        {
          shots[i].visibility(true);
          shots[i].spriteComponent()->getSprite()->xPos(
            player.spriteComponent()->getSprite()->xPos() +
            (player.spriteComponent()->getSprite()->width() / 2));
          shots[i].spriteComponent()->getSprite()->yPos(
            player.spriteComponent()->getSprite()->yPos() - 10);
          break;
        }
      }
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

  // Sanity Checking
  if (new_dir.y > 0)
  {
    new_dir.y *= -1;
  }
  if ((ball.direction().x > 0 && new_dir.x > 0) ||
      (ball.direction().x < 0 && new_dir.x < 0))
  {
    new_dir.x *= -1;
  }
  if ((new_dir.x < 0 && player.direction().x > 0) ||
      (new_dir.x > 0 && player.direction().x < 0))
  {
    new_dir.x *= -1;
  }
  ball.direction(new_dir.x, new_dir.y);
}

bool BreakoutGame::collisionDetection(float x, float y, float size)
{
  // Wall Collision
  if (x < 0 && ball.direction().x < 0)
  {
    x = 0;
    ball.direction(-ball.direction().x, ball.direction().y);
  }
  else if (x > float(game_width) - size && ball.direction().x > 0)
  {
    x = float(game_width) - size;
    ball.direction(-ball.direction().x, ball.direction().y);
  }
  if (y < 0 && ball.direction().y < 0)
  {
    y = 0;
    ball.direction(ball.direction().x, -ball.direction().y);
  }
  else if (y > float(game_height) - size && ball.direction().y > 0)
  {
    lives--;
    return false;
  }

  ball.spriteComponent()->getSprite()->xPos(x);
  ball.spriteComponent()->getSprite()->yPos(y);

  // Paddle Collision
  if (player.spriteComponent()->getBoundingBox().isInside(x, y + size))
  {
    calculateNewDirection(x, size);
  }

  // Block Collision
  for (int i = 0; i < BLOCK_NUMBER; i++)
  {
    if (blocks[i].visibility() &&
        blocks[i].spriteComponent()->getBoundingBox().isInside(
          ball.spriteComponent()->getBoundingBox()))
    {
      blocks[i].visibility(false);
      if (i % 8 == 0)
      {
        // Hit power-up block
        int power_up_number = i / 10;
        power_ups[power_up_number].spriteComponent()->getSprite()->xPos(
          blocks[i].spriteComponent()->getSprite()->xPos());
        power_ups[power_up_number].spriteComponent()->getSprite()->yPos(
          blocks[i].spriteComponent()->getSprite()->yPos());
        power_ups[power_up_number].visibility(true);
      }
      else
      {
        score += 1;
      }

      // Get new ball direction
      if (x + size <= blocks[i].spriteComponent()->getSprite()->xPos() + 2 &&
          ball.direction().x > 0)
      {
        ball.direction(-ball.direction().x, ball.direction().y);
      }
      else if (x >= blocks[i].spriteComponent()->getSprite()->xPos() +
                      blocks[i].spriteComponent()->getSprite()->width() - 2 &&
               ball.direction().x < 0)
      {
        ball.direction(-ball.direction().x, ball.direction().y);
      }
      if (y + size <= blocks[i].spriteComponent()->getSprite()->yPos() + 2 &&
          ball.direction().y > 0)
      {
        ball.direction(ball.direction().x, -ball.direction().y);
      }
      else if (y >= blocks[i].spriteComponent()->getSprite()->yPos() +
                      blocks[i].spriteComponent()->getSprite()->height() - 2 &&
               ball.direction().y < 0)
      {
        ball.direction(ball.direction().x, -ball.direction().y);
      }
    }
  }
  return true;
}

void BreakoutGame::restartGame()
{
  gameover = false;
  gamewon = false;
  score = 0;
  lives = 3;

  // Reset Player
  player.spriteComponent()->getSprite()->xPos(280);
  player.spriteComponent()->getSprite()->yPos(850);

  // Reset Ball
  ball.spriteComponent()->getSprite()->xPos(320);
  ball.spriteComponent()->getSprite()->yPos(800);
  vector2 dir = vector2(-1, -1);
  dir.normalise();
  ball.direction(dir.x, dir.y);

  // Reset blocks
  for (int i = 0; i < BLOCK_NUMBER; i++)
  {
    blocks[i].visibility(true);
  }

  // Reset gems
  for (int i = 0; i < GEM_NUMBER; i++)
  {
    gems[i].spriteComponent()->getSprite()->xPos(0);
    gems[i].spriteComponent()->getSprite()->yPos(-50);
    gems[i].visibility(false);
  }

  // Reset power ups
  for (int i = 0; i < POWER_UP_NUMBER; i++)
  {
    power_ups[i].spriteComponent()->getSprite()->xPos(0);
    power_ups[i].spriteComponent()->getSprite()->yPos(0);
    power_ups[i].visibility(false);
  }

  // Reset shots
  for (int i = 0; i < SHOT_NUMBER; i++)
  {
    shots[i].spriteComponent()->getSprite()->xPos(0);
    shots[i].spriteComponent()->getSprite()->yPos(0);
    shots[i].visibility(false);
  }
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

    // Shot Timer
    if (double(game_time.game_time.count()) >= player.shootTimer())
    {
      player.canShoot(false);
      player.shootTimer(0);
    }

    // Release gems
    for (int i = 0; i < GEM_NUMBER; i++)
    {
      if (std::rand() % 5000000 + 1 < 2)
      {
        int random_x = std::rand() % (game_width - 120) + 60;
        std::cout << random_x << std::endl;
        gems[i].spriteComponent()->getSprite()->xPos(float(random_x));
        gems[i].speed(float(std::rand() % 100 + 100));
        gems[i].visibility(true);
      }
    }

    // Gem Collision Detection
    for (int i = 0; i < GEM_NUMBER; i++)
    {
      if (gems[i].spriteComponent()->getBoundingBox().isInside(
            player.spriteComponent()->getBoundingBox()) &&
          gems[i].visibility())
      {
        gems[i].visibility(false);
        gems[i].spriteComponent()->getSprite()->yPos(-50);
        score += 10;
      }
      if (gems[i].spriteComponent()->getSprite()->yPos() > float(game_height))
      {
        gems[i].visibility(false);
        gems[i].spriteComponent()->getSprite()->yPos(-50);
      }
    }

    // Power Up Collision Detection
    for (int i = 0; i < POWER_UP_NUMBER; i++)
    {
      if (power_ups[i].spriteComponent()->getBoundingBox().isInside(
            player.spriteComponent()->getBoundingBox()) &&
          power_ups[i].visibility())
      {
        power_ups[i].visibility(false);
        power_ups[i].spriteComponent()->getSprite()->yPos(-50);
        score += 10;
        player.canShoot(true);
        player.shootTimer(float(game_time.game_time.count()) + 4000);
      }
      if (power_ups[i].spriteComponent()->getSprite()->yPos() >
          float(game_height))
      {
        power_ups[i].visibility(false);
        power_ups[i].spriteComponent()->getSprite()->yPos(-50);
      }
    }

    // Shot Collision Detection
    for (int i = 0; i < SHOT_NUMBER; i++)
    {
      for (int j = 0; j < BLOCK_NUMBER; j++)
      {
        if (shots[i].spriteComponent()->getBoundingBox().isInside(
              blocks[j].spriteComponent()->getBoundingBox()) &&
            shots[i].visibility() && blocks[j].visibility())
        {
          blocks[j].visibility(false);
          shots[i].visibility(false);
          score += 5;
        }
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

    if (!collisionDetection(
          ball_x, ball_y, ball.spriteComponent()->getSprite()->width()))
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

    // Update Gems
    for (int i = 0; i < GEM_NUMBER; i++)
    {
      if (gems[i].visibility())
      {
        float current_y = gems[i].spriteComponent()->getSprite()->yPos();
        current_y -= float(gems[i].direction().y * gems[i].speed() *
                           (game_time.delta_time.count() / 1000.f));
        gems[i].spriteComponent()->getSprite()->yPos(current_y);
      }
    }

    // Update Power Ups
    for (int i = 0; i < POWER_UP_NUMBER; i++)
    {
      if (power_ups[i].visibility())
      {
        float current_y = power_ups[i].spriteComponent()->getSprite()->yPos();
        current_y -= float(power_ups[i].direction().y * power_ups[i].speed() *
                           (game_time.delta_time.count() / 1000.f));
        power_ups[i].spriteComponent()->getSprite()->yPos(current_y);
      }
    }

    // Update Shots
    for (int i = 0; i < SHOT_NUMBER; i++)
    {
      if (shots[i].visibility())
      {
        float current_y = shots[i].spriteComponent()->getSprite()->yPos();
        current_y -= float(shots[i].direction().y * shots[i].speed() *
                           (game_time.delta_time.count() / 1000.f));
        shots[i].spriteComponent()->getSprite()->yPos(current_y);
      }
    }
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
    renderer->renderText("Press ENTER when in the game to restart", 130, 520);
  }
  else
  {
    for (int i = 0; i < BLOCK_NUMBER; i++)
    {
      if (blocks[i].visibility())
      {
        renderer->renderSprite(*blocks[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < GEM_NUMBER; i++)
    {
      if (gems[i].visibility())
      {
        renderer->renderSprite(*gems[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < POWER_UP_NUMBER; i++)
    {
      if (power_ups[i].visibility())
      {
        renderer->renderSprite(*power_ups[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < SHOT_NUMBER; i++)
    {
      if (shots[i].visibility())
      {
        renderer->renderSprite(*shots[i].spriteComponent()->getSprite());
      }
    }

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

    std::string score_txt = "Score: ";
    score_txt += std::to_string(score);
    renderer->renderText(score_txt, 525, 50, 1, ASGE::COLOURS::WHITE);
  }
}
