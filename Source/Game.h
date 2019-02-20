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
  void setUpBlock(int count, float x, float y);
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

  bool in_menu = true;
  bool gameover = false;
  bool gamewon = false;
  int lives = 3;
};