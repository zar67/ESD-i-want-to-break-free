#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Components/GameObject.h"
#include "Utility/Rect.h"

const int BLOCK_NUMBER = 30;
const int GEM_NUMBER = 3;
const int POWER_UP_NUMBER = 3;
const int SHOT_NUMBER = 10;

/**
 *  An OpenGL Game based on ASGE.
 */
class BreakoutGame : public ASGE::OGLGame
{
 public:
  BreakoutGame();
  ~BreakoutGame();
  virtual bool init() override;

 private:
  void keyHandler(const ASGE::SharedEventData data);
  void clickHandler(const ASGE::SharedEventData data);
  void setupResolution();
  void setUpBlock(int count, float x, float y, const std::string& sprite);
  bool collisionDetection(float x, float y, float size);
  void calculateNewDirection(float x, float size);
  void restartGame();

  virtual void update(const ASGE::GameTime&) override;
  virtual void render(const ASGE::GameTime&) override;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  // Add your GameObjects
  GameObject player;
  GameObject ball;
  GameObject blocks[BLOCK_NUMBER];
  GameObject gems[GEM_NUMBER];
  GameObject power_ups[POWER_UP_NUMBER];
  GameObject shots[SHOT_NUMBER];

  bool in_menu = true;
  bool gameover = false;
  bool gamewon = false;
  bool respawn_ball = false;
  int lives = 3;
  int score = 0;
};