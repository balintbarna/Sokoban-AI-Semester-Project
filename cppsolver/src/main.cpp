#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const char CHAR_WALL = 'X';
const char CHAR_GOAL = 'G';
const char CHAR_CAN = 'J';
const char CHAR_CAN_ON_GOAL = 'j';
const char CHAR_ROBOT = 'M';
const char CHAR_ROBOT_ON_GOAL = 'm';
const char CHAR_ROAD = '.';

string S_WALL("█");
string S_GOAL("◌");
string S_CAN("■");
string S_CAN_ON_GOAL("◙");
string S_ROBOT("☺");
string S_ROBOT_ON_GOAL("☻");
string S_ROAD(" ");


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

string convert_to_visual(char c)
{
    switch (c)
    {
    case CHAR_WALL:
        return S_WALL;
    case CHAR_CAN:
        return S_CAN;
    case CHAR_CAN_ON_GOAL:
        return S_CAN_ON_GOAL;
    case CHAR_GOAL:
        return S_GOAL;
    case CHAR_ROAD:
        return S_ROAD;
    case CHAR_ROBOT:
        return S_ROBOT;
    case CHAR_ROBOT_ON_GOAL:
        return S_ROBOT_ON_GOAL;
    
    default:
        return string("?");
    }
}

void print_map(vector<string> map)
{
    for(string s : map)
    {
        for(char c : s)
        {
            cout << convert_to_visual(c);
        }
        cout << endl;
    }
}

void print_map_as_is(vector<string> map)
{
    for(string s : map)
    {
        cout<< s << endl;
    }
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
    // cout<<"starting find object"<<endl;
    int x, y;
    for(y = 0; y < map.size(); y++)
    {
        // cout<<"y:"<<y<<endl;
        string &line = map[y];
        // cout<<"linesize"<<line.size()<<endl;
        for(x = 0; x < line.size(); x++)
        {
            char k = line[x];
            // cout<<"coord["<<x<<","<<y<<"]"<<endl;
            // cout<<"char:"<<k<<endl;

            for(char c : objects)
            {
                if(k == c)
                {
                    // jump to return point
                    goto STEP_OUT;
                }
            }
        }
    }
    // loop completed without finding anything
    x = -1;
    y = -1;
    // if loop finds something, jump here
    STEP_OUT:
    return vector<int>{x,y};
}

vector<int> find_robot(vector<string> &map)
{
    // cout<<"starting find robot"<<endl;
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
    cout << "original map as is" << endl;
    print_map_as_is(starting_map);
    cout << "original map" << endl;
    print_map(starting_map);

    // copy working map
    auto working_map(starting_map);
    cout << "working map" << endl;
    print_map(working_map);

    // experiment
    string commands("lll");
    apply_steps(working_map, commands);

    cout << "map after steps" << endl;
    print_map(working_map);

    return 0;
}

