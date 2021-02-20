#include <vector>

enum class TileType
{
  WALL,
  DOOR,
  FLOOR,
  EXIT
};

struct Room {
private:
    std::vector<std::vector<TileType>> room_data;
};