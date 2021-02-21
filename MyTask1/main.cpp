#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Room.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;

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
        DrawTile(screen, *room.wall, base_x, base_y);
        break;
      case TileType::FLOOR:
        DrawTile(screen, *room.floor, base_x, base_y);
        break;
      case TileType::EMPTY:
        DrawTile(screen, *room.empty, base_x, base_y);
        break;
      case TileType::EXIT:
        // pict_path = room.exit_path;
        break;
      case TileType::CLOSED_DOOR:
        // pict_path = room.closed_door_path;
        break;
      case TileType::OPENED_DOOR:
        // pict_path = room.opened_door_path;
        break;
      }
      base_x += tileSize;
    }
    base_x = 0;
    base_y += tileSize;
  }

}

int main(int argc, char** argv)
{
	if(!glfwInit()) // GLFW library initialization
    return -1;

	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

  std::string room_path ="resources/roomA";
  Room cur_room(room_path, 'A');

  Point starting_pos{.x = cur_room.player_start_pos.x, .y = cur_room.player_start_pos.y};
	Player player{starting_pos};



  //game loop
	while (!glfwWindowShouldClose(window))
	{
    DrawRoom(screenBuffer, cur_room);
    // drawBackground(&screenBuffer);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents(); // proccess all pending

    processPlayerMovement(player, cur_room);
    player.Draw(screenBuffer);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window); // message exchange between back- and front- buffers
	}

	glfwTerminate();
	return 0;
}
