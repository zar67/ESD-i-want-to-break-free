//
// Created by Zoe on 08/08/2019.
//

#include "Game.h"
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Platform.h>
#include <Engine/Sprite.h>
#include <iostream>
#include <string>

const int MENU_SCREEN = 0;
const int GAME_SCREEN = 1;
const int GAME_OVER_SCREEN = 2;
const int PAUSE_SCREEN = 3;
const int NEXT_LEVEL_SCREEN = 4;

Game::~Game()
{
  this->inputs->unregisterCallback(key_callback_id);
  this->inputs->unregisterCallback(mouse_callback_id);
}

bool Game::init()
{
  game_width = 655;
  game_height = 920;
  game_name = "Breakout";

  if (!initAPI())
    return false;

  renderer->setClearColour(ASGE::COLOURS::BLACK);
  renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Game::keyHandler, this);
  mouse_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Game::clickHandler, this);

  ui_elements.setupUIObjects(renderer.get(), 200, 30);

  // Setup Level
  std::string filename =
    "../../GameData/Levels/lvl_" + std::to_string(current_level) + ".txt";
  if (!level.load(filename))
  {
    std::cout << "Level NOT loaded" << std::endl;
    return false;
  }

  // Setup GameObjects
  if (!player.setup(
        renderer.get(), "data/Textures/paddle.png", 260, 820, 300, 120, 25))
  {
    std::cout << "Player NOT setup correctly" << std::endl;
    return false;
  }

  if (!ball.setup(
        renderer.get(), "data/Textures/ball.png", 310, 790, 400, 20, 20))
  {
    std::cout << "Ball NOT setup correctly" << std::endl;
    return false;
  }

  for (int i = 0; i < SETTINGS::MAX_BLOCK_NUM; i++)
  {
    int num = rand() % 8 + 1;
    std::string texture =
      "data/Textures/Blocks/tile_" + std::to_string(num) + ".png";

    if (!blocks[i].setupSprite(renderer.get(), texture, 0, -30, 60, 30))
    {
      std::cout << "Block NOT setup correctly" << std::endl;
      return false;
    }
    blocks[i].active(true);
  }

  for (int i = 0; i < SETTINGS::MAX_GEM_NUM; i++)
  {
    int num = rand() % 8 + 1;
    std::string texture =
      "data/Textures/Gems/gem_" + std::to_string(num) + ".png";

    if (!gems[i].setupSprite(renderer.get(), texture, 0, -30, 30, 30))
    {
      std::cout << "Gem NOT setup correctly" << std::endl;
      return false;
    }
    gems[i].setupMovement(150);
    gems[i].setDirection(0, 1);
    gems[i].active(false);
  }

  for (int i = 0; i < SETTINGS::MAX_POWER_UP_NUM; i++)
  {
    if (!power_ups[i].setupSprite(
          renderer.get(), "data/Textures/power_up.png", 0, -30, 30, 30))
    {
      std::cout << "Power Up NOT setup correctly" << std::endl;
      return false;
    }

    power_ups[i].setupMovement(150);
    power_ups[i].setDirection(0, 1);
    power_ups[i].active(false);
  }

  for (int i = 0; i < SETTINGS::MAX_SHOT_NUM; i++)
  {
    if (!shots[i].setupSprite(
          renderer.get(), "data/Textures/shot.png", 0, -20, 10, 40))
    {
      std::cout << "Shot NOT setup correctly" << std::endl;
      return false;
    }

    shots[i].setupMovement(200);
    shots[i].setDirection(0, -1);
    shots[i].active(false);
  }

  return true;
}

