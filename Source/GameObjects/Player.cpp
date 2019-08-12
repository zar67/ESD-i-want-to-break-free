//
// Created by Zoe on 12/08/2019.
//

#include "Player.h"

bool Player::setup(ASGE::Renderer* renderer,
                   const std::string& file_name,
                   float x_start,
                   float y_start,
                   float start_speed,
                   float width,
                   float height)
{
  if (setupSprite(renderer, file_name, x_start, y_start, width, height))
  {
    start_x = x_start;
    start_y = y_start;
    setupMovement(start_speed);
    return true;
  }

  active(true);

  return false;
}

void Player::increaseScore(int a)
{
  score += a;
}

void Player::decreaseLives()
{
  lives -= 1;
}

void Player::reset()
{
  movement_component->reset();

  sprite_component->getSprite()->xPos(start_x);
  sprite_component->getSprite()->yPos(start_y);

  score = 0;
  lives = 3;
  can_shoot = false;
}

void Player::canShoot(bool s)
{
  can_shoot = s;
}

void Player::shootTimer(float t)
{
  shoot_timer = t;
}

int Player::playerScore()
{
  return score;
}

int Player::playerLives()
{
  return lives;
}

bool Player::canShoot()
{
  return can_shoot;
}

float Player::shootTimer()
{
  return shoot_timer;
}

void Player::resetPosition()
{
  sprite_component->getSprite()->xPos(start_x);
  sprite_component->getSprite()->yPos(start_y);
}
