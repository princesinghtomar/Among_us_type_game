#include "grid.cpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int life = 100;
bool light = false;
int total_tasks = 2;
bool t1 = false;
bool t2 = false;
int score= 0;
int completed_tasks = 0;
float timer = 1000;
float lastFrame = glfwGetTime();

const int max_length = 18;
int text_parts[max_length][2] = {
    {2761 + 0,8}, {2761 + 8,2},  {2761 + 10,10},{2761 + 20,8},{2761 + 28,6},{2761 + 34,10},{2761 + 44,8},
    {2761 + 52,4},{2761 + 56,10},{2761 + 66,8}, {2761 + 74,4},{2761 + 78,4},{2761 + 82,10},{2761 + 92,8},
    {2761 + 100,8},{2761 + 108,6},{2761 + 114,6},{2761 + 120,2}
};

char text_arr[max_length] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'L', 'S', 'P','E','F','N','/'};

void print_text(unsigned int shaderProgram,char c,int x,int y){
	int i = 0;
   int lenth = sizeof(text_arr) / sizeof(text_arr[0]);
   while (text_arr[i] != c)
   {
     if (i > max_length)
     {
        break;
     }
     i++;
   }
  	if(i < max_length ){
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model,glm::vec3(x,y,0.01f));
      unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_LINES, text_parts[i][0], text_parts[i][1]);
   }
}
void create_print_text(unsigned int shaderProgram){
   int x = -550;
   int y = +200;
   std::string temp_light = light?"0N":"0FF";
   std::string st[] = {"TL " + std::to_string(int(timer)), "LF "+std::to_string(life),"TS "+
   std::to_string(completed_tasks)+"/"+std::to_string(total_tasks),"LT " + temp_light}; 
   // std::cout << st[3] << std::endl;
   // exit(0);
   for(int i=0 ;i < sizeof(st)/sizeof(st[0]);i++){
      x=-550;
      for(int j=0;j<st[i].length();j++){
         if(st[i][j]== ' '){
            // std::cout << " ";
         }
         else{
            // std::cout<<st[i][j];
            print_text(shaderProgram,st[i][j],x,y);
         }
         x+=30;
      }
      // std::cout << "\n";
      y-=30;
   }
}

GLint othermodeldata[7][2] = {
   {0,2653},
   {2653,54}, // player
   {2707,54}, // imposter
   {2883,6},  // task 1
   {2883,6},  // task 2
   {2889,6},   // Exit
   // {2895,44} // circle button
};

glm::vec3 modelpositions[] = {
   glm::vec3(0.0f, 0.0f,0.0f),     // maze center location
   glm::vec3(0.0f,0.0f,0.1f), // player starting position also can be used to calculate index of the box currently present
   glm::vec3(600.0f,-600.0f,0.2f),    // imposter starting position to be decided later based on last open box
   glm::vec3(600.0f,0.0f,0.001f),
   glm::vec3(0.0f,-600.0f,0.001f),
   glm::vec3(600.0f,-600.0f,0.001f),
   // glm::vec3(0.0f,0.0f,0.4f),
};

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
      std::cout << "ERROR::SHADER::"<< type <<" COMPILATION_FAILED\n"
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
   unsigned int geometryShader;
   if(geometryShaderSource != ""){
      geometryShader =  CompileShader(GL_GEOMETRY_SHADER,geometryShaderSource);
   }
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   if(geometryShaderSource != ""){
      glAttachShader(shaderProgram,geometryShader);
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
   if(geometryShaderSource!=""){
      glDeleteShader(geometryShader);
   }

   return shaderProgram;
}

void imposter_start(){
   int val = int(rand()%3);
   if(!val){
      modelpositions[2] = glm::vec3(600.0f,0.0f,0.2f);
   } else{
      if(val == 1){
         modelpositions[2] = glm::vec3(0.0f,-600.0f,0.2f);
      }
      else{
         modelpositions[2] = glm::vec3(600.0f,-600.0f,0.2f);
      }
   }
}

GLint power_desc[1][2] = {
   {2895,44} // circle button
};

glm::vec3 power_positions[1]={
   glm::vec3(0.0f,0.0f,0.1f)
};

bool usedpower[] = {
   false
};

void check_player(){
   if(modelpositions[1].x == 600 && modelpositions[1].y == 0){
      if(!t1){
         t1 = true;
         if(completed_tasks < total_tasks)
            completed_tasks+=1;
      }
   }
   if(modelpositions[1].x == 0 && modelpositions[1].y <=-600){
      if(!t2){
         t2 = true;
         if(completed_tasks < total_tasks)
            completed_tasks+=1;
      }
   }
   if(modelpositions[1].x == 600 && modelpositions[1].y <= -600){
      if(completed_tasks >= total_tasks){
         // win
         exit(0);
      }
   }
   for(int i=0;i<sizeof(power_positions)/sizeof(power_positions[0]);i++){
      if(modelpositions[1].x == power_positions[i].x && modelpositions[1].y == power_positions[i].y){
         usedpower[i] = true;
      }
   }

}

