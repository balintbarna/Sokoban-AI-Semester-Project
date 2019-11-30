from collections import deque

from enum import Enum

class Command(Enum):
    TURN_LEFT = 1
    TURN_RIGHT = 2
    TURN_AROUND = 3
    GO_STRAIGHT = 4
    PUSH_CAN_AND_RETURN = 5

cmdlist = deque()