# Sokoban Solver (c++)
This is the readme for the Sokoban Solver (c++) that is part of the Sokoban Project.

## Specification
The solver reads the map from a file. The map consists of the following characters:

* █: wall
* ◌: goal
* ■: can
* ◙: can on goal
* ☺: robot
* ☻: robot on goal
(space): road

This representation is human-friendly, easy to evaluate for a human when printed. The program does not really care how it looks as longs as it is an obvious and simple (everything is one character) representation.

### Example

```
███████
█◌    █
█     █
█◌█◌ ◌█
████ ██
█ ■   █
█ ■   █
█  ████
█ ■ ■☺█
█     █
█   ███
███████
```

### Steps

A step can be represented by the characters

* u (up) 
* r (right) 
* l (left)
* d (down) 

Capital letters are used when the robot is pushing a can. This information is necessary because when the robot finished pushing a can it needs to push it to the next coordinate (a half movement), then move back (another half movement), so it adds two half movements as an extra command.

### Solutions

A (partial) solution is a sequence of these steps, represented by a string of these characters.

#### Example
"uullU"

### Search

1. The open list will start with one item, "" (empty partial solution).
2. On each expansion, the next item will be removed, ("" in first iteration,) then a direction will be appended at the end of it and the new partial solution will be added to the open list.
3. A new partial solution will be added for all possible directions.
4. Valid directions should be tested. There has to be a function which generates the current state of the map from the original state and the partial solution. There will be a function inside of it which checks if the next step is valid or not.
5. It will also check for a successful solution (there are no empty goals and no cans not on goals)
6. Where a new partial solution will be added in an open list depends on the search strategy. In breadth first, it would be added to the end of the list (it will go through all others first, before continuing with the current one), like a queue. In depth first, it would add it to the beginning of the list (it will go through one complete direction vertically, then go on to the next option), like a stack. An iterative depth-first could be more optimal here. Better search algorithms may give better results.
7. When it finds a successful solution, it should not be expanded further.
8. If this kind of search finds the best solution first, it should not keep searching when it finds a solution
