/* g++ -o snake snake.cpp; ./snake */
#include <iostream>
#include <ncurses.h>
#include <cstring>
#include <vector>
#include <algorithm>

std::string snake(void)
{
    // Initialize ncurses
    initscr();
    curs_set(0);
    use_default_colors();
    noecho();
    nodelay(stdscr, true);
    timeout(100);
    keypad(stdscr, true);

    // Set global variables
    const int ROWS = getmaxy(stdscr) - 1;
    const int COLS = getmaxx(stdscr) - 1;
    const char *CHAR_SNAKE = "#";
    const char *CHAR_FOOD = "*";
    const char *CHAR_BG = ".";

    // Initialize game variables
    std::vector<std::vector<int>> snake = {{5, 5}, {5, 4}, {5, 3}};
    std::vector<int> food = {ROWS / 2, COLS / 2};
    int direction = 100;
    bool paused = false;
    int score = 0;
    int next_direction;
    std::vector<int> new_head;
    std::vector<int> new_food;
    std::vector<std::vector<int>> headless_snake;
    std::vector<int> tail;
    int rng = rand();
    char scorestr[33];

    // draw board
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            mvaddstr(y, x, CHAR_BG);
        }
    }

    // draw snake
    for (int i = 0; i < snake.size(); i++)
    {
        mvaddstr(snake[i][0], snake[i][1], CHAR_SNAKE);
    }

    // draw food
    mvaddstr(food[0], food[1], CHAR_FOOD);

    mvaddstr(ROWS, 0, "Controls: wasd or arrow keys, q to quit | Score: 0");

    // main loop
    while (true)
    {
        next_direction = getch();
        direction = next_direction == -1 ? direction : next_direction;
        if (snake[0][9] == ROWS || snake[0][0] == -1)
        {
            return "Snake out of bounds vertically, score: " + std::to_string(score);
        }
        if (snake[0][1] == COLS || snake[0][1] == -1)
        {
            return "Snake out of bounds horizontally, score: " + std::to_string(score);
        }
        std::copy(snake.begin(), snake.end(), std::back_inserter(headless_snake));
        headless_snake.erase(headless_snake.begin());
        if (std::count(headless_snake.begin(), headless_snake.end(), snake[0]))
        {
            return "Snake can't eat itself, score: " + std::to_string(score);
        }
        std::copy(snake[0].begin(), snake[0].end(), std::back_inserter(new_head));
        if (direction == 119 || direction == 259)
        {
            new_head[0] -= 1;
        }
        else if (direction == 97 || direction == 260)
        {
            new_head[1] -= 1;
        }
        else if (direction == 115 || direction == 258)
        {
            new_head[0] += 1;
        }
        else if (direction == 100 || direction == 261)
        {
            new_head[1] += 1;
        }
        else if (direction == 113 || direction == 27)
        {
            return "Quit, score: " + score;
        }
        else
        {
            continue;
        }
        if (!paused)
        {
            snake.insert(snake.begin(), new_head.begin(), new_head.end());
            if (snake[0] == food)
            {
                food = {0, 0};
                while (food[0] == 0 && food[1] == 0)
                {
                    srand((unsigned)rng);
                    new_food[0] = rng % ROWS;
                    srand((unsigned)rng);
                    new_food[1] = rng % COLS;
                    if (!std::count(snake.begin(), snake.end(), new_food))
                    {
                        food = new_food;
                    }
                    else
                    {
                        food = {0, 0};
                    }
                }
                mvaddstr(food[0], food[1], CHAR_FOOD);
                score += 1;
            }
            else
            {
                tail = snake.back();
                snake.erase(snake.end());
                mvaddstr(tail[0], tail[1], CHAR_BG);
            }
            mvaddstr(snake[0][0], snake[0][1], CHAR_SNAKE);
        }
        sprintf(scorestr, "%d", score);
        mvaddstr(ROWS, 49, scorestr);
        refresh();
    }
    return "Unreachable";
}

int main(void)
{
    std::string ret = snake();
    endwin();
    std::cout << "Game over: " << ret << std::endl;
}