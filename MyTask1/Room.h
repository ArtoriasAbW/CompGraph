#ifndef MAIN_ROOM_H
#define MAIN_ROOM_H

#include <vector>
#include <string>
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
  OPENED_DOOR
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
  Room(std::string path, char room_type);
  // ~Room();
  void _Print();
  std::vector<std::vector<TileType>> room_data;
  Point player_start_pos;
  char type;
  Picture wall;
  Picture floor;
  Picture empty;
  Picture exit;
  Picture opened_door;
  Picture closed_door;
};


bool isType(Room &room, Point point, TileType type);

#endif