

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <list>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

class GridBox
{
public:
   bool isVisited;
   bool pathWay[4];

   GridBox()
   {
      isVisited = false;

      for (int i = 0; i < 4; i++)
      {
         pathWay[i] = false;
      }
   }
};

std::list<float> list_xy[6];

void display();
void RenderString(float x, float y, void *font, const unsigned char *msg, float r, float g, float b);
void display_win();

GridBox *grid;
int width, height;
int init_x, init_y;
int goal_x, goal_y;
double R, G, B;
int *chosen;
bool isWorking;
int state = 0;
int userInputLastDirection = -1;

GridBox &getPosXY(int x, int y)
{
   return grid[y * width + x];
}

void remove_line(int x, int y, int dest)
{
   // do remove line on the basis of coordinates
}

void update_labyrinth()
{
   int i;
   int x, y;

   for (i = 0; i < width * height; i++)
   {
      x = i % width;
      y = i / width;

      if (grid[i].pathWay[RIGHT] == true)
      {
         remove_line(x, y, RIGHT);
      }
      if (grid[i].pathWay[UP] == true)
      {
         remove_line(x, y, UP);
      }
      if (grid[i].pathWay[DOWN] == true)
      {
         remove_line(x, y, DOWN);
      }
      if (grid[i].pathWay[LEFT] == true)
      {
         remove_line(x, y, LEFT);
      }
   }
}

void choose_starting()
{
   int x, y;
   int dest = rand() % 2 + 1;
   if (dest == DOWN)
   {

      init_x = x = rand() % width;
      init_y = y = height - 1;
      getPosXY(x, y).pathWay[UP] = true;

      goal_x = x = rand() % width;
      goal_y = y = 0;
      getPosXY(x, y).pathWay[DOWN] = true;
   }
   else
   {
      init_x = x = width - 1;
      init_y = y = rand() % height;
      getPosXY(x, y).pathWay[RIGHT] = true;

      goal_x = x = 0;
      goal_y = y = rand() % height;
      getPosXY(x, y).pathWay[LEFT] = true;
   }
   chosen = new int[height * width];

   x = rand() % width;
   y = rand() % height;
   getPosXY(x, y).isVisited = true;
   chosen[0] = width * y + x;
}

void gen_maze()
{
   int x, y; // position of the current grid
   int dest; // direction of to be connected grid
   static int length = 0;
   int tmp;

   if (length == width * height)
   {
      state = 1;
      for (int i = 0; i < width * height; i++)
         grid[i].isVisited = false;
      return;
   }

   if (length == 0)
   {
      choose_starting();
      length = 1;
   }

   bool gridOpen = false;
   int counter = 0;
   while (!gridOpen)
   {
      tmp = chosen[rand() % length];
      x = tmp % width;
      y = tmp / width;

      dest = rand() % 4;
      counter++;
      switch (dest)
      {

      case UP:
         if (y == height - 1 || getPosXY(x, y + 1).isVisited == true)
            continue;

         getPosXY(x, y + 1).isVisited = true;

         getPosXY(x, y + 1).pathWay[DOWN] = true;
         getPosXY(x, y).pathWay[UP] = true;

         chosen[length] = width * (y + 1) + x;
         length++;
         gridOpen = true;
         break;

      case DOWN:
         if (y == 0 || getPosXY(x, y - 1).isVisited == true)
            continue;

         getPosXY(x, y - 1).isVisited = true;

         getPosXY(x, y - 1).pathWay[UP] = true;
         getPosXY(x, y).pathWay[DOWN] = true;

         chosen[length] = width * (y - 1) + x;
         length++;
         gridOpen = true;
         break;

      case RIGHT:
         if (x == width - 1 || getPosXY(x + 1, y).isVisited == true)
            continue;

         getPosXY(x + 1, y).isVisited = true;

         getPosXY(x + 1, y).pathWay[LEFT] = true;
         getPosXY(x, y).pathWay[RIGHT] = true;

         chosen[length] = width * y + x + 1;
         length++;
         gridOpen = true;
         break;

      case LEFT:
         if (x == 0 || getPosXY(x - 1, y).isVisited == true)
            continue;

         getPosXY(x - 1, y).isVisited = true;

         getPosXY(x - 1, y).pathWay[RIGHT] = true;
         getPosXY(x, y).pathWay[LEFT] = true;

         chosen[length] = width * y + x - 1;
         length++;
         gridOpen = true;
         break;
      }
   }
}