void Game::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (screen_open == GAME_SCREEN)
  {
    if (key->key == ASGE::KEYS::KEY_ESCAPE &&
        key->action == ASGE::KEYS::KEY_PRESSED)
    {
      screen_open = PAUSE_SCREEN;
    }
    else if (key->key == ASGE::KEYS::KEY_A)
    {
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        player.setDirection(0, 0);
      }
      else
      {
        player.setDirection(-1, 0);
      }
    }
    else if (key->key == ASGE::KEYS::KEY_D)
    {
      if (key->action == ASGE::KEYS::KEY_RELEASED)
      {
        player.setDirection(0, 0);
      }
      else
      {
        player.setDirection(1, 0);
      }
    }
    else if (key->key == ASGE::KEYS::KEY_SPACE &&
             key->action == ASGE::KEYS::KEY_PRESSED && player.canShoot())
    {
      for (int i = 0; i < SETTINGS::MAX_SHOT_NUM; i++)
      {
        if (!shots[i].active())
        {
          shots[i].active(true);
          shots[i].setPosition(player.position().x + (player.width() / 2),
                               player.position().y - shots[i].height());
          break;
        }
      }
    }
  }
  else if (screen_open != GAME_SCREEN && key->key == ASGE::KEYS::KEY_ESCAPE &&
           key->action == ASGE::KEYS::KEY_PRESSED)
  {
    signalExit();
  }
}

void Game::clickHandler(const ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  if (screen_open == MENU_SCREEN)
  {
    ASGE::Sprite* sprite =
      ui_elements.menu_play_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      resetGame();
      screen_open = GAME_SCREEN;
      return;
    }

    sprite = ui_elements.menu_quit_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      signalExit();
    }
  }

  if (screen_open == PAUSE_SCREEN)
  {
    ASGE::Sprite* sprite =
      ui_elements.pause_resume_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      screen_open = GAME_SCREEN;
      return;
    }

    sprite = ui_elements.pause_menu_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      screen_open = MENU_SCREEN;
      return;
    }

    sprite = ui_elements.pause_quit_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      signalExit();
    }
  }

  if (screen_open == NEXT_LEVEL_SCREEN)
  {
    ASGE::Sprite* sprite =
      ui_elements.next_level_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      loadNextLevel();
      resetLevel();
      screen_open = GAME_SCREEN;
      level_completed = false;
      return;
    }
  }

  if (screen_open == GAME_OVER_SCREEN)
  {
    ASGE::Sprite* sprite =
      ui_elements.game_end_play_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      resetGame();
      screen_open = GAME_SCREEN;
      return;
    }

    sprite = ui_elements.game_end_menu_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      screen_open = MENU_SCREEN;
      return;
    }

    sprite = ui_elements.game_end_quit_button.spriteComponent()->getSprite();
    if (collision.isInside(sprite, click->xpos, click->ypos) &&
        ui_elements.updateButton(click->action, sprite))
    {
      signalExit();
    }
  }
}

void Game::loadNextLevel()
{
  current_level++;
  std::string level_name =
    "../../GameData/Levels/lvl_" + std::to_string(current_level) + ".txt";

  if (!level.load(level_name))
  {
    std::cout << "Next Level NOT loaded correctly" << std::endl;
  }
}

