#include "Room.h"
#include <iostream>

Room::Room(std::string path, int room_type) { 
    std::ifstream input;
    std::string pics_path = path + "pics";
    floor = std::make_unique<Image>(pics_path + "/floor.png");
    wall = std::make_unique<Image>(pics_path + "/wall.png");
    empty = std::make_unique<Image>(pics_path + "/empty.png");
    opened_door = std::make_unique<Image>(pics_path + "/opened_door.png");
    closed_door = std::make_unique<Image>(pics_path + "/closed_door.png");
    exit = std::make_unique<Image>(pics_path + "/exit.png");
    key = std::make_unique<Image>(pics_path + "/key.png");
    input.open(path + std::to_string(room_type));
    std::string line;
    int j = 0;
    while (!input.eof()) {
        getline(input, line);
        std::vector<TileType> tmp;
        for (int i = 0; i < line.size(); ++i) {
            switch (line[i]) {
                case '#': 
                    tmp.push_back(TileType::WALL);
                    break;
                case '.':
                    tmp.push_back(TileType::FLOOR);
                    break;
                case ' ':
                tmp.push_back(TileType::EMPTY);
                    break;
                case '@':
                  tmp.push_back(TileType::FLOOR);
                  player_start_pos.x = i * tileSize;
                  player_start_pos.y = j * tileSize;
                  break;
                case 'Q':
                  tmp.push_back(TileType::EXIT);
                  break;
                case 'x':
                  tmp.push_back(TileType::CLOSED_DOOR);
                  break;
                case 'X':
                  tmp.push_back(TileType::OPENED_DOOR);
                  break;
                case 'K':
                  tmp.push_back(TileType::KEY);
                  break;
                case 'e':
                  tmp.push_back(TileType::ENEMY);
                  break;
                }
        }
        room_data.push_back(std::move(tmp));
        ++j;
    }
    input.close();
    for (int k = 0; k < room_data.size() / 2; ++k) {
      std::swap(room_data[k], room_data[room_data.size() - k - 1]);
    }
    player_start_pos.y = (j - 1) * tileSize - player_start_pos.y;
}


bool isType(Room &room, Point point, TileType type) {
  if (room.room_data[point.y / tileSize][point.x / tileSize] == type) {
    return true;
  }
  return false;
}



// straight motion only
void Enemy::move() {
  if (begin_pos.x != end_pos.x) {
    if (direction) {
      if (cur_pos.x < end_pos.x - move_speed) {
        cur_pos.x += move_speed;
      } else {
        cur_pos.x = end_pos.x;
        direction = false;
      }
    } else {
      if (cur_pos.x > begin_pos.x + move_speed) {
        cur_pos.x -= move_speed;
      } else {
        cur_pos.x = begin_pos.x;
        direction = true;
      }
    }
  } else {
    if (direction) {
      if (cur_pos.y < end_pos.y - move_speed) {
        cur_pos.y += move_speed;
      } else {
        cur_pos.y = end_pos.y;
        direction = false;
      }
    } else {
      if (cur_pos.y > begin_pos.y + move_speed) {
        cur_pos.y -= move_speed;
      } else {
        cur_pos.y = begin_pos.y;
        direction = true;
      }
    }
  }
}

void Enemy::draw(Image &screen) {
  for(int y = cur_pos.y; y <= cur_pos.y + tileSize; ++y)
  {
    for(int x = cur_pos.x; x <= cur_pos.x + tileSize; ++x)
    {
      screen.PutPixel(x, y, blend(screen.GetPixel(x, y), 
      icon.get()->GetPixel(direction ? tileSize - x + cur_pos.x : x - cur_pos.x, tileSize - y + cur_pos.y)));
    }
  }
}