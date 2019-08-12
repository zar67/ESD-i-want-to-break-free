//
// Created by Zoe on 12/08/2019.
//

#include "UIHandler.h"

void UIHandler::setupUIObjects(ASGE::Renderer* renderer,
                               float width,
                               float height)
{
  menu_play_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 344, width, height);
  menu_quit_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 394, width, height);

  game_end_play_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 401, width, height);
  game_end_menu_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 451, width, height);
  game_end_quit_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 501, width, height);

  pause_resume_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 301, width, height);
  pause_menu_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 351, width, height);
  pause_quit_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 401, width, height);

  next_level_button.setupSprite(
    renderer, "data/Textures/UI/button_long_grey.png", 220, 420, width, height);

  button_width = width;
  button_height = height;
}

bool UIHandler::updateButton(int action, ASGE::Sprite* sprite)
{
  if (action == ASGE::MOUSE::BUTTON_PRESSED)
  {
    sprite->loadTexture(pressed_sprite);
    sprite->width(button_width);
    sprite->height(button_height);

    return false;
  }
  else
  {
    sprite->loadTexture(normal_sprite);
    sprite->width(button_width);
    sprite->height(button_height);

    return true;
  }
}
