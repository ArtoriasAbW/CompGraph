#ifndef MAIN_ROOM_H
#define MAIN_ROOM_H

#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <memory>
#include "Image.h"

struct Point
{
  int x;
  int y;
};

enum class TileType {
  WALL,
  FLOOR,
  EXIT,
  EMPTY,
  CLOSED_DOOR,
  OPENED_DOOR,
  KEY
};

enum class PlayerState {
  DEAD,
  ALIVE,
  WIN,
  AT_DOOR
};

enum class Direction {
  LEFT,
  RIGHT
};

using Picture = std::unique_ptr<Image, std::default_delete<Image>>;


struct Room {
  Room(std::string path, int room_type);
  std::vector<std::vector<TileType>> room_data;
  std::array<int, 4> room_idxs; // для перехода в другие комнаты
  Point player_start_pos;
  char type;
  int number;
  
  Picture wall;
  Picture floor;
  Picture empty;
  Picture exit;
  Picture opened_door;
  Picture closed_door;
  Picture key;
};


bool isType(Room &room, Point point, TileType type);

#endif