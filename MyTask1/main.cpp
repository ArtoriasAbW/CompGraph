#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Room.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 700, WINDOW_HEIGHT = 720;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player, Room &room)
{
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MovementDir::UP, room);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MovementDir::DOWN, room);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MovementDir::LEFT, room);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MovementDir::RIGHT, room);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

Point EnterTheDoor(Point old) {
  std::cout << old.x << " " << old.y << std::endl;
  if (old.x > WINDOW_WIDTH - 3 * tileSize) {
    std::cout  << "1" << std::endl;
    return {tileSize, old.y};
  }
  if (old.x <= tileSize) {
    std::cout  << "2" << std::endl;
    return {WINDOW_WIDTH - 3 * tileSize, old.y};
  }
  if (old.y > WINDOW_HEIGHT - tileSize) {
    std::cout  << "3" << std::endl;
    return {old.x, 2 * tileSize};
  }
  std::cout  << "4" << std::endl;
  return {old.x, WINDOW_HEIGHT - 2 * tileSize};
}

void DrawTile(Image &screen, Image &picture, int x, int y) {
  for (int i = 0; i < tileSize; ++i) {
    for (int j = 0; j < tileSize; ++j) {
      screen.PutPixel(x + i, y + j, picture.GetPixel(i, tileSize - j - 1));
    }
  }
}


void DrawRoom(Image &screen, Room &room) {
  int height = room.room_data.size();
  int base_x = 0;
  int base_y = 0;
  int pict_height = 0;
  int pict_width = 0;
  std::string pict_path;
  for (int i = 0; i < height; ++i) {
    int width = room.room_data[i].size();
    for (int j = 0; j < width; ++j) {
      switch (room.room_data[i][j])
      {
      case TileType::WALL:
        DrawTile(screen, *room.wall.get(), base_x, base_y);
        break;
      case TileType::FLOOR:
        DrawTile(screen, *room.floor.get(), base_x, base_y);
        break;
      case TileType::EMPTY:
        DrawTile(screen, *room.empty.get(), base_x, base_y);
        break;
      case TileType::EXIT:
        DrawTile(screen, *room.exit.get(), base_x, base_y);
        break;
      case TileType::CLOSED_DOOR:
        DrawTile(screen, *room.closed_door.get(), base_x, base_y);
        break;
      case TileType::OPENED_DOOR:
        DrawTile(screen, *room.opened_door.get(), base_x, base_y);
        break;
      }
      base_x += tileSize;
    }
    base_x = 0;
    base_y += tileSize;
  }
}

Room *nextRoom(std::vector<Room> &rooms, int idx) {
  return &rooms[idx % rooms.size()];
}

void DrawEndOfGame(Image &buffer, Image &picture) {
  int base_x = 186;
  int base_y = 283;
  for (int i = 0; i < picture.Width(); ++i) {
    for (int j = 0; j < picture.Height(); ++j) {
      buffer.PutPixel(base_x + i, base_y + j, picture.GetPixel(i, picture.Height() - j - 1));
    }
  }
}

int main(int argc, char** argv)
{
	if(!glfwInit()) // GLFW library initialization
    return -1;

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My game", nullptr, nullptr); // application window
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); // make window context current

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);



	if(initGL() != 0)  // loading pointers on OpenGL functions
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();


	
  
	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS; // set size of the visiable part of the window
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

  std::ifstream lab;
  lab.open("resources/rooms");
  int room_number;
  lab >> room_number;
  char room_type;
  std::string room_path ="resources/room";
  std::vector<Room> rooms;
  for (int i = 0; i < room_number; ++i) {
    lab >> room_type;
    Room room1(room_path, room_type);
    rooms.push_back(std::move(room1));
  }
  lab.close();  

  int room_idx = 0;
  Room *cur_room = nextRoom(rooms, room_idx);
  Point starting_pos{.x = cur_room->player_start_pos.x, .y = cur_room->player_start_pos.y};
	Player player{starting_pos};

  Image lose("resources/lose.png");
  Image win("resources/win.png");

  GLfloat old_time = glfwGetTime();
  GLfloat cur_time;
  //game loop
	while (!glfwWindowShouldClose(window))
	{
    DrawRoom(screenBuffer, *cur_room);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents(); // proccess all pending
    if (player.State() == PlayerState::ALIVE) {
      processPlayerMovement(player, *cur_room);
    }
    player.Draw(screenBuffer);
    if (player.State() == PlayerState::DEAD) {
      DrawEndOfGame(screenBuffer, lose);    
    }
    if (player.State() == PlayerState::WIN) {
      DrawEndOfGame(screenBuffer, win);
    }
    if (player.State() == PlayerState::AT_DOOR) {
      cur_time = glfwGetTime();
      GLfloat delta =  cur_time - old_time;
      old_time = cur_time;
      if (delta > 0.1) { // чтобы избежать двойного перехода сквозь дверь
        ++room_idx;
        if (room_idx == room_number) {
          room_idx = 0;
        }
        cur_room = nextRoom(rooms, room_idx);
        player.setCoords(EnterTheDoor(player.getCoords()));
      }
      player.setState(PlayerState::ALIVE);
    }
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window); // message exchange between back- and front- buffers
	}

	glfwTerminate();
	return 0;
}
