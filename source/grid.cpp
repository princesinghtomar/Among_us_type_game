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
int array_fordistincetion[3] = {0, 0, 0};

std::vector<std::tuple<int, int>> chosing_vector;

void set_path(int x, int y)
{
    grid[y][x] = false;
}

void set_wall(int x, int y)
{
    grid[y][x] = true;
}

bool is_wall(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return grid[y][x];
    }
    else
    {
        return false;
    }
}

void create_maze(int x, int y)
{
    set_path(x, y);
    std::vector<std::tuple<int, int>> all_directions;
    all_directions.push_back(std::tuple<int, int>(1, 0));
    all_directions.push_back(std::tuple<int, int>(-1, 0));
    all_directions.push_back(std::tuple<int, int>(0, 1));
    all_directions.push_back(std::tuple<int, int>(0, -1));
    std::random_shuffle(all_directions.begin(), all_directions.end());
    while (all_directions.size() > 0)
    {
        auto direction_to_try = all_directions.back();
        all_directions.pop_back();
        int node_x = x + (std::get<0>(direction_to_try)) * 2;
        int node_y = y + (std::get<1>(direction_to_try)) * 2;

        if (is_wall(node_x, node_y))
        {
            int link_cell_x = x + (std::get<0>(direction_to_try));
            int link_cell_y = y + (std::get<1>(direction_to_try));
            set_path(link_cell_x, link_cell_y);
            create_maze(node_x, node_y);
        }
    }
    return;
}

std::string printing()
{
    std::string string = "";
    std::map<bool, std::string> conv = {
        {true, "█"},
        {false, " "}};
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            string += conv[grid[y][x]];
        }
        string += "\n";
    }
    return string;
}

