//
// Created by Zoe on 12/08/2019.
//

#include "GameObject.h"

GameObject::~GameObject()
{
  freeSprite();
  freeMovement();
}

bool GameObject::setupSprite(ASGE::Renderer* renderer,
                             const std::string& file_name,
                             float x,
                             float y,
                             float width,
                             float height)
{
  freeSprite();

  start_x = x;
  start_y = y;

  sprite_component = new SpriteComponent();
  if (sprite_component->loadSprite(renderer, file_name))
  {
    sprite_component->getSprite()->xPos(x);
    sprite_component->getSprite()->yPos(y);
    sprite_component->getSprite()->width(width);
    sprite_component->getSprite()->height(height);

    return true;
  }

  freeSprite();
  return false;
}

void GameObject::setupMovement(float start_speed)
{
  freeMovement();
  movement_component = new MovementComponent(start_speed);
}

void GameObject::update(double delta_time)
{
  setPosition(
    movement_component->moveGameObject(delta_time, position().x, position().y));
}

SpriteComponent* GameObject::spriteComponent()
{
  return sprite_component;
}

MovementComponent* GameObject::movementComponent()
{
  return movement_component;
}

bool GameObject::active()
{
  return visible;
}

void GameObject::active(bool a)
{
  visible = a;
}

void GameObject::setPosition(float x, float y)
{
  sprite_component->getSprite()->xPos(x);
  sprite_component->getSprite()->yPos(y);
}

void GameObject::setPosition(Vector2 p)
{
  sprite_component->getSprite()->xPos(p.x);
  sprite_component->getSprite()->yPos(p.y);
}

void GameObject::setDirection(float x, float y)
{
  movement_component->setDirection(x, y);
}

void GameObject::setDirection(Vector2 d)
{
  movement_component->setDirection(d.x, d.y);
}

Vector2 GameObject::position()
{
  return Vector2(sprite_component->getSprite()->xPos(),
                 sprite_component->getSprite()->yPos());
}

Vector2 GameObject::direction()
{
  return movement_component->direction();
}

float GameObject::width()
{
  return sprite_component->getSprite()->width();
}

float GameObject::height()
{
  return sprite_component->getSprite()->height();
}

void GameObject::freeSprite()
{
  delete sprite_component;
  sprite_component = nullptr;
}

void GameObject::freeMovement()
{
  delete movement_component;
  movement_component = nullptr;
}

Rectangle GameObject::getRectangle()
{
  return Rectangle(position().x, position().y, width(), height());
}

void GameObject::reset()
{
  setPosition(start_x, start_y);
  visible = false;
}

Circle GameObject::getCircle()
{
  return Circle(position().x, position().y, width() / 2);
}
