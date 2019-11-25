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


vector<string> read_map(string map_path)
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

vector<string> apply_steps(vector<string> &map, string &steps)
{
    vector<string> nu(map);
    // find robot
    int x = -1, y = -1;
    for(; y < nu.size(); y++)
    {
        string line = nu[y];
        for(; x < line.size(); x++)
        {
            if(line[x] == CHAR_ROBOT || line[x] == CHAR_ROBOT_ON_GOAL)
            {
                break;
            }
        }
    }
    if (x < 0 || y < 0)
    {
        cout << "robot pos error" << endl;
        return nu;
    }

    cout << "Robot pos: (" << x << "; " << y << ")" << endl;

    for(int i = 0; i < steps.size(); i++)
    {
        char step = steps[i];
        int tx = -1, ty = -1;
        if (step == 'L')
        {
            tx = x-1;
            ty = y;
        }
        if (step == 'U')
        {
            tx = x;
            ty = y-1;
        }
        if (step == 'R')
        {
            tx = x+1;
            ty = y;
        }
        if (step == 'D')
        {
            tx = x;
            ty = y+1;
        }
        char target = nu[ty][tx];
        
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
    if (argc != 2)
    {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " MAP_PATH" << std::endl;
        return 1;
    }
    string map_path(argv[1]);

    auto map = read_map(map_path);

    cout << "original map" << endl;
    print_map(map);

    string commands("LLL");
    auto nu = apply_steps(map, commands);

    cout << "map after steps" << endl;
    print_map(nu);

    return 0;
}

