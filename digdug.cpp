#include <iostream>
#include <vector>
#include <cstdlib>  // For system() and rand()
#include <ctime>    // For time()
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Game constants
const int WIDTH = 20;
const int HEIGHT = 15;
const char PLAYER = 'P';
const char ENEMY = 'E';
const char DIRT = '#';
const char EMPTY = ' ';
const char ROCK = 'O';
const char PUMP = '*';

// Direction constants
enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

// Game state
struct GameState {
    int playerX, playerY;
    int score;
    bool gameOver;
    std::vector<std::vector<char>> map;
    std::vector<std::pair<int, int>> enemies;
    std::vector<std::pair<int, int>> pumps;
    int pumpDirection;
    bool isPumping;
};

// Function prototypes
void initializeGame(GameState& game);
void drawGame(const GameState& game);
char getInput();
void handleInput(GameState& game, char key);
void updateGame(GameState& game);
void clearScreen();
bool isValidMove(const GameState& game, int x, int y);
void moveEnemies(GameState& game);
void updatePumps(GameState& game);

// Linux-specific keyboard input function
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

// Check if a key has been pressed
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

int main() {
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    GameState game;
    initializeGame(game);
    
    while (!game.gameOver) {
        drawGame(game);
        
        // Check for input
        if (kbhit()) {
            char key = getch();
            handleInput(game, key);
        }
        
        updateGame(game);
        
        // Add a small delay
        usleep(150000);  // 150ms delay
    }
    
    clearScreen();
    std::cout << "Game Over! Final Score: " << game.score << std::endl;
    return 0;
}

void initializeGame(GameState& game) {
    // Initialize game state
    game.playerX = WIDTH / 2;
    game.playerY = HEIGHT / 2;
    game.score = 0;
    game.gameOver = false;
    game.isPumping = false;
    game.pumpDirection = RIGHT;
    
    // Initialize map with dirt
    game.map.resize(HEIGHT, std::vector<char>(WIDTH, DIRT));
    
    // Create some empty tunnels and rocks
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1) {
                game.map[i][j] = DIRT;  // Border is always dirt
            } else if (rand() % 10 == 0) {
                game.map[i][j] = ROCK;  // 10% chance of rock
            }
        }
    }
    
    // Place player
    game.map[game.playerY][game.playerX] = PLAYER;
    
    // Create enemies
    for (int i = 0; i < 3; i++) {
        int x, y;
        do {
            x = rand() % (WIDTH - 2) + 1;
            y = rand() % (HEIGHT - 2) + 1;
        } while ((x == game.playerX && y == game.playerY) || game.map[y][x] == ROCK);
        
        game.enemies.push_back({x, y});
        game.map[y][x] = ENEMY;
    }
}

