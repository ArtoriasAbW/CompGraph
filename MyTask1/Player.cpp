#include "Player.h"
#include <iostream>

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}


void Player::ProcessInput(MovementDir dir, Room &room) { 
  int move_dist = move_speed;
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
      neighbour_cell2.x += tileSize - 6;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      new_y -= move_dist;
      neighbour_cell1.y -= move_dist;
      neighbour_cell2.y -= move_dist;
      neighbour_cell2.x += tileSize - 6;
      break;
    case MovementDir::LEFT:
      direction = Direction::LEFT;
      old_coords.x = coords.x;
      new_x -= move_dist;
      neighbour_cell1.x -= move_dist;
      neighbour_cell2.x -= move_dist;
      neighbour_cell1.y += tileSize - 5;
      break;
    case MovementDir::RIGHT:
      direction = Direction::RIGHT;
      old_coords.x = coords.x;
      new_x += move_dist;
      neighbour_cell1.x += move_dist + tileSize - 1;
      neighbour_cell2.x += move_dist + tileSize - 1;
      neighbour_cell2.y += tileSize - 5;
      break;
    default:
      break;
  }
  // TODO: обобщить для всех клеток
  int old_x = coords.x;
  int old_y = coords.y;
  coords.x = new_x;
  coords.y = new_y;
  if (isType(room, neighbour_cell1, TileType::OPENED_DOOR) || isType(room, neighbour_cell2, TileType::OPENED_DOOR)) {
    state = PlayerState::AT_DOOR;
  } else if (isType(room, neighbour_cell1, TileType::WALL) || isType(room, neighbour_cell2, TileType::WALL)) {
    coords.x = old_x;
    coords.y = old_y;
  } else if (isType(room, neighbour_cell1, TileType::CLOSED_DOOR)) {
    coords.x = old_x;
    coords.y = old_y;
    if (key_number > 0) {
      key_number -= 1;
      room.room_data[neighbour_cell1.y / tileSize][neighbour_cell1.x / tileSize] = TileType::OPENED_DOOR;
    }
  } else if (isType(room, neighbour_cell2, TileType::CLOSED_DOOR)) {
    coords.x = old_x;
    coords.y = old_y;
    if (key_number > 0) {
      key_number -= 1;
      room.room_data[neighbour_cell2.y / tileSize][neighbour_cell2.x / tileSize] = TileType::OPENED_DOOR;
    }
  } else if (isType(room, neighbour_cell1, TileType::EMPTY) || isType(room, neighbour_cell2, TileType::EMPTY)) {
    state = PlayerState::DEAD;
  } else if (isType(room, neighbour_cell1, TileType::EXIT) || isType(room, neighbour_cell2, TileType::EXIT)) {
    state = PlayerState::WIN;
  } else if (isType(room, neighbour_cell1, TileType::KEY)) {
    room.room_data[neighbour_cell1.y / tileSize][neighbour_cell1.x / tileSize] = TileType::FLOOR;
    key_number += 1;
  } else if (isType(room, neighbour_cell2, TileType::KEY)) {
    room.room_data[neighbour_cell2.y / tileSize][neighbour_cell2.x / tileSize] = TileType::FLOOR;
    key_number += 1;
  }
}

void Player::Draw(Image &screen) {
  for(int y = coords.y; y <= coords.y + tileSize; ++y)
  {
    for(int x = coords.x; x <= coords.x + tileSize; ++x)
    {
      screen.PutPixel(x, y, blend(screen.GetPixel(x, y), 
      icon.GetPixel(direction == Direction::RIGHT ? tileSize - x + coords.x: x - coords.x, tileSize - y + coords.y)));
    }
  }
}