void Game::update(const ASGE::GameTime& game)
{
  if (screen_open == GAME_SCREEN && !game_over && !game_won && !level_completed)
  {
    if (player.playerLives() <= 0)
    {
      game_over = true;
      screen_open = GAME_OVER_SCREEN;
    }

    level_completed = true;
    for (int i = 0; i < level.block_num; i++)
    {
      if (blocks[i].active())
      {
        level_completed = false;
        break;
      }
    }

    if (level_completed)
    {
      if (current_level == SETTINGS::NUM_OF_LEVELS)
      {
        game_won = true;
        screen_open = GAME_OVER_SCREEN;
      }
      else
      {
        screen_open = NEXT_LEVEL_SCREEN;
      }
    }

    // Shot timer
    if (static_cast<float>(game.game_time.count()) >= player.shootTimer())
    {
      player.canShoot(false);
    }

    // Update GameObjects
    player.update(game.delta_time.count() / 1000.f);
    collision.playerBoundaryCollision(&player, game_width);

    ball.update(game.delta_time.count() / 1000.f);
    collision.ballCollisionHandler(&ball, &player, game_width);

    // Gone below paddle
    if (ball.position().y >= static_cast<float>(game_height) &&
        ball.direction().y > 0)
    {
      player.decreaseLives();
      ball.reset();
    }

    for (int i = 0; i < level.block_num; i++)
    {
      if (blocks[i].active())
      {
        if (collision.ballBlockCollision(&ball, &blocks[i]))
        {
          blocks[i].active(false);
          player.increaseScore(1);

          for (int j = 0; j < level.power_up_num; j++)
          {
            if (level.power_up_blocks[j] == i)
            {
              power_ups[j].position();
              power_ups[j].active(true);
            }
          }
        }

        for (int j = 0; j < SETTINGS::MAX_SHOT_NUM; j++)
        {
          if (shots[j].active() &&
              collision.AABBAABBCollision(blocks[i].getRectangle(),
                                          shots[j].getRectangle()))
          {
            blocks[i].active(false);
            shots[j].active(false);
            player.increaseScore(5);

            for (int k = 0; k < level.power_up_num; k++)
            {
              if (level.power_up_blocks[k] == i)
              {
                power_ups[k].position();
                power_ups[k].active(true);
              }
            }
          }
        }
      }
    }

    for (int i = 0; i < SETTINGS::MAX_GEM_NUM; i++)
    {
      if (gems[i].active())
      {
        gems[i].update(game.delta_time.count() / 1000);

        if (collision.AABBAABBCollision(gems[i].getRectangle(),
                                        player.getRectangle()))
        {
          gems[i].reset();
          player.increaseScore(20);
        }

        if (gems[i].position().y > static_cast<float>(game_height))
        {
          gems[i].reset();
        }
      }
      else
      {
        if (rand() % 200000 + 1 < 2)
        {
          float random_x =
            static_cast<float>(std::rand() % (game_width - 120) + 60);
          gems[i].setPosition(random_x, gems[i].position().y);
          gems[i].movementComponent()->setSpeed(
            static_cast<float>(std::rand() % 100 + 100));
          gems[i].active(true);
        }
      }
    }

    for (int i = 0; i < level.power_up_num; i++)
    {
      if (power_ups[i].active())
      {
        power_ups[i].update(game.delta_time.count() / 1000);

        if (collision.AABBAABBCollision(power_ups[i].getRectangle(),
                                        player.getRectangle()))
        {
          power_ups[i].reset();
          player.canShoot(true);
          player.shootTimer(static_cast<float>(game.game_time.count()) + 4000);
        }

        if (power_ups[i].position().y > static_cast<float>(game_height))
        {
          power_ups[i].reset();
        }
      }
    }

    for (int i = 0; i < SETTINGS::MAX_SHOT_NUM; i++)
    {
      if (shots[i].active())
      {
        shots[i].update(game.delta_time.count() / 1000);
      }
    }
  }
}

void Game::renderMainMenuScreen()
{
  renderer->renderText("BLOCK BREAKER", 185, 250, 2, ASGE::COLOURS::WHITE);
  renderer->renderSprite(
    *ui_elements.menu_play_button.spriteComponent()->getSprite());
  renderer->renderText("Play", 300, 368, 1, ASGE::COLOURS::BLACK);
  renderer->renderSprite(
    *ui_elements.menu_quit_button.spriteComponent()->getSprite());
  renderer->renderText("Quit", 300, 418, 1, ASGE::COLOURS::BLACK);
}

void Game::renderGameScreen()
{
  renderer->renderSprite(*player.spriteComponent()->getSprite());
  renderer->renderSprite(*ball.spriteComponent()->getSprite());

  std::string score = "Score: ";
  score += std::to_string(player.playerScore());
  renderer->renderText(score, 25, 890, 1.5, ASGE::COLOURS::WHITE);

  std::string lives = "Lives: ";
  lives += std::to_string(player.playerLives());
  renderer->renderText(lives, 480, 890, 1.5, ASGE::COLOURS::WHITE);

  for (int i = 0; i < level.block_num; i++)
  {
    if (blocks[i].active())
    {
      renderer->renderSprite(*blocks[i].spriteComponent()->getSprite());
    }
  }

  for (int i = 0; i < SETTINGS::MAX_SHOT_NUM; i++)
  {
    if (shots[i].active())
    {
      renderer->renderSprite(*shots[i].spriteComponent()->getSprite());
    }
  }

  for (int i = 0; i < level.power_up_num; i++)
  {
    if (power_ups[i].active())
    {
      renderer->renderSprite(*power_ups[i].spriteComponent()->getSprite());
    }
  }

  for (int i = 0; i < SETTINGS::MAX_GEM_NUM; i++)
  {
    if (gems[i].active())
    {
      renderer->renderSprite(*gems[i].spriteComponent()->getSprite());
    }
  }
}

