//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_BALL_H
#define BREAKOUT_BALL_H

#include "../Components/GameObject.h"
#include "../Utility/Circle.h"
#include "../Utility/Vector2.h"
#include <Engine/Sprite.h>

class Ball : public GameObject
{
 public:
  Ball() = default;
  ~Ball() override = default;

  bool setup(ASGE::Renderer* renderer,
             const std::string& file_name,
             float x_start,
             float y_start,
             float start_speed,
             float width,
             float height);

  // Setters
  void reset() override;
  void generateDirection();

 private:
  float start_x = 0;
  float start_y = 0;
};

#endif // BREAKOUT_BALL_H
