#include "Player.h"
#include <iostream>

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}


void Player::ProcessInput(MovementDir dir, Room &room) { // TODO: need to fix
  int move_dist = 4;
  Point neighbour_cell1 = {.x = coords.x, .y = coords.y};
  Point neighbour_cell2 = {.x = coords.x, .y = coords.y};
  int new_x = coords.x;
  int new_y = coords.y;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      new_y += move_dist;
      neighbour_cell1.y += tileSize + move_dist - 1;
      neighbour_cell2.y += move_dist + tileSize - 1;
      neighbour_cell2.x += tileSize - 1;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      // coords.y -= move_dist;
      new_y -= move_dist;
      neighbour_cell1.y -= move_dist;
      neighbour_cell2.y -= move_dist;
      neighbour_cell2.x += tileSize - 1;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      // coords.x -= move_dist;
      new_x -= move_dist;
      neighbour_cell1.x -= move_dist;
      neighbour_cell2.x -= move_dist;
      neighbour_cell1.y += tileSize - 1;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      // coords.x += move_dist;
      new_x += move_dist;
      neighbour_cell1.x += move_dist + tileSize - 1;
      neighbour_cell2.x += move_dist + tileSize - 1;
      neighbour_cell2.y += tileSize - 1;
      break;
    default:
      break;
  }
  if (!(isWall(room, neighbour_cell1) || isWall(room, neighbour_cell2))) {
    coords.x = new_x;
    coords.y = new_y;
  }
}

void Player::Draw(Image &screen)
{
  for(int y = coords.y; y <= coords.y + tileSize; ++y)
  {
    for(int x = coords.x; x <= coords.x + tileSize; ++x)
    {
      screen.PutPixel(x, y, blend(screen.GetPixel(x, y), icon.GetPixel(x - coords.x, tileSize - y + coords.y)));
    }
  }
}