#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

char CHAR_WALL = '█';
char CHAR_GOAL = '◌';
char CHAR_CAN = '■';
char CHAR_CAN_ON_GOAL = '◙';
char CHAR_ROBOT = '☺';
char CHAR_ROBOT_ON_GOAL = '☻';
char CHAR_ROAD = ' ';


vector<string> read_map(string &map_path)
{
    vector<string> map;
    fstream f(map_path, fstream::in);
    string line;
    for(;;)
    {
        line.clear();
        getline(f, line, '\n');
        if(line.empty()) break;
        map.push_back(line);
    }

    f.close();
    return map;
}

char whats_here(vector<string> &map, int x, int y)
{
    return map[y][x];
}

char whats_here(vector<string> &map, vector<int> &coords)
{
    return whats_here(map, coords[0], coords[1]);
}

vector<int> find_object_in(vector<string> &map, string &objects)
{
    int x = -1, y = -1;
    bool searching = true;
    for(; y < map.size() && searching; y++)
    {
        string &line = map[y];
        for(; x < line.size() && searching; x++)
        {
            for(char c : objects)
            {
                if(line[x] == c)
                {
                    searching = false;
                    break;
                }
            }
        }
    }
    return vector<int>{x,y};
}

vector<int> find_robot(vector<string> &map)
{
    string objects{CHAR_ROBOT, CHAR_ROBOT_ON_GOAL};
    auto coords = find_object_in(map, objects);
    int x = coords[0];
    int y = coords[1];
    if (x < 0 || y < 0)
    {
        cout << "robot pos error" << endl;
    }

    cout << "Robot pos: (" << x << "; " << y << ")" << endl;
    return coords;
}

vector<int> get_next_coord(vector<int> &current_coord, char step)
{
    step = tolower(step);
    int x = current_coord[0];
    int y = current_coord[1];
    int tx = -1;
    int ty = -1;
    if (step == 'l')
    {
        tx = x-1;
        ty = y;
    }
    if (step == 'u')
    {
        tx = x;
        ty = y-1;
    }
    if (step == 'r')
    {
        tx = x+1;
        ty = y;
    }
    if (step == 'd')
    {
        tx = x;
        ty = y+1;
    }
    return vector<int>{tx, ty};
}

void apply_step(vector<string> &map, vector<int> &current_coord, char step)
{
    auto next_coord = get_next_coord(current_coord, step);
    char at_next = whats_here(map, next_coord);
}

void apply_steps(vector<string> &map, string &steps)
{
    // find robot
    auto robot_pos = find_robot(map);

    // iterate thru steps
    for(int i = 0; i < steps.size(); i++)
    {
        char step = steps[i];
        apply_step(map, robot_pos, step);
    }
}

void print_map(vector<string> map)
{
    for(string s : map)
    {
        cout << s << endl;
    }
}

int main(int argc, char* argv[])
{
    // get map path
    if (argc != 2)
    {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " MAP_PATH" << std::endl;
        return 1;
    }
    string map_path(argv[1]);

    // read map
    auto starting_map = read_map(map_path);

    // print map
    cout << "original map" << endl;
    print_map(starting_map);

    // copy working map
    auto working_map(starting_map);

    // experiment
    string commands("lll");
    apply_steps(working_map, commands);

    cout << "map after steps" << endl;
    print_map(working_map);

    return 0;
}

