//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_UIHANDLER_H
#define BREAKOUT_UIHANDLER_H

#include "../Components/GameObject.h"
#include <Engine/Mouse.h>
#include <string>

struct UIHandler
{
  void setupUIObjects(ASGE::Renderer* renderer, float width, float height);
  bool updateButton(int action, ASGE::Sprite* sprite);

  GameObject menu_play_button;
  GameObject menu_quit_button;

  GameObject game_end_play_button;
  GameObject game_end_menu_button;
  GameObject game_end_quit_button;

  GameObject pause_resume_button;
  GameObject pause_menu_button;
  GameObject pause_quit_button;

  GameObject next_level_button;

  std::string pressed_sprite = "data/Textures/UI/"
                               "button_long_dark_grey_pressed.png";
  std::string normal_sprite = "data/Textures/UI/button_long_grey.png";

  float button_width = 0;
  float button_height = 0;
};

#endif // BREAKOUT_UIHANDLER_H