std::vector<std::tuple<int, int, int, int, int>> crm()
{
    srand(time(0));
    width = int(width / 2) * 2 + 1;
    height = int(height / 2) * 2 + 1;
    grid = new bool *[height];
    for (int y = 0; y < height; y++)
    {
        grid[y] = new bool[width];
        for (int x = 0; x < width; x++)
        {
            grid[y][x] = true;
        }
    }
    create_maze(10, 10);
    std::vector<std::tuple<int, int, int, int, int>> grid2;
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 360, 0, 0, 1));
    grid2.push_back(std::tuple<int, int, int, int, int>(330, 360, 0, 0, 1));
    grid2.push_back(std::tuple<int, int, int, int, int>(330, 290.01, 0, 0, 1));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 360, 0, 0, 1));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 290.01, 0, 0, 1));
    grid2.push_back(std::tuple<int, int, int, int, int>(330, 290.01, 0, 0, 1));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (!grid[y][x])
            {
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + x * x_width, y * y_width - 340, 1, 1, 1));             // (-,+)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + (x + 1) * x_width, y * y_width - 340, 1, 1, 1));       // (+,+)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + (x + 1) * x_width, (y + 1) * y_width - 340, 1, 1, 1)); // (+,-)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + x * x_width, y * y_width - 340, 1, 1, 1));             // (-,+)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + x * x_width, (y + 1) * y_width - 340, 1, 1, 1));       // (-,-)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + (x + 1) * x_width, (y + 1) * y_width - 340, 1, 1, 1)); // (+,-)
            }
            else
            {
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + x * x_width, y * y_width - 340, 0, 0, 0));             // (-,+)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + (x + 1) * x_width, y * y_width - 340, 0, 0, 0));       // (+,+)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + (x + 1) * x_width, (y + 1) * y_width - 340, 0, 0, 0)); // (+,-)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + x * x_width, y * y_width - 340, 0, 0, 0));             // (-,+)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + x * x_width, (y + 1) * y_width - 340, 0, 0, 0));       // (-,-)
                grid2.push_back(std::tuple<int, int, int, int, int>(-300 + (x + 1) * x_width, (y + 1) * y_width - 340, 0, 0, 0)); // (+,-)
            }
        }
    }
    grid2.push_back(grid2.back());
    // player among us art :
    //helmet part :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 14.0, 260 + 23.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 14.0, 260 + 19.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 22.0, 260 + 19.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 14.0, 260 + 23.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 22.0, 260 + 23.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 22.0, 260 + 19.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13.0, 260 + 22.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13.0, 260 + 20.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 23.0, 260 + 20.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13.0, 260 + 22.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 23.0, 260 + 22.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 23.0, 260 + 20.0, 0.0, 0.0, 1.0));

    // main body part :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 26.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 26.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 26.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 11.0, 260 + 17.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 11.0, 260 + 28.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 17.0, 260 + 28.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 11.0, 260 + 17.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 17.0, 260 + 17.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 17.0, 260 + 28.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9.0, 260 + 27.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9.0, 260 + 17.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19.0, 260 + 17.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9.0, 260 + 27.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19.0, 260 + 27.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19.0, 260 + 17.0, 0.0, 1.0, 0.0));

    // legs :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 4.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 12.0, 260 + 4.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 12.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 12.0, 260 + 4.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 16.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 16.0, 260 + 4.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 4.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 16.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 10.0, 0.0, 1.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 4.0, 0.0, 1.0, 0.0));

    // bagpack :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 23.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 23.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 23.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5.0, 260 + 22.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5.0, 260 + 11.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 11.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5.0, 260 + 22.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 22.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 11.0, 1.0, 0.0, 0.0));

    // imposter among us part :
    //helmet part :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 14.0, 260 + 23.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 14.0, 260 + 19.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 22.0, 260 + 19.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 14.0, 260 + 23.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 22.0, 260 + 23.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 22.0, 260 + 19.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13.0, 260 + 22.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13.0, 260 + 20.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 23.0, 260 + 20.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13.0, 260 + 22.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 23.0, 260 + 22.0, 0.0, 0.0, 1.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 23.0, 260 + 20.0, 0.0, 0.0, 1.0));

    // main body part :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 26.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 26.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 26.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 11.0, 260 + 17.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 11.0, 260 + 28.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 17.0, 260 + 28.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 11.0, 260 + 17.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 17.0, 260 + 17.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 17.0, 260 + 28.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9.0, 260 + 27.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9.0, 260 + 17.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19.0, 260 + 17.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9.0, 260 + 27.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19.0, 260 + 27.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19.0, 260 + 17.0, 1.0, 0.0, 0.0));

    // legs :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 4.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 12.0, 260 + 4.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 12.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 12.0, 260 + 4.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 16.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 16.0, 260 + 4.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 4.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 16.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 10.0, 1.0, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20.0, 260 + 4.0, 1.0, 0.0, 0.0));

    // bagpack :
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 23.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 10.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 23.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 23.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 8.0, 260 + 10.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5.0, 260 + 22.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5.0, 260 + 11.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 11.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5.0, 260 + 22.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 22.0, 0.5, 0.0, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 6.0, 260 + 11.0, 0.5, 0.0, 0.0));

    // some texts :-)
    // 0
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // 1)
    grid2.push_back(std::tuple<int, int, int, int, int>(15.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(15.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // - mid
    // 2)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // 3)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // 4)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    // 5)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // 6)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    // 7)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    // 8)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // 9)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    // T)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(15.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // | mid
    grid2.push_back(std::tuple<int, int, int, int, int>(15.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // | mid
    // L)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    // S)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // P)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    // E)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //  ---
    // F)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  ---
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 15.0f, 1.0f, 1.0f, 1.0f)); // - mid
    // N)
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 25.0f, 1.0f, 1.0f, 1.0f)); // |
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //     |
    // '/')
    grid2.push_back(std::tuple<int, int, int, int, int>(05.0f, 05.0f, 1.0f, 1.0f, 1.0f)); //   /
    grid2.push_back(std::tuple<int, int, int, int, int>(25.0f, 25.0f, 1.0f, 1.0f, 1.0f)); //  /

    // task cells
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 290, 0, 1.0f, 1.0)); // (-,+)
    grid2.push_back(std::tuple<int, int, int, int, int>(-270, 290, 0, 1.0f, 1.0)); // (+,+)
    grid2.push_back(std::tuple<int, int, int, int, int>(-270, 260, 0, 1.0f, 1.0)); // (+,-)
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 290, 0, 1.0f, 1.0)); // (-,+)
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 260, 0, 1.0f, 1.0)); // (-,-)
    grid2.push_back(std::tuple<int, int, int, int, int>(-270, 260, 0, 1.0f, 1.0)); // (+,-)
    // Exit
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 290, 0, 0.0f, 1.0)); // (-,+)
    grid2.push_back(std::tuple<int, int, int, int, int>(-270, 290, 0, 0.0f, 1.0)); // (+,+)
    grid2.push_back(std::tuple<int, int, int, int, int>(-270, 260, 0, 0.0f, 1.0)); // (+,-)
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 290, 0, 0.0f, 1.0)); // (-,+)
    grid2.push_back(std::tuple<int, int, int, int, int>(-300, 260, 0, 0.0f, 1.0)); // (-,-)
    grid2.push_back(std::tuple<int, int, int, int, int>(-270, 260, 0, 0.0f, 1.0)); // (+,-)

    float pi = 4 * acos(0.0) + 0.2;
    float radius = 10;
    float i = 0.0;
    int j = 1;
    while (i < pi)
    {
        if (j % 3 == 0)
        {
            grid2.push_back(std::tuple<int, int, int, int, int>(-285, 275, 1.0f, 0.0f, 1.0)); // (+,-)
        }
        grid2.push_back(std::tuple<int, int, int, int, int>(-285 + radius * cos(i), 275 + radius * sin(i), 1.0f, 0.0f, 1.0)); // (+,-)
        i += 0.2;
        j += 1;
    }

    // some power up - 6
    grid2.push_back(std::tuple<int, int, int, int, int>(-295, 285, 1.0f, 1.0f, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-275, 285, 1.0f, 1.0f, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-275, 265, 1.0f, 1.0f, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-295, 285, 1.0f, 1.0f, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-295, 265, 1.0f, 1.0f, 0.0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-275, 265, 1.0f, 1.0f, 0.0));
    // some other power up // 3*3
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 25, 260 + 0, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20, 260 + 30, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 30, 260 + 30, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 5, 260, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 10, 260 + 30, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 0, 260 + 30, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 15, 260 + 0, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 10, 260 + 30, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 20, 260 + 30, 1, 0, 0));

    // obstruction-  24260 +
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13, 260 + 30 - 2, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13, 260 + 30 - 29, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 15, 260 + 30 - 29, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 13, 260 + 30 - 2, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 15, 260 + 30 - 2, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 15, 260 + 30 - 29, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 7, 260 + 30 - 16, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 21, 260 + 30 - 16, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 21, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 7, 260 + 30 - 16, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 7, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 21, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 7, 260 + 30 - 7, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 7, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 7, 260 + 30 - 7, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9, 260 + 30 - 7, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 9, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19, 260 + 30 - 7, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 21, 260 + 30 - 7, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 21, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19, 260 + 30 - 7, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 19, 260 + 30 - 18, 1, 0, 0));
    grid2.push_back(std::tuple<int, int, int, int, int>(-300 + 21, 260 + 30 - 18, 1, 0, 0));

    return grid2;
}

