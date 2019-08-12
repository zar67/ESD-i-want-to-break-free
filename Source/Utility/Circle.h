//
// Created by Zoe on 09/08/2019.
//

#ifndef BREAKOUT_CIRCLE_H
#define BREAKOUT_CIRCLE_H

struct Circle
{
  Circle() = default;
  Circle(float x_pos, float y_pos, float r);

  float x = 0;
  float y = 0;
  float radius = 0;
};

#endif // PONGCLONE_CIRCLE_H