void input_width_and_height()
{
   while (1)
   {
      std::cout << "Input width, height (From 5 to 30)" << std::endl;
      std::cin >> width >> height;
      if (width > 30 || width < 5 || height > 30 || height < 5)
      {
         std::cout << "out of range!" << std::endl;
      }
      else
      {
         break;
      }
   }
}

void display_options()
{
   std::cout << std::endl;
   std::cout << "Enter key : start/stop" << std::endl;
   std::cout << "Arrow key : move the character" << std::endl;
   std::cout << std::endl
             << "SOLVE THE MAZE!!!" << std::endl;
}


GLFWwindow *initialise()
{
   // glfw: initialize and configure
   // ------------------------------
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // glfw window creation
   // --------------------
   GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      _Exit(-1);
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   // glad: load all OpenGL function pointers
   // ---------------------------------------
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "Failed to initialize GLAD" << std::endl;
      _Exit(-1);
   }
   return window;
}

const char *vertexShaderSource = "\n"
   "#version 330 core\n"
   "layout (location = 0) in vec3 aPos;\n"
   "layout (location = 1) in vec3 aColor;\n"
   "out vec3 ourColor;\n"
   "void main()\n"
   "{\n"
   "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
   "  ourColor = aColor;\n"
   "}\0";

const char *fragmentShaderSource = "\n"
   "#version 330 core\n"
   "out vec4 FragColor;\n"
   "in vec3 ourColor;\n"
   "void main()\n"
   "{\n"
   "  FragColor = vec4(ourColor, 1.0);\n"
   "}\n\0";

static unsigned int CompileShader(unsigned int type,const char *typeShaderSource ){
   unsigned int shader = glCreateShader(type);
   glShaderSource(shader, 1, &typeShaderSource, NULL);
   glCompileShader(shader);

   // check for shader compile errors
   int success;
   char infoLog[512];
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
   }
   return shader;
}

static unsigned int CreateShader(){
   int success;
   char infoLog[512];
   unsigned int shaderProgram = glCreateProgram();
   unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER,vertexShaderSource);
   unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER,fragmentShaderSource);
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   // check for linking errors
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
   }
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   return shaderProgram;
}

int main()
{
   // input_width_and_height();
   height = width = 10;

   display_options();

   grid = new GridBox[width * height];
   for (double x = 1; x < width + 2; x++)
   {
      list_xy[0].push_back((float)x * 10);
      list_xy[1].push_back((float)10);
      list_xy[2].push_back((float)0);
      for (int i = 3; i < 6; i++)
      {
         list_xy[i].push_back((float)1);
      }
      list_xy[0].push_back((float)x * 10);
      list_xy[1].push_back((float)height * 10 + 10.0);
      list_xy[2].push_back((float)0);
      for (int i = 3; i < 6; i++)
      {
         list_xy[i].push_back((float)1);
      }
      // glVertex2f(x * 10, 10.0);
      // glVertex2f(x * 10, height * 10 + 10.0);
   }
   for (double x = 1; x < height + 2; x++)
   {
      list_xy[0].push_back((float)10);
      list_xy[1].push_back((float)x * 10);
      list_xy[2].push_back((float)0);
      for (int i = 3; i < 6; i++)
      {
         list_xy[i].push_back((float)1);
      }
      list_xy[0].push_back((float)width * 10 + 10.0);
      list_xy[1].push_back((float)x * 10);
      list_xy[2].push_back((float)0);
      for (int i = 3; i < 6; i++)
      {
         list_xy[i].push_back((float)1);
      }
      // glVertex2f(10.0, x * 10);
      // glVertex2f(width * 10 + 10.0, x * 10);
   }

   GLFWwindow *window = initialise();

   unsigned int shaderProgram = CreateShader();

   // set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
   
   float vertices[] = {
      // positions         // colors
      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
      0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
   };  

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   // color attribute
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
   glEnableVertexAttribArray(1);

   // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
   // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
   glBindVertexArray(0);

   // uncomment this call to draw in wireframe polygons.
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // render loop
   // -----------
   while (!glfwWindowShouldClose(window))
   {
      // input
      // -----
      processInput(window);

      // render
      // ------
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // draw our first triangle
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // glBindVertexArray(0); // no need to unbind it every time

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   // optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteProgram(shaderProgram);

   // glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
   glfwTerminate();
   return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
   // make sure the viewport matches the new window dimensions; note that width and
   // height will be significantly larger than specified on retina displays.
   glViewport(0, 0, width, height);
}