void abd()
{
    // for(int y=0;y<height;y++){
    //     for(int x=0;x<width;x++){
    //         std::cout <<grid[y][x];
    //     }
    //     std::cout << "\n";
    // }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (grid[y][x] == false)
            {
                chosing_vector.push_back(std::tuple<int, int>(x, 20 - y));
            }
        }
    }
    std::random_shuffle(chosing_vector.begin(), chosing_vector.end());
}

float *rets()
{
    std::vector<std::tuple<int, int, int, int, int>> grid2 = crm();
    float *vertices = new float[1000000];
    for (int i = 0; i < 1000000; i++)
    {
        vertices[i] = 10000;
    }
    int j = 0;
    int p = 0;
    for (auto k = grid2.begin(); k != grid2.end(); k++)
    {
        vertices[j++] = float(std::get<0>(*k));
        vertices[j++] = float(std::get<1>(*k));
        vertices[j++] = float(std::get<2>(*k));
        vertices[j++] = float(std::get<3>(*k));
        vertices[j++] = float(std::get<4>(*k));
        p++;
    }
    int i = 0;
    while (vertices[i++] != 10000)
        ;
    i--;
    abd();
    // for(auto i = chosing_vector.begin();i!=chosing_vector.end();i++){
    //     std::cout << std::get<0>(*i) << " " << std::get<0>(*i) << std::endl;
    // }
    return vertices;
}