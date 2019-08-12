//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_MOVEMENTCOMPONENT_H
#define BREAKOUT_MOVEMENTCOMPONENT_H

#include "../Utility/Vector2.h"

class MovementComponent
{
 public:
  MovementComponent(float initial_speed);
  ~MovementComponent() = default;

  // Position setters
  Vector2 moveGameObject(double delta, float x, float y);

  // Setters
  void reset();
  void increaseSpeed(float a);
  void setSpeed(float a);
  void setDirection(float x, float y);

  // Getters
  float speed();
  Vector2 direction();

 private:
  float start_speed = 350;
  float move_speed;

  Vector2 dir = Vector2(0, 0);
};

#endif // BREAKOUT_MOVEMENTCOMPONENT_H
