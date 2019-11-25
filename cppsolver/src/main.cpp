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

const char CHAR_OUT_OF_BOUNDS = '1';
const char CHAR_CANNOT_ARRIVE = '2';
const char CHAR_CANNOT_LEAVE = '3';

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
        string s;
        s.push_back(c);
        return s;
    }
}

void print_map(vector<string> &map)
{
    for(string &s : map)
    {
        for(char c : s)
        {
            cout << convert_to_visual(c);
        }
        cout << endl;
    }
}

void print_map_as_is(vector<string> &map)
{
    for(string &s : map)
    {
        cout<< s << endl;
    }
}

char whats_here(vector<string> &map, int x, int y)
{
    if(y < 0 || x < 0 || y >= map.size() || x >= map[y].size())
    {
        return CHAR_OUT_OF_BOUNDS;
    }
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

void set_here(vector<string> &map, vector<int> &coords, char to)
{
    int x = coords[0];
    int y = coords[1];
    map[y][x] = to;
}

char when_leaving(char from)
{
    switch (from)
    {
    case CHAR_ROBOT:
    case CHAR_CAN:
        return CHAR_ROAD;
    case CHAR_ROBOT_ON_GOAL:
    case CHAR_CAN_ON_GOAL:
        return CHAR_GOAL;
    
    default:
        return CHAR_CANNOT_LEAVE;
    }
}

char when_arriving(char on, char what)
{
    if(on == CHAR_ROAD)
    {
        if(what == CHAR_ROBOT || what == CHAR_ROBOT_ON_GOAL)
        {
            return CHAR_ROBOT;
        }
        else if(what == CHAR_CAN || what == CHAR_CAN_ON_GOAL)
        {
            return CHAR_CAN;
        }
    }
    else if(on == CHAR_GOAL)
    {
        if(what == CHAR_ROBOT || what == CHAR_ROBOT_ON_GOAL)
        {
            return CHAR_ROBOT_ON_GOAL;
        }
        else if(what == CHAR_CAN || what == CHAR_CAN_ON_GOAL)
        {
            return CHAR_CAN_ON_GOAL;
        }
    }
    return CHAR_CANNOT_ARRIVE;
}

/**
 * returns if applying the step was successful
 */
bool apply_step(vector<string> &map, vector<int> &current_coord, char step)
{
    auto next_coord = get_next_coord(current_coord, step);
    char at_current = whats_here(map, current_coord);
    char at_next = whats_here(map, next_coord);
    // out of bounds
    if(at_next == CHAR_OUT_OF_BOUNDS || at_current == CHAR_OUT_OF_BOUNDS)
    {
        cout << "CHAR OUT OF BOUNDS" << endl;
        return false;
    }

    // road or goal, simple case, go there, set current to what it used to be
    if(at_next == CHAR_ROAD || at_next == CHAR_GOAL)
    {
        char arrive = when_arriving(at_next, at_current);
        char leave = when_leaving(at_current);
        set_here(map, next_coord, arrive);
        set_here(map, current_coord, leave);
    }
    // can't go to wall, return false
    else if(at_next == CHAR_WALL)
    {
        return false;
    }
    else if(at_next == CHAR_CAN || at_next == CHAR_CAN_ON_GOAL)
    {
        // needs to be road or goal behind can
        auto behind_coord = get_next_coord(next_coord, step);
        char behind_can = whats_here(map, behind_coord);
        if(behind_can == CHAR_ROAD || behind_can == CHAR_GOAL)
        {
            char arrive = when_arriving(behind_can, at_next);
            char leave = when_leaving(at_next);
            set_here(map, behind_coord, arrive);
            set_here(map, next_coord, leave);
            at_next = leave;
        }
        else
        {
            return false;
        }
        char arrive = when_arriving(at_next, at_current);
        char leave = when_leaving(at_current);
        set_here(map, next_coord, arrive);
        set_here(map, current_coord, leave);
    }

    current_coord = next_coord;
    return true;
}

void apply_steps(vector<string> &map, string &steps)
{
    // find robot
    auto robot_pos = find_robot(map);

    // iterate thru steps
    for(int i = 0; i < steps.size(); i++)
    {
        char step = steps[i];
        cout << "step"<<i<<":"<<step<<endl;
        apply_step(map, robot_pos, step);
        print_map(map);
    }
}

int check_solved(vector<string> &map)
{
    int n = 0;
    for(string &s : map)
    {
        for(char c : s)
        {
            if(c == CHAR_CAN || c == CHAR_GOAL || c == CHAR_ROBOT_ON_GOAL)
            {
                n++;
            }
        }
    }
    return n;
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
    // get in the frist one
    string commands("dlllluuuurrdruuurululd");
    // get in second one
    commands.append("rdddlllddrulurrdruuurullluld");
    // third
    commands.append("rrrdddlllddddruuulurrdruuuurulll");
    // fourth
    commands.append("rrddddlllddddrrrrulldluuulurrdruuuruulldrurd");
    apply_steps(working_map, commands);
    if(check_solved(working_map))
    {
        cout << "Solution found:" << commands << endl;
    }


    return 0;
}