void choose(){
   int j=0;
   for(auto i=chosing_vector.begin();i!=chosing_vector.end();i++){
      if(j<sizeof(power_positions)/sizeof(power_positions[0])){
         power_positions[j].x = 30*(std::get<0>(*i));
         power_positions[j].y = -30*(std::get<1>(*i));
      }
      else{
         break;
      }
   }
}


void power(unsigned int shaderProgram){
   for(int k=0;k<sizeof(power_positions)/sizeof(power_positions[0]);k++){
      if(!usedpower[k]){
         glm::mat4 model = glm::mat4(1.0f);
         // if(k==1){
         model = glm::translate(model,power_positions[k]);
         // }
         // else if(k == 2){
            // model = glm::translate(model,glm::vec3(modelpositions[k],0.02f));
         // }
         // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
         unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
         glDrawArrays(GL_TRIANGLES,power_desc[k][0],power_desc[k][1]);
      }
   }
}

int main()
{
   srand(time(0));
   float *vertices = rets();
   imposter_start();
   GLFWwindow *window = initialise();
   glEnable(GL_DEPTH_TEST);  
   unsigned int shaderProgram = CreateShader();
   int vsize = 0;
   while(vertices[vsize++]!=10000);
   vsize--;
   int length_needed = vsize;
   vsize /= 5;

   // std::cout<<"hello " << vertices[2653*5+1] <<std::endl;
   
   std::cout << " >> " <<sizeof(modelpositions)/sizeof(modelpositions[0]) << std::endl;

   // exit(0);

   std::cout << sizeof(&vertices) << " : " <<vsize<< std::endl;

   unsigned int VBO, VAO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   // glGenBuffers(1, &EBO);
   // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, length_needed*sizeof(float), vertices, GL_STATIC_DRAW);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

   // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
   // glBindBuffer(GL_ARRAY_BUFFER, 0);

   // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
   // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
   // glBindVertexArray(0);

   // uncomment this call to draw in wireframe polygons.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // std::cout << modelpositions[1].x << std::endl;
   // render loop
   // -----------
   choose();
   while (!glfwWindowShouldClose(window))
   {
      // Frames :
      // Currently not much needed 
      float currentFrame = glfwGetTime();
      float deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      timer -= deltaTime;
      if(int(timer)%20==0){
         score --;
      }
      if(timer <= 0){
         // times up
         exit(0);
      }
      // input
      // -----
      processInput(window);

      // render
      // ------
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // draw our first triangle
      glUseProgram(shaderProgram);
      glm::mat4 view = glm::mat4(1.0f);
      glm::mat4 projection = glm::mat4(1.0f);
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      // projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      projection = glm::ortho(-(float)SCR_WIDTH/2,(float)SCR_WIDTH/2,-(float)SCR_HEIGHT/2,(float)SCR_HEIGHT/2, 0.1f, 10.0f);
      unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
      
      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      create_print_text(shaderProgram);
      power(shaderProgram);
      for(int k=0;k<sizeof(modelpositions)/sizeof(modelpositions[0]);k++){
         if(!(usedpower[0] && k==2)){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,modelpositions[k]);
            unsigned int modelLoc = glGetUniformLocation(shaderProgram,"model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,othermodeldata[k][0],othermodeldata[k][1]);
         }
      }
      // glDrawArrays(GL_TRIANGLES,0,2652);
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
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
      int index_x = abs(modelpositions[1].x/x_width) ;
      int index_y = height - abs(modelpositions[1].y/y_width) - 1 + 1;
      if(index_y >= 0 && index_y < height){
         if(!grid[index_y][index_x]){
            modelpositions[1].y += y_width;
         }
      }
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
      int index_x = abs(modelpositions[1].x/x_width);
      int index_y = height - abs(modelpositions[1].y/y_width) - 1 -1;
      if(index_y<height && index_y >= 0){
         if(!grid[index_y][index_x]){
            modelpositions[1].y -= y_width;
         }
      }
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
      int index_x = abs(modelpositions[1].x/x_width) -1;
      int index_y = height - abs(modelpositions[1].y/y_width)-1;
      if(index_x >= 0 && index_x < width){
         if(!grid[index_y][index_x]){
            modelpositions[1].x -= x_width;
         }
      }
   }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
      int index_x = abs(modelpositions[1].x/x_width) +1;
      int index_y = height - abs(modelpositions[1].y/y_width) - 1;
      if(index_x >= 0 && index_x < width){
         if(!grid[index_y][index_x]){
            modelpositions[1].x += x_width;
         }
      }
   }
   // std::cout<< "x : " << modelpositions[1].x << " |  y : " << modelpositions[1].y << std::endl;
   check_player();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
   // make sure the viewport matches the new window dimensions; note that width and
   // height will be significantly larger than specified on retina displays.
   glViewport(0, 0, width, height);
}