void Game::renderNextLevelScreen()
{
  renderGameScreen();

  renderer->renderText("YOU WIN!", 220, 380, 2, ASGE::COLOURS::WHITE);
  renderer->renderSprite(
    *ui_elements.next_level_button.spriteComponent()->getSprite());
  renderer->renderText("Next Level", 265, 444, 1, ASGE::COLOURS::BLACK);
}

void Game::renderGameOverScreen()
{
  renderGameScreen();

  if (game_won)
  {
    renderer->renderText("YOU WIN!", 230, 380, 2, ASGE::COLOURS::WHITE);
  }
  else if (game_over)
  {
    renderer->renderText("YOU LOSE", 230, 380, 2, ASGE::COLOURS::WHITE);
  }

  renderer->renderSprite(
    *ui_elements.game_end_play_button.spriteComponent()->getSprite());
  renderer->renderText("Play Again", 265, 425, 1, ASGE::COLOURS::BLACK);
  renderer->renderSprite(
    *ui_elements.game_end_menu_button.spriteComponent()->getSprite());
  renderer->renderText("Menu", 295, 475, 1, ASGE::COLOURS::BLACK);
  renderer->renderSprite(
    *ui_elements.game_end_quit_button.spriteComponent()->getSprite());
  renderer->renderText("Quit", 295, 525, 1, ASGE::COLOURS::BLACK);
}

void Game::renderPauseScreen()
{
  renderGameScreen();

  renderer->renderText("PAUSED", 250, 200, 2, ASGE::COLOURS::WHITE);

  renderer->renderSprite(
    *ui_elements.pause_resume_button.spriteComponent()->getSprite());
  renderer->renderText("Resume", 291, 325, 1, ASGE::COLOURS::BLACK);
  renderer->renderSprite(
    *ui_elements.pause_menu_button.spriteComponent()->getSprite());
  renderer->renderText("Menu", 295, 375, 1, ASGE::COLOURS::BLACK);
  renderer->renderSprite(
    *ui_elements.pause_quit_button.spriteComponent()->getSprite());
  renderer->renderText("Quit", 295, 425, 1, ASGE::COLOURS::BLACK);
}

void Game::render(const ASGE::GameTime& game)
{
  if (screen_open == MENU_SCREEN)
  {
    renderMainMenuScreen();
  }
  else if (screen_open == GAME_SCREEN)
  {
    renderGameScreen();
  }
  else if (screen_open == NEXT_LEVEL_SCREEN)
  {
    renderNextLevelScreen();
  }
  else if (screen_open == GAME_OVER_SCREEN)
  {
    renderGameOverScreen();
  }
  else if (screen_open == PAUSE_SCREEN)
  {
    renderPauseScreen();
  }
}

void Game::resetGame()
{
  game_over = false;
  game_won = false;

  current_level = 0;
  loadNextLevel();

  ball.reset();
  player.reset();

  resetGameObjects();
}

void Game::resetLevel()
{
  game_over = false;
  game_won = false;

  ball.reset();
  player.resetPosition();

  resetGameObjects();
}

void Game::resetGameObjects()
{
  for (int i = 0; i < level.block_num; i++)
  {
    blocks[i].active(true);
    blocks[i].setPosition(level.block_positions[i][0],
                          level.block_positions[i][1]);
  }
  for (int i = 0; i < SETTINGS::MAX_GEM_NUM; i++)
  {
    gems[i].reset();
  }
  for (int i = 0; i < level.power_up_num; i++)
  {
    power_ups[i].reset();
    power_ups[i].setPosition(
      blocks[level.power_up_blocks[i]].position().x + (blocks[i].width() / 2) -
        (power_ups[i].width() / 2),
      blocks[level.power_up_blocks[i]].position().y + (blocks[i].height() / 2) -
        (power_ups[i].height() / 2));
  }
  for (int i = 0; i < SETTINGS::MAX_SHOT_NUM; i++)
  {
    shots[i].reset();
  }
}