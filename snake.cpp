#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <conio.h>
#include <windows.h>
using namespace std;

const int WIDTH      = 20;
const int HEIGHT     = 20;
const int FOODMAX    = 3;
const int STARTDELAY = 150;
const int MINDELAY   = 50;
const string SAVEFILE = "highscore.txt";

struct Point
{
    int x, y;
};

vector<Point> snake;
vector<Point> foods;

int movex = 1;
int movey = 0;

bool dead = false;
bool wrap = false;

int score     = 0;
int bestscore = 0;

HANDLE console;


// reads best score from file
void loadbest()
{
    ifstream f(SAVEFILE);
    if (f.is_open())
        f >> bestscore;
}

// saves best score to file if player beat it
void savebest()
{
    if (score > bestscore)
    {
        bestscore = score;
        ofstream f(SAVEFILE);
        if (f.is_open())
            f << bestscore;
    }
}

// checks if a food is on this tile
bool foodhere(int x, int y)
{
    for (int i = 0; i < (int)foods.size(); i++)
        if (foods[i].x == x && foods[i].y == y)
            return true;
    return false;
}

// checks if the snake is on this tile
bool snakehere(int x, int y)
{
    for (int i = 0; i < (int)snake.size(); i++)
        if (snake[i].x == x && snake[i].y == y)
            return true;
    return false;
}

// finds a random empty tile to put food on
Point makefood()
{
    Point p;
    do
    {
        p.x = rand() % WIDTH;
        p.y = rand() % HEIGHT;
    }
    while (snakehere(p.x, p.y) || foodhere(p.x, p.y));
    return p;
}

// gets the current speed, faster every 5 points
int getdelay()
{
    int d = STARTDELAY - (score / 5) * 5;
    return max(d, MINDELAY);
}

// hides the blinking cursor
void hidecursor()
{
    CONSOLE_CURSOR_INFO info = { 1, FALSE };
    SetConsoleCursorInfo(console, &info);
}

// writes a line directly to the console buffer so the screen doesnt flicker
void drawrow(int row, const string &line)
{
    COORD pos = { 0, (SHORT)row };
    DWORD written;
    WriteConsoleOutputCharacterA(console, line.c_str(), (DWORD)line.size(), pos, &written);
}

// draws the board every tick
void draw()
{
    string border = "+" + string(WIDTH, '-') + "+";
    drawrow(0, border);

    for (int y = 0; y < HEIGHT; y++)
    {
        string row = "|";
        for (int x = 0; x < WIDTH; x++)
        {
            if      (foodhere (x, y)) row += 'F';
            else if (snakehere(x, y)) row += 'O';
            else                      row += ' ';
        }
        row += "|";
        drawrow(y + 1, row);
    }

    drawrow(HEIGHT + 1, border);

    string info = " Score:" + to_string(score)
                + "  Best:" + to_string(bestscore)
                + "  Speed:" + to_string((STARTDELAY - getdelay()) / 5 + 1)
                + "  Wrap:" + (wrap ? "ON " : "OFF")
                + "  [WASD] move  [T] wrap  [Q] quit     ";
    drawrow(HEIGHT + 2, info);
}

// handles keyboard input
void handleinput()
{
    if (!_kbhit())
        return;

    char key = _getch();

    if ((key == 'w' || key == 'W') && movey == 0) { movex =  0; movey = -1; }
    if ((key == 's' || key == 'S') && movey == 0) { movex =  0; movey =  1; }
    if ((key == 'a' || key == 'A') && movex == 0) { movex = -1; movey =  0; }
    if ((key == 'd' || key == 'D') && movex == 0) { movex =  1; movey =  0; }
    if  (key == 'q' || key == 'Q') dead = true;
    if  (key == 't' || key == 'T') wrap = !wrap;
}

// moves the snake, check collisions, handles eating
void update()
{
    Point head = snake.front();
    head.x += movex;
    head.y += movey;

    if (wrap)
    {
        head.x = (head.x + WIDTH)  % WIDTH;
        head.y = (head.y + HEIGHT) % HEIGHT;
    }
    else
    {
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT)
        {
            dead = true;
            return;
        }
    }

    if (snakehere(head.x, head.y))
    {
        dead = true;
        return;
    }

    snake.insert(snake.begin(), head);

    bool ate = false;
    for (int i = 0; i < (int)foods.size(); i++)
    {
        if (head.x == foods[i].x && head.y == foods[i].y)
        {
            score++;
            foods.erase(foods.begin() + i);
            foods.push_back(makefood());
            ate = true;
            break;
        }
    }

    if (!ate)
        snake.pop_back();
}

int main()
{
    srand((unsigned)time(0));

    console = GetStdHandle(STD_OUTPUT_HANDLE);

    loadbest();
    hidecursor();
    system("cls");

    snake.push_back({10, 10});

    for (int i = 0; i < FOODMAX; i++)
        foods.push_back(makefood());

    while (!dead)
    {
        draw();
        handleinput();
        update();
        Sleep(getdelay());
    }

    savebest();

    COORD pos = { 0, (SHORT)(HEIGHT + 5) };
    SetConsoleCursorPosition(console, pos);
    cout << "  game over\n\n";
    cout << "  your score : " << score     << "\n";
    cout << "  best score : " << bestscore << "\n\n";
    cout << "  press any key to exit\n";
    _getch();

    return 0;
}