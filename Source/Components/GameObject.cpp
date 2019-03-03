#include "GameObject.h"
#include <Engine/Renderer.h>

GameObject::~GameObject()
{
  free();
}

bool GameObject::addSpriteComponent(ASGE::Renderer* renderer,
                                    const std::string& texture_file_name)
{
  free();

  sprite_component = new SpriteComponent();
  if (sprite_component->loadSprite(renderer, texture_file_name))
  {
    return true;
  }

  free();
  return false;
}

void GameObject::free()
{
  delete sprite_component;
  sprite_component = nullptr;
}

SpriteComponent* GameObject::spriteComponent()
{
  return sprite_component;
}

void GameObject::visibility(bool v)
{
  visible = v;
}

bool GameObject::visibility()
{
  return visible;
}

float GameObject::speed()
{
  return speed_;
}

void GameObject::speed(float s)
{
  speed_ = s;
}

vector2 GameObject::direction()
{
  return velocity;
}

void GameObject::direction(float x_, float y_)
{
  velocity.x = x_;
  velocity.y = y_;
}

void GameObject::canShoot(bool s)
{
  can_shoot = s;
}

bool GameObject::canShoot()
{
  return can_shoot;
}

void GameObject::shootTimer(float t)
{
  shoot_timer = t;
}

float GameObject::shootTimer()
{
  return float(shoot_timer);
}
