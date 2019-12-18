#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <math.h>
using namespace std;

typedef vector<string> SMap;

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

struct State {
    SMap map;
    string steps;
};

SMap read_map(string &map_path)
{
    SMap map;
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

void print_map(SMap &map)
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

void print_map_as_is(SMap &map)
{
    for(string &s : map)
    {
        cout<< s << endl;
    }
}

char whats_here(SMap &map, int x, int y)
{
    if(y < 0 || x < 0 || y >= map.size() || x >= map[y].size())
    {
        return CHAR_OUT_OF_BOUNDS;
    }
    return map[y][x];
}

char whats_here(SMap &map, vector<int> &coords)
{
    return whats_here(map, coords[0], coords[1]);
}

vector<int> find_object_in(SMap &map, string &objects)
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

vector<int> find_robot(SMap &map)
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

    // cout << "Robot pos: (" << x << "; " << y << ")" << endl;
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
    vector<int> ret{tx, ty};
    return ret;
}

void set_here(SMap &map, vector<int> &coords, char to)
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
bool apply_step(SMap &map, vector<int> &current_coord, char &step)
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
        // show on step if pushing can
        step = toupper(step);
    }

    current_coord = next_coord;
    return true;
}

/**
 * returns if steps were successfully applied
 */
bool apply_steps(SMap &map, string &steps)
{
    // find robot
    auto robot_pos = find_robot(map);

    // iterate thru steps
    for(int i = 0; i < steps.size(); i++)
    {
        // cout<<"step"<<i<<":"<<steps[i]<<endl;
        bool success = apply_step(map, robot_pos, steps[i]);
        if(success == false) return false;
    }

    return true;
}

int check_solved(SMap &map)
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

void do_breadth_first_search(SMap &original_map)
{
    auto working_map(original_map);
    // create open list
    deque<string> open_list;
    vector<SMap> closed_list;
    open_list.push_back(string(""));
    // iterate until there's nothing left
    int steps_len = 0;
    while(open_list.empty() == false)
    {
        auto steps = open_list.front();
        open_list.pop_front();
        int nu_size = steps.size();
        bool print = nu_size != steps_len;
        if(nu_size != steps_len)
        {
            cout << "Steps length: " << nu_size << endl;
            steps_len = nu_size;
        }
        working_map = original_map;
        bool did_apply = apply_steps(working_map, steps);
        if(did_apply == false) continue;
        if(print)
        {
            cout<<"Steps:"<<steps<<endl;
            cout<<"Map:"<<endl;
            print_map(working_map);
        }

        if(check_solved(working_map) == 0)
        {
            cout << "Solution found:" << steps << endl;
            break;
        }

        bool bused = false;
        for(auto used : closed_list)
        {
            if(used == working_map)
            {
                bused = true;
                break;
            }
        }
        if(bused) continue;

        closed_list.push_back(SMap(working_map));

        string up(steps);
        up.push_back('u');
        string down(steps);
        down.push_back('d');
        string left(steps);
        left.push_back('l');
        string right(steps);
        right.push_back('r');
        open_list.push_back(up);
        open_list.push_back(down);
        open_list.push_back(left);
        open_list.push_back(right);
    }
}

void do_depth_first_search(SMap &original_map, int max_depth)
{
    auto working_map(original_map);
    // create open list
    deque<string> open_list;
    vector<SMap> closed_list;
    open_list.push_back(string(""));
    // iterate until there's nothing left
    int best = 999;
    while(open_list.empty() == false)
    {
        auto steps = open_list.back();
        open_list.pop_back();
        int nu_size = steps.size();
        if(nu_size > max_depth)
        {
            continue;
        }
        working_map = original_map;
        bool did_apply = apply_steps(working_map, steps);
        if(did_apply == false) continue;

        int slvd = check_solved(working_map);
        bool print = slvd < best;
        if(print)
        {
            best = slvd;
            cout<<"Steps:"<<steps<<endl;
            print_map(working_map);
        }
        if(slvd == 0)
        {
            cout << "Solution found:" << steps << endl;
            return;
        }

        // bool bused = false;
        // for(auto used : closed_list)
        // {
        //     if(used == working_map)
        //     {
        //         bused = true;
        //         break;
        //     }
        // }
        // if(bused) continue;

        // closed_list.push_back(SMap(working_map));

        string up(steps);
        up.push_back('u');
        string down(steps);
        down.push_back('d');
        string left(steps);
        left.push_back('l');
        string right(steps);
        right.push_back('r');
        open_list.push_back(up);
        open_list.push_back(down);
        open_list.push_back(left);
        open_list.push_back(right);
    }
}

