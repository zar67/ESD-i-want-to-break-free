//
// Created by Zoe on 12/08/2019.
//

#include "Level.h"
#include <fstream>

bool Level::load(std::string file_name)
{
  std::fstream file(file_name);

  /*
      Level File Setup:
      Number of blocks
      Positions (x,y  x,y  x,y  etc...)
  */

  if (file.is_open())
  {
    file >> block_num;

    if (block_num > SETTINGS::MAX_BLOCK_NUM)
    {
      block_num = SETTINGS::MAX_BLOCK_NUM;
    }

    char dummy;
    float x, y;

    // Get positions
    for (int j = 0; j < block_num; j++)
    {
      file >> x >> dummy >> y;

      block_positions[j][0] = x;
      block_positions[j][1] = y;
    }

    file >> power_up_num;

    if (power_up_num > SETTINGS::MAX_POWER_UP_NUM)
    {
      power_up_num = SETTINGS::MAX_POWER_UP_NUM;
    }

    // Get power up indexes
    for (int j = 0; j < power_up_num; j++)
    {
      file >> x >> dummy;

      power_up_blocks[j] = static_cast<int>(x);
    }

    return true;
  }

  return false;
}
