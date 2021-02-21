#include "Room.h"
#include <iostream>

Room::Room(std::string path, char room_type) { // need to reverse data
    std::ifstream input;
    std::string pics_path = path + "/pics";
    floor = new Image(pics_path + "/floor.png");
    wall = new Image(pics_path + "/wall.png");
    empty = new Image(pics_path + "/empty.png");
    input.open(path + "/room" + room_type);
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
                }
        }
        room_data.push_back(std::move(tmp));
        ++j;
    }
    for (int k = 0; k < room_data.size() / 2; ++k) {
      std::swap(room_data[k], room_data[room_data.size() - k - 1]);
    }
    player_start_pos.y = (j - 1) * tileSize - player_start_pos.y;
}

void Room::_Print() {
  for (int i = 0; i < room_data.size(); ++i) {
    for (int j = 0; j < room_data[i].size(); ++j) {
      switch (room_data[i][j]) 
      {
      case TileType::FLOOR:
        std::cout << "floor";
        break;
      case TileType::WALL:
        std::cout << "wall";
        break;
      } 
    }
    std::cout << std::endl;
  }
}


bool isWall(Room &room, Point point) {
  if (room.room_data[point.y / tileSize][point.x / tileSize] == TileType::WALL) {
    return true;
  }
  return false;
}