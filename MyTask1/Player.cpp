#include "Player.h"
#include <iostream>

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      break;
    default:
      break;
  }
}

void Player::Draw(Image &screen)
{
  for(int y = coords.y; y <= coords.y + icon.Height(); ++y)
  {
    for(int x = coords.x; x <= coords.x + icon.Height(); ++x)
    {
      screen.PutPixel(x, y, blend(screen.GetPixel(x, y), icon.GetPixel(x - coords.x, icon.Height() - y + coords.y)));
    }
  }
}