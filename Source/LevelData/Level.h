//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_LEVEL_H
#define BREAKOUT_LEVEL_H

#include "GameConstants.h"
#include <bitset>
#include <string>

struct Level
{
  bool load(std::string file_name);

  int block_num = 0;
  int power_up_num = 0;

  float block_positions[SETTINGS::MAX_BLOCK_NUM][2];
  int power_up_blocks[SETTINGS::MAX_POWER_UP_NUM];
};

#endif // BREAKOUT_LEVEL_H
