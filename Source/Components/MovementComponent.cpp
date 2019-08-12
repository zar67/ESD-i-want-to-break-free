//
// Created by Zoe on 12/08/2019.
//

#include "MovementComponent.h"

MovementComponent::MovementComponent(float initial_speed)
{
  start_speed = initial_speed;
  reset();
}

Vector2 MovementComponent::moveGameObject(double delta, float x, float y)
{
  float new_x = x + float(move_speed * dir.x * delta);
  float new_y = y + float(move_speed * dir.y * delta);

  return Vector2(new_x, new_y);
}

void MovementComponent::reset()
{
  move_speed = start_speed;
  dir = Vector2(0, 0);
}

void MovementComponent::increaseSpeed(float a)
{
  move_speed += a;
}

void MovementComponent::setSpeed(float a)
{
  move_speed = a;
}

void MovementComponent::setDirection(float x, float y)
{
  dir.x = x;
  dir.y = y;
  dir.normalise();
}

float MovementComponent::speed()
{
  return move_speed;
}

Vector2 MovementComponent::direction()
{
  return dir;
}
