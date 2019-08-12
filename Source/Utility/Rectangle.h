//
// Created by Zoe on 09/08/2019.
//

#ifndef BREAKOUT_RECTANGLE_H
#define BREAKOUT_RECTANGLE_H

struct Rectangle
{
  Rectangle() = default;
  Rectangle(float x_pos, float y_pos, float w, float h);

  float x = 0;
  float y = 0;
  float width = 0;
  float height = 0;
};

#endif // PONGCLONE_RECTANGLE_H