int get_dist(vector<int> coord1, vector<int> coord2)
{
    int x1 = coord1[0];
    int x2 = coord2[0];
    int y1 = coord1[1];
    int y2 = coord2[2];
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

int get_manhattan_dist(vector<int> coord1, vector<int> coord2)
{
    int x1 = coord1[0];
    int x2 = coord2[0];
    int y1 = coord1[1];
    int y2 = coord2[2];
    return abs(x1-x2) + abs(y1-y2);
}

/**
 * Function is based on https://github.com/tonyling/skb-solver
 */
int check_dead(SMap &map, vector<vector<int>> cans)
{
    int dead = 0;
    for (auto coords : cans)
	{
		bool N_wall = false;
		bool E_wall = false;
		bool S_wall = false;
		bool W_wall = false;
		bool in_corner = false;
		
		int cur_box_x = coords[0];
		int cur_box_y = coords[1];
		
		//check if there is a wall north of the box
		if (map[cur_box_y - 1][cur_box_x] == CHAR_WALL)
			N_wall = true;
		//check if there is a wall east of the box
		if (map[cur_box_y][cur_box_x + 1] == CHAR_WALL)
			E_wall = true;
		//check if there is a wall south of the box
		if (map[cur_box_y + 1][cur_box_x] == CHAR_WALL)
			S_wall = true;
		//check if there is a wall west of the box
		if (map[cur_box_y][cur_box_x - 1] == CHAR_WALL)
			W_wall = true;
		
		//first check if box is in a corner
		//check if box in NE corner
		if (N_wall && E_wall)
		{
			in_corner = true;
		}
		//check if box in NW corner
		else if (N_wall && W_wall)
		{
			in_corner = true;
		}
		//check if box in SE corner
		else if (S_wall && E_wall)
		{
			in_corner = true;
		}
		//check if box in SW corner
		else if (S_wall && W_wall)
		{
			in_corner = true;
		}
		
		//if box is ever in a corner, then box is in a deadlock position
		if (in_corner)
		{
			dead++;
		}
		//if box is next to a wall, check to see if wall is unbroken with
		//2 unsafe corners and no goals along the wall
		else
		{
			//if wall north of box, search for and east-most and west-most wall
			if (N_wall)
			{
				bool safe = false;
				bool corner_E = false;
				bool corner_W = false;
				
				//search east to see if there are accessible tiles along unbroken
				//north walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_x + 1; i < map[cur_box_y].size(); i++)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[cur_box_y][i] == CHAR_GOAL) ||
					 (map[cur_box_y][i] == CHAR_CAN_ON_GOAL) || (map[cur_box_y][i] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if north tile is not a wall, then it is safe
					if (map[cur_box_y - 1][i] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if NE corner
					if ((map[cur_box_y][i] == CHAR_WALL) && (map[cur_box_y - 1][i] == CHAR_WALL))
					{
						corner_E = true;
						break;
					}
				}
				
				//search west to see if there are accessible tiles along unbroken
				//north walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_x - 1; i >= 0 ; i--)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[cur_box_y][i] == CHAR_GOAL) ||
					 (map[cur_box_y][i] == CHAR_CAN_ON_GOAL) || (map[cur_box_y][i] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if north tile is not a wall, then it is safe
					if (map[cur_box_y - 1][i] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if NW corner
					if ((map[cur_box_y][i] == CHAR_WALL) && (map[cur_box_y - 1][i] == CHAR_WALL))
					{
						corner_W = true;
						break;
					}
				}
				
				//if unbroken wall along path east and west of the box to corners with any goal
				//increment score to signify unsafe position
				if (!safe)
				{
					if(corner_E && corner_W)
						dead++;
				}	
			}
			
			//if wall north of box, search for and north-most and south-most wall
			if (E_wall)
			{
				bool safe = false;
				bool corner_N = false;
				bool corner_S = false;
				
				//search north to see if there are accessible tiles along unbroken
				//east walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_y - 1; i >=0; i--)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[i][cur_box_x] == CHAR_GOAL) ||
					 (map[i][cur_box_x] == CHAR_CAN_ON_GOAL) || (map[i][cur_box_x] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if east tile is not a wall, then it is safe
					if (map[i][cur_box_x + 1] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if NE corner
					if ((map[i][cur_box_x] == CHAR_WALL) && (map[i][cur_box_x + 1] == CHAR_WALL))
					{
						corner_N = true;
						break;
					}
				}
				
				//search south to see if there are accessible tiles along unbroken
				//east walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_y + 1; i < map.size(); i++)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[i][cur_box_x] == CHAR_GOAL) ||
					 (map[i][cur_box_x] == CHAR_CAN_ON_GOAL) || (map[i][cur_box_x] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if east tile is not a wall, then it is safe
					if (map[i][cur_box_x + 1] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if SE corner
					if ((map[i][cur_box_x] == CHAR_WALL) && (map[i][cur_box_x + 1] == CHAR_WALL))
					{
						corner_S = true;
						break;
					}
				}
				
				//if unbroken wall along path east and west of the box to corners with any goal
				//increment score to signify unsafe position
				if (!safe)
				{
					if(corner_N && corner_S)
						dead++;
				}	
			}
			
			//if wall south of box, search for and east-most and west-most wall
			if (S_wall)
			{
				bool safe = false;
				bool corner_E = false;
				bool corner_W = false;
				
				//search east to see if there are accessible tiles along unbroken
				//south walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_x + 1; i < map[cur_box_y].size(); i++)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[cur_box_y][i] == CHAR_GOAL) ||
					 (map[cur_box_y][i] == CHAR_CAN_ON_GOAL) || (map[cur_box_y][i] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if south tile is not a wall, then it is safe
					if (map[cur_box_y + 1][i] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if SE corner
					if ((map[cur_box_y][i] == CHAR_WALL) && (map[cur_box_y + 1][i] == CHAR_WALL))
					{
						corner_E = true;
						break;
					}
				}
				
				//search west to see if there are accessible tiles along unbroken
				//south walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_x - 1; i >= 0 ; i--)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[cur_box_y][i] == CHAR_GOAL) ||
					 (map[cur_box_y][i] == CHAR_CAN_ON_GOAL) || (map[cur_box_y][i] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if south tile is not a wall, then it is safe
					if (map[cur_box_y + 1][i] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if SW corner
					if ((map[cur_box_y][i] == CHAR_WALL) && (map[cur_box_y + 1][i] == CHAR_WALL))
					{
						corner_W = true;
						break;
					}
				}
				
				//if unbroken wall along path east and west of the box to corners with any goal
				//increment score to signify unsafe position
				if (!safe)
				{
					if(corner_E && corner_W)
						dead++;
				}	
			}
			
			//if wall north of box, search for and north-most and south-most wall
			if (W_wall)
			{
				bool safe = false;
				bool corner_N = false;
				bool corner_S = false;
				
				//search north to see if there are accessible tiles along unbroken
				//east walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_y - 1; i >=0; i--)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[i][cur_box_x] == CHAR_GOAL) ||
					 (map[i][cur_box_x] == CHAR_CAN_ON_GOAL) || (map[i][cur_box_x] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if east tile is not a wall, then it is safe
					if (map[i][cur_box_x - 1] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if NW corner
					if ((map[i][cur_box_x] == CHAR_WALL) && (map[i][cur_box_x - 1] == CHAR_WALL))
					{
						corner_N = true;
						break;
					}
				}
				
				//search south to see if there are accessible tiles along unbroken
				//east walls until a corner is found.  boxes and players are ignored
				//and considered accessible tiles since they can move
				for (int i = cur_box_y + 1; i < map.size(); i++)
				{
					//if goal is found along the way then it cannot be an unsafe position
					if ((map[i][cur_box_x] == CHAR_GOAL) ||
					 (map[i][cur_box_x] == CHAR_CAN_ON_GOAL) || (map[i][cur_box_x] == CHAR_ROBOT_ON_GOAL))
					{
						safe = true;
						break;
					}
					
					//if east tile is not a wall, then it is safe
					if (map[i][cur_box_x - 1] != CHAR_WALL)
					{
						safe = true;
						break;
					}
					
					//if SW corner
					if ((map[i][cur_box_x] == CHAR_WALL) && (map[i][cur_box_x - 1] == CHAR_WALL))
					{
						corner_S = true;
						break;
					}
				}
				
				//if unbroken wall along path east and west of the box to corners with any goal
				//increment score to signify unsafe position
				if (!safe)
				{
					if(corner_N && corner_S)
						dead++;
				}	
			}
		}
	}
	return dead;
}

int calc_cost(SMap &map, string steps)
{
    // solution point and steps size added
    int cost = 0;

    vector<int> robo_pos;
    vector<vector<int>> goals;
    vector<vector<int>> cans;
    for(int y = 0; y < map.size(); y++)
    {
        string line = map[y];
        for(int x = 0; x < line.size(); x++)
        {
            char c = line[x];
            if (c == CHAR_GOAL || c == CHAR_ROBOT_ON_GOAL)
            {
                goals.push_back(vector<int>{x,y});
            }
            if (c == CHAR_CAN)
            {
                vector<int> can_coord{x,y};
                cans.push_back(can_coord);
            }
            if(c == CHAR_ROBOT || c == CHAR_ROBOT_ON_GOAL)
            {
                robo_pos.push_back(x);
                robo_pos.push_back(y);
            }
        }
    }
    int dead = check_dead(map, cans);
    if(dead>0)
    {
        return -1;
    }
    int least_robo_can_dist = 99999999;
    for(auto ccoord : cans)
    {
        // robot distance
        int dist = get_manhattan_dist(robo_pos, ccoord);
        if(dist < least_robo_can_dist)
        {
            least_robo_can_dist = dist;
        }
        // goal distance
        int best = 99999999;
        for(auto gcoord : goals)
        {
            dist = get_manhattan_dist(ccoord, gcoord);
            if(dist < best)
            {
                best = dist;
            }
        }
        cost += best;
        // blocking surroundings
        // for(char c : "lrud")
        // {
        //     auto bes_coord = get_next_coord(ccoord, c);
        //     char b = whats_here(map, bes_coord);
        //     if(b == CHAR_CAN || b == CHAR_CAN_ON_GOAL || b == CHAR_WALL)
        //     {
        //         cost++;
        //     }
        // }
    }
    cost += least_robo_can_dist;

    return cost;
}

void do_algorithm_astar_search(SMap &original_map)
{
    // make copy of map
    auto working_map(original_map);
    // create lists
    list<string> open_list;    
    vector<SMap> closed_list;
    list<int> ol_cost;

    // do init step stuff
    string steps("");
    open_list.push_back(steps);
    int best_cost = 9999999;
    ol_cost.push_back(best_cost);
    int last_size = steps.size();
    // iterate until there's nothing left
    while(open_list.empty() == false)
    {
        // get next element
        steps = open_list.front();
        open_list.pop_front();
        ol_cost.pop_front();
        int nu_size = steps.size();
        // if(last_size != nu_size)
        // {
        //     last_size = nu_size;
        //     cout<<"New size:"<<nu_size<<endl;
        //     cout<<"Steps:"<<steps<<endl;
        //     print_map(working_map);
        // }
        // generate new state
        working_map = original_map;
        bool did_apply = apply_steps(working_map, steps);
        if(did_apply == false) continue;
        // get cost
        int cost = calc_cost(working_map, steps);
        if(cost == -1)
        {
            continue;
        }
        // check if solution
        if(cost == 0)
        {
            cout << "Solution found:" << steps << endl;
            break;
        }
        // print
        if(cost < best_cost)
        {
            best_cost = cost;
            cout<<"New best cost:"<<cost<<endl;
            cout<<"Steps:"<<steps<<endl;
            print_map(working_map);
        }
        // check and add to closed list
        bool bused = false;
        for(auto used : closed_list)
        {
            if(used == working_map)
            {
                bused = true;
                break;
            }
        }
        if(bused) continue;
        closed_list.push_back(working_map);

        // insert these based on cost
        string up(steps);
        up.push_back('u');
        string down(steps);
        down.push_back('d');
        string left(steps);
        left.push_back('l');
        string right(steps);
        right.push_back('r');

        auto olit = open_list.begin();
        auto endit = ol_cost.end();
        for(auto it = ol_cost.begin();; it++, olit++)
        {
            if(it == endit || cost < *it)
            {
                ol_cost.insert(it, cost);
                ol_cost.insert(it, cost);
                ol_cost.insert(it, cost);
                ol_cost.insert(it, cost);
                
                open_list.insert(olit, up);
                open_list.insert(olit, down);
                open_list.insert(olit, left);
                open_list.insert(olit, right);

                break;
            }
        }
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

    // do_breadth_first_search(starting_map);
    // do_depth_first_search(starting_map, 115);
    do_algorithm_astar_search(starting_map);

    return 0;
}

