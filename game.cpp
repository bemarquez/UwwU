#include "game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

SnakeGame::SnakeGame() : isRunning(true), isPaused(false), score(0), level(1) {
    srand(static_cast<unsigned>(time(0)));
    initialize();
}

void SnakeGame::run() {
    showMenu();
    while (isRunning) {
        while (isRunning && !isPaused) {
            processInput();
            update();
            render();
        }

        while (isPaused) {
            renderPauseMessage();
            processPauseInput();
        }
    }
}

void SnakeGame::initialize() {
    snake.clear();
    snake.push_back({ width / 2, height / 2 });
    direction = { 1, 0 };
    score = 0;
    level = 1;
    food = { rand() % width, rand() % height };
}

void SnakeGame::render() {
    std::vector<std::string> buffer(height + 2, std::string(width + 2, ' '));

    for (int i = 0; i < width + 2; ++i) {
        buffer[0][i] = '#';
        buffer[height + 1][i] = '#';
    }

    for (int i = 0; i < height; ++i) {
        buffer[i + 1][0] = '#';
        buffer[i + 1][width + 1] = '#';
    }

    buffer[food.y + 1][food.x + 1] = '0';

    bool isHead = true;
    for (const auto& s : snake) {
        if (isHead) {
            buffer[s.y + 1][s.x + 1] = '0';
            isHead = false;
        }
        else {
            buffer[s.y + 1][s.x + 1] = 'O';
        }
    }

    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

    for (const auto& line : buffer) {
        std::cout << line << '\n';
    }

    std::cout << "Player: " << playerName << " Score: " << score << " Level: " << level << "\n";
}

void SnakeGame::renderPauseMessage() {
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = height + 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    std::cout << "=== JOGO PAUSADO ===\n";
    std::cout << "Pressione 'r' para resumir ou 'q' para sair\n";
}

void SnakeGame::update() {
    Point newHead = { snake.front().x + direction.x, snake.front().y + direction.y };

    if (newHead.x >= width || newHead.x < 0 || newHead.y >= height || newHead.y < 0) {
        gameOver();
        return;
    }

    for (const auto& s : snake) {
        if (newHead.x == s.x && newHead.y == s.y) {
            gameOver();
            return;
        }
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y) {
        score += 10;
        if (score % 100 == 0) {
            level++;
        }
        food = { rand() % width, rand() % height };
    }
    else {
        snake.pop_back();
    }

    if (level == 80) {
        victory();
        return;
    }

    int speed = 300 - (level - 1) * 20;
    if (speed < 50) speed = 50;
    Sleep(speed);
}

void SnakeGame::victory() {
    system("cls");
    std::cout << "=== PARABENS! VOCE VENCEU! ===\n";
    std::cout << "Jogador: " << playerName << "\n";
    std::cout << "Score final: " << score << "\n";
    std::cout << "1. Recomeçar\n";
    std::cout << "2. Sair\n";
    std::cout << "Digite sua escolha: ";

    char choice;
    std::cin >> choice;
    switch (choice) {
    case '1':
        reset();
        break;
    case '2':
        isRunning = false;
        break;
    default:
        std::cout << "Escolha invalida! Tente novamente.\n";
        victory();
        break;
    }
}

void SnakeGame::processInput() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
            if (direction.y == 0) direction = { 0, -1 };
            break;
        case 's':
            if (direction.y == 0) direction = { 0, 1 };
            break;
        case 'a':
            if (direction.x == 0) direction = { -1, 0 };
            break;
        case 'd':
            if (direction.x == 0) direction = { 1, 0 };
            break;
        case 'p':
            isPaused = true;
            break;
        case 'q':
            isRunning = false;
            break;
        }
    }
}

void SnakeGame::processPauseInput() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'r':
            isPaused = false;
            break;
        case 'q':
            isRunning = false;
            break;
        }
    }
}

void SnakeGame::showMenu() {
    system("cls");
    std::cout << "=== JOGO DA COBRINHA ===\n";
    std::cout << "1. Start\n";
    std::cout << "2. Mostrar comandos\n";
    std::cout << "3. Sair\n";
    std::cout << "Digite uma opcao: ";

    char choice;
    std::cin >> choice;
    switch (choice) {
    case '1':
        enterPlayerName();
        reset();
        break;
    case '2':
        showCommands();
        break;
    case '3':
        isRunning = false;
        break;
    default:
        std::cout << "Escolha invalida! Tente novamente.\n";
        showMenu();
        break;
    }
}

void SnakeGame::showCommands() {
    system("cls");
    std::cout << "=== COMANDOS ===\n";
    std::cout << "w - Mover para cima\n";
    std::cout << "s - Mover para baixo\n";
    std::cout << "a - Mover para esquerda\n";
    std::cout << "d - Mover para direita\n";
    std::cout << "p - Pausar\n";
    std::cout << "r - Resumir\n";
    std::cout << "q - Sair\n";
    std::cout << "\nPressione qualquer tecla para voltar ao menu...";
    _getch();
    showMenu();
}

void SnakeGame::enterPlayerName() {
    system("cls");
    std::cout << "Digite seu nome: ";
    std::cin >> playerName;
}

void SnakeGame::gameOver() {
    system("cls");
    std::cout << "=== GAME OVER ===\n";
    std::cout << "Jogador: " << playerName << "\n";
    std::cout << "Score final: " << score << "\n";
    std::cout << "1. Recomecar\n";
    std::cout << "2. Sair\n";
    std::cout << "Digite sua escolha: ";

    char choice;
    std::cin >> choice;
    switch (choice) {
    case '1':
        reset();
        break;
    case '2':
        isRunning = false;
        break;
    default:
        std::cout << "Escolha invalida! Tente novamente.\n";
        gameOver();
        break;
    }
}

void SnakeGame::reset() {
    isRunning = true;
    isPaused = false;
    initialize();
}