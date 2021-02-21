#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Room.h"

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  void ProcessInput(MovementDir dir, Room &room);
  void Draw(Image &screen);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 80, .g = 255, .b = 0, .a = 255};
  Image icon{"resources/player.png"};
  int move_speed = 4; 
};

#endif //MAIN_PLAYER_H
