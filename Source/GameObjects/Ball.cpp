//
// Created by Zoe on 12/08/2019.
//

#include "Ball.h"

bool Ball::setup(ASGE::Renderer* renderer,
                 const std::string& file_name,
                 float x_start,
                 float y_start,
                 float start_speed,
                 float width,
                 float height)
{
  start_x = x_start;
  start_y = y_start;

  if (setupSprite(renderer, file_name, x_start, y_start, width, height))
  {
    setupMovement(start_speed);
    reset();
    return true;
  }

  active(true);

  return false;
}

void Ball::reset()
{
  movement_component->reset();

  sprite_component->getSprite()->xPos(start_x);
  sprite_component->getSprite()->yPos(start_y);

  generateDirection();
}

void Ball::generateDirection()
{
  float new_x_dir = static_cast<float>(std::rand() % 3 - 3);
  float new_y_dir = -2;

  setDirection(new_x_dir, new_y_dir);
}
