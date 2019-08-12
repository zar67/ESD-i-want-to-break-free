//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H

#include "../Utility/Circle.h"
#include "../Utility/Rectangle.h"
#include "../Utility/Vector2.h"
#include "MovementComponent.h"
#include "SpriteComponent.h"
#include <string>

class GameObject
{
 public:
  // Constructor and (virtual) Destructor
  GameObject() = default;
  virtual ~GameObject();

  // Component adding functions
  bool setupSprite(ASGE::Renderer* renderer,
                   const std::string& file_name,
                   float x,
                   float y,
                   float width,
                   float height);

  void setupMovement(float start_speed);

  void update(double delta_time);

  // The components
  SpriteComponent* spriteComponent();
  MovementComponent* movementComponent();

  // Setters
  void active(bool a);
  void setPosition(float x, float y);
  void setPosition(Vector2 p);
  void setDirection(float x, float y);
  void setDirection(Vector2 d);
  virtual void reset();

  // Getters
  bool active();
  Vector2 position();
  Vector2 direction();
  float width();
  float height();
  Rectangle getRectangle();
  Circle getCircle();

 protected:
  void freeSprite();
  void freeMovement();

  SpriteComponent* sprite_component = nullptr;
  MovementComponent* movement_component = nullptr;

  float start_x = 0;
  float start_y = 0;

  bool visible = true;
};

#endif // BREAKOUT_GAMEOBJECT_H
