//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_PLAYER_H
#define BREAKOUT_PLAYER_H

#include "../Components/GameObject.h"
#include "../Utility/Rectangle.h"
#include <Engine/Sprite.h>

class Player : public GameObject
{
 public:
  Player() = default;
  ~Player() override = default;

  bool setup(ASGE::Renderer* renderer,
             const std::string& file_name,
             float x_start,
             float y_start,
             float start_speed,
             float width,
             float height);

  // Setters
  void increaseScore(int a);
  void decreaseLives();
  void reset() override;
  void resetPosition();

  void canShoot(bool s);
  void shootTimer(float t);

  // Getters
  int playerScore();
  int playerLives();
  bool canShoot();
  float shootTimer();

 private:
  float start_x = 0;
  float start_y = 0;

  bool can_shoot = false;
  float shoot_timer = 0;

  int score = 0;
  int lives = 3;
};

#endif // BREAKOUT_PLAYER_H
