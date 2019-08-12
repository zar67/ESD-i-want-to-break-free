//
// Created by Zoe on 08/08/2019.
//

#include "Vector2.h"
#include <math.h>

Vector2::Vector2(float dir_x, float dir_y)
{
  x = dir_x;
  y = dir_y;
}

Vector2::Vector2(const Vector2& rhs)
{
  x = rhs.x;
  y = rhs.y;
}

void Vector2::setAs(float dir_x, float dir_y)
{
  x = dir_x;
  y = dir_y;
}

void Vector2::multiplyBy(float scalar)
{
  x *= scalar;
  y *= scalar;
}

void Vector2::multiplyBy(float scalar_x, float scalar_y)
{
  x *= scalar_x;
  y *= scalar_y;
}

void Vector2::increaseBy(float increase)
{
  x += increase;
  y += increase;
}

void Vector2::increaseBy(float inc_x, float inc_y)
{
  x += inc_x;
  y += inc_y;
}

void Vector2::decreaseBy(float decrease)
{
  x -= decrease;
  y -= decrease;
}

void Vector2::decreaseBy(float dec_x, float dec_y)
{
  x -= dec_x;
  y -= dec_y;
}

void Vector2::normalise()
{
  float magnitude = sqrtf((x * x) + (y * y));

  if (!magnitude)
  {
    return;
  }

  x /= magnitude;
  y /= magnitude;
}

float Vector2::crossProduct(Vector2& vector)
{
  return (x * vector.y) - (y * vector.x);
}

float Vector2::dotProduct(Vector2& vector)
{
  return (x * vector.x) + (y * vector.y);
}

float Vector2::distance(float dest_x, float dest_y)
{
  float x_diff = x - dest_x;
  float y_diff = y - dest_y;
  return sqrt((x_diff * x_diff) + (y_diff * y_diff));
}

float Vector2::distance(Vector2& destination)
{
  float x_diff = x - destination.x;
  float y_diff = y - destination.y;
  return sqrt((x_diff * x_diff) + (y_diff * y_diff));
}

float Vector2::length()
{
  return sqrtf((x * x) + (y * y));
}
