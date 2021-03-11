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
  KEY,
  ENEMY
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



class Enemy {
public:
  Enemy(Point begin, Point end) : cur_pos(begin), begin_pos(begin), end_pos(end)
  {
    if (begin_pos.x < end_pos.x || begin_pos.y < end_pos.y) {
      direction = true;
    } else {
      direction = false;
    }
    icon = std::make_unique<Image>("resources/pics/enemy.png");
  }
  void move();
  void draw(Image &screen);
  Point getCoords() { return cur_pos; }
private:
  bool direction; // true if right or up
  Point cur_pos;
  Point begin_pos;
  Point end_pos;
  Picture icon;

  int move_speed = 4;
};

class Room {
public:
  Room(std::string path, int room_type);
  std::vector<std::vector<TileType>> room_data;
  std::vector<Enemy> enemies;
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