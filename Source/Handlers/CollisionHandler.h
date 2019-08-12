//
// Created by Zoe on 12/08/2019.
//

#ifndef BREAKOUT_COLLISIONHANDLER_H
#define BREAKOUT_COLLISIONHANDLER_H

#include "../GameObjects/Ball.h"
#include "../GameObjects/Player.h"
#include "../Utility/Circle.h"
#include "../Utility/Rectangle.h"
#include "../Utility/Vector2.h"
#include <Engine/Sprite.h>

class CollisionHandler
{
 public:
  CollisionHandler() = default;
  ~CollisionHandler() = default;

  Vector2 calculateNewDir(Ball* ball, Player* player);

  Vector2 AABBCircleCollision(Circle circle, Rectangle rectangle);
  bool AABBAABBCollision(Rectangle rect_one, Rectangle rect_two);
  int getCollisionSide(Vector2 point, Rectangle col_shape, Vector2 ball_dir);

  void ballCollisionHandler(Ball* ball, Player* player, int game_width);

  void ballBoundaryCollision(Ball* ball, int game_width);
  void ballPlayerCollision(Ball* ball, Player* player);
  bool ballBlockCollision(Ball* ball, GameObject* block);

  void playerBoundaryCollision(Player* player, int game_width);

  bool isInside(const ASGE::Sprite* sprite, double x, double y) const;

 private:
  const float radian_boundary = 1.309f;
};

#endif // BREAKOUT_COLLISIONHANDLER_H