void drawGame(const GameState& game) {
    clearScreen();
    
    std::cout << "Dig Dug Clone - Score: " << game.score << std::endl;
    std::cout << "Use WASD to move, SPACE to pump, Q to quit" << std::endl;
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == game.playerY && j == game.playerX) {
                // Red color for player
                std::cout << "\033[1;31m" << PLAYER << "\033[0m ";
            } else if (game.map[i][j] == ENEMY) {
                // Blue color for enemies
                std::cout << "\033[1;34m" << ENEMY << "\033[0m ";
            } else if (game.map[i][j] == PUMP) {
                // Yellow color for pump
                std::cout << "\033[1;33m" << PUMP << "\033[0m ";
            } else if (game.map[i][j] == ROCK) {
                // Gray color for rocks
                std::cout << "\033[1;37m" << ROCK << "\033[0m ";
            } else {
                std::cout << game.map[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

void handleInput(GameState& game, char key) {
    int newX = game.playerX;
    int newY = game.playerY;
    
    if (key == ' ') {  // Space to pump
        game.isPumping = true;
        // Create a pump in the direction the player is facing
        int pumpX = game.playerX;
        int pumpY = game.playerY;
        
        switch (game.pumpDirection) {
            case UP: pumpY--; break;
            case DOWN: pumpY++; break;
            case LEFT: pumpX--; break;
            case RIGHT: pumpX++; break;
        }
        
        if (isValidMove(game, pumpX, pumpY)) {
            game.pumps.push_back({pumpX, pumpY});
            if (game.map[pumpY][pumpX] != ENEMY) {
                game.map[pumpY][pumpX] = PUMP;
            }
        }
        return;
    } else if (key == 'q' || key == 'Q') {
        game.gameOver = true;
        return;
    }
    
    switch (key) {
        case 'w': case 'W': 
            newY--; 
            game.pumpDirection = UP;
            break;  // Up
        case 's': case 'S': 
            newY++; 
            game.pumpDirection = DOWN;
            break;  // Down
        case 'a': case 'A': 
            newX--; 
            game.pumpDirection = LEFT;
            break;  // Left
        case 'd': case 'D': 
            newX++; 
            game.pumpDirection = RIGHT;
            break;  // Right
    }
    
    if (isValidMove(game, newX, newY)) {
        // Remove player from old position
        game.map[game.playerY][game.playerX] = EMPTY;
        
        // If moving into dirt, increase score
        if (game.map[newY][newX] == DIRT) {
            game.score += 10;
        }
        
        // Update player position
        game.playerX = newX;
        game.playerY = newY;
        game.map[game.playerY][game.playerX] = PLAYER;
    }
}

void updateGame(GameState& game) {
    // Update pumps
    updatePumps(game);
    
    // Move enemies
    moveEnemies(game);
    
    // Check if player is caught by enemy
    for (const auto& enemy : game.enemies) {
        if (enemy.first == game.playerX && enemy.second == game.playerY) {
            game.gameOver = true;
            return;
        }
    }
    
    // Check win condition (all enemies defeated)
    if (game.enemies.empty()) {
        clearScreen();
        std::cout << "You win! All enemies defeated!" << std::endl;
        std::cout << "Final Score: " << game.score << std::endl;
        game.gameOver = true;
    }
}

bool isValidMove(const GameState& game, int x, int y) {
    // Check boundaries
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }
    
    // Can't move through rocks
    if (game.map[y][x] == ROCK) {
        return false;
    }
    
    return true;
}

void moveEnemies(GameState& game) {
    for (size_t i = 0; i < game.enemies.size(); i++) {
        // Remove enemy from old position
        game.map[game.enemies[i].second][game.enemies[i].first] = EMPTY;
    }
    
    for (size_t i = 0; i < game.enemies.size(); i++) {
        int& enemyX = game.enemies[i].first;
        int& enemyY = game.enemies[i].second;
        
        // Simple AI: Move toward player if possible
        Direction dir = NONE;
        
        // Decide direction (with some randomness)
        if (rand() % 3 == 0) {
            // Random movement 1/3 of the time
            dir = static_cast<Direction>(rand() % 4);
        } else {
            // Move toward player 2/3 of the time
            if (enemyX < game.playerX) dir = RIGHT;
            else if (enemyX > game.playerX) dir = LEFT;
            else if (enemyY < game.playerY) dir = DOWN;
            else if (enemyY > game.playerY) dir = UP;
        }
        
        // Try to move in the chosen direction
        int newX = enemyX;
        int newY = enemyY;
        
        switch (dir) {
            case UP: newY--; break;
            case DOWN: newY++; break;
            case LEFT: newX--; break;
            case RIGHT: newX++; break;
            case NONE: break;
        }
        
        // Check if the move is valid
        if (isValidMove(game, newX, newY) && game.map[newY][newX] != PLAYER) {
            enemyX = newX;
            enemyY = newY;
        }
    }
    
    // Place enemies back on the map
    for (const auto& enemy : game.enemies) {
        game.map[enemy.second][enemy.first] = ENEMY;
    }
}

void updatePumps(GameState& game) {
    // Check if any pumps hit enemies
    for (auto it = game.pumps.begin(); it != game.pumps.end(); ) {
        bool pumpHit = false;
        
        // Check if pump hit an enemy
        for (auto enemyIt = game.enemies.begin(); enemyIt != game.enemies.end(); ) {
            if (enemyIt->first == it->first && enemyIt->second == it->second) {
                // Enemy hit by pump
                game.score += 100;
                enemyIt = game.enemies.erase(enemyIt);
                pumpHit = true;
            } else {
                ++enemyIt;
            }
        }
        
        if (pumpHit) {
            // Remove the pump
            game.map[it->second][it->first] = EMPTY;
            it = game.pumps.erase(it);
        } else {
            ++it;
        }
    }
    
    // Clear pumps after a short time
    if (!game.isPumping && !game.pumps.empty()) {
        for (const auto& pump : game.pumps) {
            if (game.map[pump.second][pump.first] == PUMP) {
                game.map[pump.second][pump.first] = EMPTY;
            }
        }
        game.pumps.clear();
    }
    
    game.isPumping = false;
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";  // ANSI escape sequence to clear screen
}
