#include <bits/stdc++.h>
int height = 20;
int width = 20;
bool **grid;
int x_width = 30;
int y_width = 30;
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
    // int all_directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
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

int main(){
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
    std::tuple<int,int> **grid1;
    grid1 = new std::tuple<int,int>*[height];
    for(int y=0;y<height;y++){
        grid1[y] = new std::tuple<int,int>[width];
        for(int x=0;x<width;x++){
            grid1[y][x] = std::tuple<int,int>(y*30+5,-300 + x*30);
        }
    }
    create_maze(10,10);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            std::cout << grid[y][x];
        }
        std::cout << "\n";
    }
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            std::cout << "("<< std::get<0>(grid1[y][x])<<","<<std::get<1>(grid1[y][x])<<"),\t";
        }
        std::cout << "\n";
    }
    int j=0;
    std::vector<std::tuple<int,int,int,int,int>> grid2;
    // insert box co-ordinates :) you can use push front is push back not used :)
    grid2.push_back(std::tuple<int,int,int,int,int>(-300,360,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>( 330,360,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>( 330,290.01,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>(-300,360,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>(-300,290.01,0,0,1));
    grid2.push_back(std::tuple<int,int,int,int,int>( 330,290.01,0,0,1));
    j+=6;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            // std::cout<<"timerun : "<<grid[y][x]<< " " <<std::endl;
            if(!grid[y][x]){
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,1,1,1));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,y*y_width-340,1,1,1));  // (+,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,1,1,1));  // (+,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,1,1,1));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,(y+1)*y_width-340,1,1,1));  // (-,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,1,1,1));  // (+,-)
                j+=6;
            }
            else{
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,0,0,0));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,y*y_width-340,0,0,0));  // (+,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,0,0,0));  // (+,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,y*y_width-340,0,0,0));  // (-,+)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + x*x_width,(y+1)*y_width-340,0,0,0));  // (-,-)
                grid2.push_back(std::tuple<int,int,int,int,int>(-300 + (x+1)*x_width,(y+1)*y_width-340,0,0,0));  // (+,-)
                j+=6;
            }
        }
    }
    std::cout << "Here are vertices : \n";
    int lamda = 0;
    for(auto k = grid2.begin();k!=grid2.end();k++){
        // if(lamda%6==0){
        //     lamda = 0;
        //     std::cout <<"\n"; 
        // }
        std::cout <<std::get<0>(*k)<<".0,"<<std::get<1>(*k)<<".0,"<<std::get<2>(*k)<<".0,"<<std::get<3>(*k)<<".0,"<<std::get<4>(*k)<<".0,\n";
        j++;
        // lamda++;
    }
    std::cout <<"\n\n"<< grid2.size() << " : " << j << std::endl;
    std::cout <<"\n\n";
    std::cout << printing();
}