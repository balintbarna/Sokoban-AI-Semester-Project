# Sokoban Robot Program

This is a python program running on an ev3 robot. Its job is to run on a sokoban map, realizing the precomputed solution for the puzzle.

## Parts

simple_pid external library is used. It is included by source code, because installing it on the robot was more difficult.

Some example programs are included in the project, their purpose is to showcase how to use the ev3dev library

In the test folder, python programs can be written for unit tests.

In the src folder the main program is found, main.py. It uses a state machine to realize behaviours, which will depend on the robots current state and precomputed puzzle solution. Under that, there's reactive controls, for example a line follower. In the driver folder we can find drivers for each hardware API which gives us very easy to use functions to control the robot and read sensors.