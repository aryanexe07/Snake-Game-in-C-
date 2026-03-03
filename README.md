# Snake-Game-in-C++

A simple snake game that runs in the windows terminal. Built in C++.

## How to run

Compile it with g++:

```
g++ snake.cpp -o snake.exe
```

Then just run it:

```
snake.exe
```

## Controls

| Key | What it does |
|-----|-------------|
| W A S D | move the snake |
| T | toggle wrap mode on and off |
| Q | quit the game |

## Features

- The snake speeds up every 5 points
- 3 food items on the board at all times
- Wrap mode lets the snake go through walls instead of dying
- Best score gets saved to a file so it carries over between sessions

## Notes

- Windows only, uses windows.h for the console drawing
- The board is 20x20
- Best score is saved in a file called highscore.txt next to the exe
