//
// Created by Zoe on 08/08/2019.
//

#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include "GameObjects/Ball.h"
#include "GameObjects/Player.h"
#include "Handlers/CollisionHandler.h"
#include "Handlers/UIHandler.h"
#include "LevelData/GameConstants.h"
#include "LevelData/Level.h"
#include <Engine/OGLGame.h>

class Game : public ASGE::OGLGame
{
 public:
  virtual ~Game();
  bool init() override;
  void update(const ASGE::GameTime& game) override;
  void render(const ASGE::GameTime& game) override;

 private:
  void keyHandler(const ASGE::SharedEventData data);
  void clickHandler(const ASGE::SharedEventData data);

  void loadNextLevel();

  void resetGame();
  void resetLevel();
  void resetGameObjects();
  void renderMainMenuScreen();
  void renderGameScreen();
  void renderNextLevelScreen();
  void renderGameOverScreen();
  void renderPauseScreen();

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  Level level;
  int current_level = 1;

  int screen_open = 0;

  CollisionHandler collision;
  UIHandler ui_elements;

  Ball ball;
  Player player;

  GameObject blocks[SETTINGS::MAX_BLOCK_NUM];
  GameObject gems[SETTINGS::MAX_GEM_NUM];
  GameObject power_ups[SETTINGS::MAX_POWER_UP_NUM];
  GameObject shots[SETTINGS::MAX_SHOT_NUM];

  bool game_over = false;
  bool game_won = false;
  bool level_completed = false;
};

#endif // HELLOASGE_GAME_H
