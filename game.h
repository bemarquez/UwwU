#ifndef GAME_H
#define GAME_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

struct Point {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame();
    void run();

private:
    void initialize();
    void render();
    void renderPauseMessage();
    void update();
    void processInput();
    void processPauseInput();
    void showMenu();
    void showCommands();
    void enterPlayerName();
    void gameOver();
    void reset();
    void victory();

    bool isRunning;
    bool isPaused;
    int score;
    int level;
    Point food;
    std::vector<Point> snake;
    Point direction;
    std::string playerName;

    const int width = 40;
    const int height = 20;
};

#endif