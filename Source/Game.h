#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Components/GameObject.h"
#include "Utility/Rect.h"

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
  void setupGem(int count, std::string sprite, float x, float y, float speed);
  void
  setupPowerUps(int count, std::string sprite, float x, float y, float speed);
  bool collisionDetection(float x, float y, float size);
  void calculateNewDirection(float x, float size);

  virtual void update(const ASGE::GameTime&) override;
  virtual void render(const ASGE::GameTime&) override;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  // Add your GameObjects
  GameObject player;
  GameObject ball;
  GameObject blocks[30];
  GameObject gems[3];
  GameObject power_ups[3];
  GameObject shots[5];

  bool in_menu = true;
  bool gameover = false;
  bool gamewon = false;
  bool respawn_ball = false;
  int lives = 3;
  int score = 0;
};