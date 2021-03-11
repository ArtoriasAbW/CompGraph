#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Room.h"

constexpr int MAX_HP = 20;

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
                 coords(pos), old_coords(coords), state(PlayerState::ALIVE), health_points(MAX_HP), key_number(0)
                 {};

  bool Moved() const;
  void ProcessInput(MovementDir dir, Room &room);
  void Draw(Image &screen);
  PlayerState State() {return state;}
  void setState(PlayerState state_) {state = state_;}
  void setCoords(Point coords_) {
    coords.x = coords_.x;
    coords.y = coords_.y;
  }
  Point getCoords() {
    return coords;
  }

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 80, .g = 255, .b = 0, .a = 255};
  Image icon{"resources/player.png"};
  int move_speed = 3; 
  PlayerState state;
  Direction direction;
  int health_points;
  int key_number;
};

#endif //MAIN_PLAYER_H
