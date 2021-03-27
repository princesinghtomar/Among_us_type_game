

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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

GridBox *grid;
float *vertices;
int width = 5, height = 5;
int init_x, init_y;
int goal_x, goal_y;
int *chosen;
int count_vertices = 0;
int stop = true;

GridBox &getPosXY(int x, int y)
{
   return grid[y * width + x];
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
      stop = false;
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
      //   std::cout <<" grid is open " << std::endl;
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

// settings

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
                                 "layout (location = 0) in vec2 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = projection * view * model * vec4(aPos ,1.0f ,1.0f);\n"
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

const char *geometryShaderSource = "";

static unsigned int CompileShader(unsigned int type, const char *typeShaderSource)
{
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
      std::cout << "ERROR::SHADER::" << type << " COMPILATION_FAILED\n"
                << infoLog << std::endl;
   }
   return shader;
}

static unsigned int CreateShader()
{
   int success;
   char infoLog[512];
   unsigned int shaderProgram = glCreateProgram();
   unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
   unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
   unsigned int geometryShader;
   if (geometryShaderSource != "")
   {
      geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometryShaderSource);
   }
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   if (geometryShaderSource != "")
   {
      glAttachShader(shaderProgram, geometryShader);
   }
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
   if (geometryShaderSource != "")
   {
      glDeleteShader(geometryShader);
   }

   return shaderProgram;
}

void create_maze()
{
   for (int i = 0; i < height * width; i++)
   {
      std::cout << "inside gen maze loop : " << i << std::endl;
      int x = i % width;
      int y = i / width;
      if (grid[i].pathWay[RIGHT] == false)
      {
         vertices[count_vertices++] = (float)x * width;
         vertices[count_vertices++] = (float)y;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
         vertices[count_vertices++] = (float)x * width;
         vertices[count_vertices++] = (float)y * width;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
      }
      if (grid[i].pathWay[LEFT] == false)
      {
         vertices[count_vertices++] = (float)x;
         vertices[count_vertices++] = (float)y;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
         vertices[count_vertices++] = (float)x;
         vertices[count_vertices++] = (float)y * width;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
      }
      if (grid[i].pathWay[UP] == false)
      {
         vertices[count_vertices++] = (float)x;
         vertices[count_vertices++] = (float)y;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
         vertices[count_vertices++] = (float)x * width;
         vertices[count_vertices++] = (float)y;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
      }
      if (grid[i].pathWay[DOWN] == false)
      {
         vertices[count_vertices++] = (float)x;
         vertices[count_vertices++] = (float)y * width;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
         vertices[count_vertices++] = (float)x * width;
         vertices[count_vertices++] = (float)y * width;
         for (int j = 0; j < 3; j++)
         {
            vertices[count_vertices++] = 1.0f;
         }
      }
   }
}

int main()
{
   // std::cout << "width * height : " << width * height << std::endl;
   // std::cout << "true : " << true << std::endl;
   grid = new GridBox[width * height];
   std::cout << "After grid " << std::endl;

   vertices = new float[10000];
   std::cout << "After defining vertices" << std::endl;
   while (stop)
   {
      gen_maze(); // generates maze different mazes each time :)
   }
   std::cout << "After gen_maze" << std::endl;

   for (int i = 0; i < width * height; i++)
   {
      int x = i % width;
      int y = i / width;
      std::cout << "x : " << x << " y : " << y << " :: " << grid[i].isVisited << " : R :" << grid[i].pathWay[RIGHT] << " L : " << grid[i].pathWay[LEFT] << " U :" << grid[i].pathWay[UP] << " D :" << grid[i].pathWay[DOWN] << std::endl;
   }

   // exit(0);

   create_maze();
   std::cout << "After create_maze " << std::endl;
   std::cout << "sizeof(grid)/sizeof(grid[0]) : " << sizeof(grid) / sizeof(grid[0]) << std::endl;

   std::cout << "count_vertices : " << count_vertices << std::endl;

   // for(int i=0;i<count_vertices;i++){
   //    std::cout << i << " : " << vertices[i] << std::endl;
   // }

   GLFWwindow *window = initialise();
   glEnable(GL_DEPTH_TEST);
   unsigned int shaderProgram = CreateShader();

   // set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
   // float vertices[] = {
   //     350.0f,  350.0f, 1.0f, 1.0f, 1.0f,      // 0
   //     350.0f, -350.0f, 1.0f, 1.0f, 1.0f,      // 2
   //    -350.0f, -350.0f, 1.0f, 1.0f, 1.0f,      // 3
   //    -350.0f,  350.0f, 1.0f, 1.0f, 1.0f,      // 1
   //     350.0f,  350.0f, 1.0f, 1.0f, 1.0f,      // 4
   //    //  350.0f,  350.0f, 0.0f, 0.0f, 0.0f,      // 0
   //    //  350.0f, -350.0f, 0.0f, 0.0f, 0.0f,      // 2
   //    // -350.0f,  350.0f, 1.0f, 1.0f, 1.0f,      // 5
   //    //  350.0f, -350.0f, 1.0f, 1.0f, 1.0f,      // 6
   //    // -350.0f, -350.0f, 1.0f, 1.0f, 1.0f,      // 7
   // };

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   // glGenBuffers(1, &EBO);
   // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
   // glBindBuffer(GL_ARRAY_BUFFER, 0);

   // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
   // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
   // glBindVertexArray(0);

   // uncomment this call to draw in wireframe polygons.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // render loop
   // -----------
   while (!glfwWindowShouldClose(window))
   {
      // input
      // -----
      processInput(window);

      // render
      // ------
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // draw our first triangle
      glUseProgram(shaderProgram);
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = glm::mat4(1.0f);
      glm::mat4 projection = glm::mat4(1.0f);
      // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      // projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      projection = glm::ortho(-(float)SCR_WIDTH / 2, (float)SCR_WIDTH / 2, -(float)SCR_HEIGHT / 2, (float)SCR_HEIGHT / 2, 0.1f, 10.0f);
      unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
      unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      glDrawArrays(GL_LINE_STRIP, 0, count_vertices);
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
