#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bits/stdc++.h>

int height = 20;
int width = 20;
bool **grid;
int x_width = 30;
int y_width = 30;
int array_fordistincetion[3] = {0,0,0};
void set_path(int x, int y){
    grid[y][x] = false;
}

void set_wall(int x, int y){
    grid[y][x] = true;
}

bool is_wall(int x,int y){
    if(x>=0 && x < width && y >= 0 && y < height){
        return grid[y][x];
    }
    else{
        return false;
    }
}

void create_maze(int x,int y){
    set_path(x,y);
    std::vector<std::tuple<int,int>> all_directions;
    all_directions.push_back(std::tuple<int,int>( 1 , 0));
    all_directions.push_back(std::tuple<int,int>(-1 , 0));
    all_directions.push_back(std::tuple<int,int>( 0 , 1));
    all_directions.push_back(std::tuple<int,int>( 0 ,-1));
    std::random_shuffle(all_directions.begin(),all_directions.end());
    while(all_directions.size() > 0){
        auto direction_to_try = all_directions.back();
        all_directions.pop_back();
        int node_x = x + (std::get<0>(direction_to_try))*2;
        int node_y = y + (std::get<1>(direction_to_try))*2;

        if(is_wall(node_x,node_y)){
            int link_cell_x = x + (std::get<0>(direction_to_try));
            int link_cell_y = y + (std::get<1>(direction_to_try));
            set_path(link_cell_x,link_cell_y);
            create_maze(node_x,node_y);
        }
    }
    return;
}

std::string printing(){
    std::string string = "";
    std::map<bool,std::string> conv = {
        {true,"█"},
        {false," "}
        };
    for (int y=0;y<height;y++){
        for (int x=0;x<width;x++){
            string += conv[grid[y][x]];
        }
        string += "\n";
    }
    return string;
}

std::vector<std::tuple<int,int,int,int,int>> crm(){
    srand(time(0));
    width = int(width/2)*2 + 1;
    height = int(height/2)*2 + 1;
    grid = new bool*[height];
    for(int y=0;y<height;y++){
        grid[y] = new bool[width];
        for(int x=0;x<width;x++){
            grid[y][x] = true;
        }
    }
    create_maze(10,10);
    std::vector<std::tuple<int,int,int,int,int>> grid2;
    grid2.push_back(std::tuple<int,int,int,int,int>(-300,360,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>( 330,360,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>( 330,290.01,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>(-300,360,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>(-300,290.01,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>( 330,290.01,0,0,1));
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            if(!grid[y][x]){
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,1,1,1));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,y*y_width-340,1,1,1));  // (+,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,1,1,1));  // (+,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,1,1,1));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,(y+1)*y_width-340,1,1,1));  // (-,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,1,1,1));  // (+,-)
            }
            else{
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,0,0,0));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,y*y_width-340,0,0,0));  // (+,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,0,0,0));  // (+,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,0,0,0));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,(y+1)*y_width-340,0,0,0));  // (-,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,0,0,0));  // (+,-)
            }
        }
    }
    grid2.push_back(grid2.back());
    grid2.push_back(std::tuple<int,int,int,int,int>(-265.0,-315.0,1.0,0.0,1.0));
    grid2.push_back(std::tuple<int,int,int,int,int>(-295.0,-345.0,1.0,0.0,1.0));
    grid2.push_back(std::tuple<int,int,int,int,int>(-265.0,-345.0,1.0,0.0,1.0));
    grid2.push_back(std::tuple<int,int,int,int,int>(-295.0,-315.0,1.0,0.0,1.0));
    grid2.push_back(std::tuple<int,int,int,int,int>(-295.0,-345.0,1.0,0.0,1.0));
    grid2.push_back(std::tuple<int,int,int,int,int>(-265.0,-345.0,1.0,0.0,1.0));
    return grid2;
}

float* rets(){
    std::vector<std::tuple<int,int,int,int,int>> grid2 = crm();
    float *vertices = new float[1000000];
    for(int i=0 ;i<1000000;i++){
        vertices[i] = 10000;
    }
    int j=0;
    int p=0;
    for(auto k = grid2.begin();k!=grid2.end();k++){
        vertices[j++] = float(std::get<0>(*k));
        vertices[j++] = float(std::get<1>(*k));
        vertices[j++] = float(std::get<2>(*k));
        vertices[j++] = float(std::get<3>(*k));
        vertices[j++] = float(std::get<4>(*k));
        p++;
    }
    int i=0;
    while(vertices[i++]!=10000);
    i--;
    return vertices;
}