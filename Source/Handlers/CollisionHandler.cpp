//
// Created by Zoe on 12/08/2019.
//

#include "CollisionHandler.h"
#include <iostream>
#include <math.h>

const int SIDE_LEFT = 1;
const int SIDE_RIGHT = 2;
const int SIDE_TOP = 3;
const int SIDE_BOTTOM = 4;
const int INVALID = 0;

Vector2 CollisionHandler::calculateNewDir(Ball* ball, Player* player)
{
  float half_player_width = player->width() / 2;

  // Calculate where the ball hit by a number from -1 to 1.
  float paddle_intersect = (player->position().x + half_player_width) -
                           (ball->position().x + (ball->width() / 2));
  float normalised_intersect = paddle_intersect / half_player_width;

  // Convert the number to a radian value. 1.309 radians = 60 degrees
  double bounce_angle = normalised_intersect * radian_boundary;

  // Get the direction the ball should go in.
  auto new_x_dir =
    float(ball->movementComponent()->speed() * -sin(bounce_angle));
  auto new_y_dir =
    float(ball->movementComponent()->speed() * cos(bounce_angle));

  Vector2 new_dir = Vector2(new_x_dir, -new_y_dir);
  new_dir.normalise();

  return Vector2(new_dir.x, new_dir.y);
}

Vector2
CollisionHandler::AABBCircleCollision(Circle circle, Rectangle rectangle)
{
  float closest_x =
    fmaxf(rectangle.x,
          fminf((circle.x + circle.radius), (rectangle.x + rectangle.width)));
  float x_diff = (circle.x + circle.radius) - closest_x;

  float closest_y =
    fmaxf(rectangle.y,
          fminf((circle.y + circle.radius), (rectangle.y + rectangle.height)));
  float y_diff = (circle.y + circle.radius) - closest_y;

  if ((circle.radius * circle.radius) > ((x_diff * x_diff) + (y_diff * y_diff)))
  {
    return Vector2(closest_x, closest_y);
  }
  return Vector2(0, 0);
}

bool CollisionHandler::AABBAABBCollision(Rectangle rect_one, Rectangle rect_two)
{
  float closest_x =
    fmaxf(rect_one.x,
          fminf(rect_two.x + rect_two.width, rect_one.x + rect_one.width));
  float x_diff = (rect_two.x + rect_two.width) - closest_x;

  float closest_y =
    fmaxf(rect_one.y,
          fminf(rect_two.y + rect_two.height, rect_one.y + rect_one.height));
  float y_diff = (rect_two.y + rect_two.height) - closest_y;

  return (x_diff < rect_two.width && y_diff < rect_two.height);
}

int CollisionHandler::getCollisionSide(Vector2 point,
                                       Rectangle col_shape,
                                       Vector2 ball_dir)
{
  if (point.x == col_shape.x && ball_dir.x > 0)
  {
    return SIDE_LEFT;
  }
  else if (point.x == col_shape.x + col_shape.width && ball_dir.x < 0)
  {
    return SIDE_RIGHT;
  }
  else if (point.y == col_shape.y && ball_dir.y > 0)
  {
    return SIDE_TOP;
  }
  else if (point.y == col_shape.y + col_shape.height && ball_dir.y < 0)
  {
    return SIDE_BOTTOM;
  }
  return INVALID;
}

void CollisionHandler::ballCollisionHandler(Ball* ball,
                                            Player* player,
                                            int game_width)
{
  ballBoundaryCollision(ball, game_width);
  ballPlayerCollision(ball, player);
}

void CollisionHandler::ballBoundaryCollision(Ball* ball, int game_width)
{
  // Hit top boundary
  if (ball->position().y <= 0 && ball->direction().y < 0)
  {
    ball->setPosition(ball->position().x, 0);
    ball->setDirection(ball->direction().x, -ball->direction().y);
  }

  // Hit left boundary
  if (ball->position().x <= 0 && ball->direction().x < 0)
  {
    ball->setPosition(0, ball->position().y);
    ball->setDirection(-ball->direction().x, ball->direction().y);
  }

  // Hit right boundary
  if (ball->position().x >= static_cast<float>(game_width) - ball->width() &&
      ball->direction().x > 0)
  {
    ball->setPosition(static_cast<float>(game_width) - ball->width(),
                      ball->position().y);
    ball->setDirection(-ball->direction().x, ball->direction().y);
  }
}

void CollisionHandler::ballPlayerCollision(Ball* ball, Player* player)
{
  Vector2 point =
    AABBCircleCollision(ball->getCircle(), player->getRectangle());

  if (point.x != INVALID || point.y != INVALID)
  {
    int side =
      getCollisionSide(point, player->getRectangle(), ball->direction());

    switch (side)
    {
      case SIDE_TOP:
      {
        ball->setPosition(ball->position().x,
                          player->position().y - ball->height());
        ball->setDirection(calculateNewDir(ball, player));
        // ball->setDirection(ball->direction().x, -ball->direction().y);
        break;
      }
      case SIDE_LEFT:
      {
        ball->setPosition(player->position().x - ball->width(),
                          ball->position().y);
        ball->setDirection(-ball->direction().x, ball->direction().y);
        break;
      }
      case SIDE_RIGHT:
      {
        ball->setPosition(player->position().x + player->width(),
                          ball->position().y);
        ball->setDirection(-ball->direction().x, ball->direction().y);
        break;
      }
      default:
        break;
    }
  }
}

bool CollisionHandler::ballBlockCollision(Ball* ball, GameObject* block)
{
  Vector2 point = AABBCircleCollision(ball->getCircle(), block->getRectangle());

  if (point.x != INVALID || point.y != INVALID)
  {
    int side =
      getCollisionSide(point, block->getRectangle(), ball->direction());

    switch (side)
    {
      case SIDE_TOP:
      {
        ball->setPosition(ball->position().x,
                          block->position().y - ball->height());
        ball->setDirection(ball->direction().x, -ball->direction().y);
        break;
      }
      case SIDE_BOTTOM:
      {
        ball->setPosition(ball->position().x,
                          block->position().y + block->height());
        ball->setDirection(ball->direction().x, -ball->direction().y);
        break;
      }
      case SIDE_LEFT:
      {
        ball->setPosition(block->position().x - ball->width(),
                          ball->position().y);
        ball->setDirection(-ball->direction().x, ball->direction().y);
        break;
      }
      case SIDE_RIGHT:
      {
        ball->setPosition(block->position().x + block->width(),
                          ball->position().y);
        ball->setDirection(-ball->direction().x, ball->direction().y);
        break;
      }
      default:
        break;
    }

    return true;
  }

  return false;
}

void CollisionHandler::playerBoundaryCollision(Player* player, int game_width)
{
  if (player->position().x < 0)
  {
    player->setDirection(0, 0);
    player->setPosition(0, player->position().y);
  }
  else if (player->position().x >
           static_cast<float>(game_width) - player->width())
  {
    player->setDirection(0, 0);
    player->setPosition(static_cast<float>(game_width) - player->width(),
                        player->position().y);
  }
}

bool CollisionHandler::isInside(const ASGE::Sprite* sprite,
                                double x,
                                double y) const
{
  return (x > sprite->xPos() && x < (sprite->xPos() + sprite->width()) &&
          y > sprite->yPos() && y < (sprite->yPos() + sprite->height()));
}
