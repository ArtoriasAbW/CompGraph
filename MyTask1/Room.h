#ifndef MAIN_ROOM_H
#define MAIN_ROOM_H

#include <vector>
#include <string>
#include <fstream>
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


struct Room {
  Room(std::string path, char room_type);
  void _Print();
  std::vector<std::vector<TileType>> room_data;
  Point player_start_pos;
  char type;
  Image *wall;
  Image *floor;
  Image *empty;
  Image *exit;
  Image *opened_door;
  Image *closed_door;
};


bool isWall(Room &room, Point point);

#endif