#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Game constants
const int TILE_SIZE = 40;
const int WIDTH = 20;
const int HEIGHT = 15;
const int WINDOW_WIDTH = WIDTH * TILE_SIZE;
const int WINDOW_HEIGHT = HEIGHT * TILE_SIZE + 50; // Extra space for score

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
    sf::Clock enemyMoveClock;
    sf::Clock pumpClock;
};

// Function prototypes
void initializeGame(GameState& game);
void handleInput(GameState& game, sf::RenderWindow& window);
void updateGame(GameState& game);
bool isValidMove(const GameState& game, int x, int y);
void moveEnemies(GameState& game);
void updatePumps(GameState& game);
void drawGame(GameState& game, sf::RenderWindow& window, sf::Font& font, 
              sf::RectangleShape& playerShape, sf::RectangleShape& enemyShape, 
              sf::RectangleShape& dirtShape, sf::RectangleShape& rockShape, 
              sf::RectangleShape& pumpShape);

int main() {
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create the game window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dig Dug - SFML");
    window.setFramerateLimit(60);
    
    // Load font
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }
    
    // Create shapes for game elements
    sf::RectangleShape playerShape(sf::Vector2f(TILE_SIZE - 4, TILE_SIZE - 4));
    playerShape.setFillColor(sf::Color::Red);
    playerShape.setOutlineThickness(2);
    playerShape.setOutlineColor(sf::Color::White);
    
    sf::RectangleShape enemyShape(sf::Vector2f(TILE_SIZE - 8, TILE_SIZE - 8));
    enemyShape.setFillColor(sf::Color::Blue);
    enemyShape.setOutlineThickness(2);
    enemyShape.setOutlineColor(sf::Color::White);
    
    sf::RectangleShape dirtShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    dirtShape.setFillColor(sf::Color(139, 69, 19)); // Brown
    
    sf::RectangleShape rockShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    rockShape.setFillColor(sf::Color(169, 169, 169)); // Gray
    
    sf::RectangleShape pumpShape(sf::Vector2f(TILE_SIZE - 10, TILE_SIZE - 10));
    pumpShape.setFillColor(sf::Color::Yellow);
    
    // Initialize game state
    GameState game;
    initializeGame(game);
    
    // Game loop
    while (window.isOpen() && !game.gameOver) {
        // Handle events
        handleInput(game, window);
        
        // Update game state
        updateGame(game);
        
        // Draw the game
        window.clear(sf::Color::Black);
        drawGame(game, window, font, playerShape, enemyShape, dirtShape, rockShape, pumpShape);
        window.display();
    }
    
    // Game over screen
    if (game.gameOver) {
        window.clear(sf::Color::Black);
        
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("Game Over!");
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(
            WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2,
            WINDOW_HEIGHT / 2 - 40
        );
        
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Final Score: " + std::to_string(game.score));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(
            WINDOW_WIDTH / 2 - scoreText.getLocalBounds().width / 2,
            WINDOW_HEIGHT / 2 + 20
        );
        
        window.draw(gameOverText);
        window.draw(scoreText);
        window.display();
        
        // Wait for a few seconds before closing
        sf::sleep(sf::seconds(3));
    }
    
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
    game.map.resize(HEIGHT, std::vector<char>(WIDTH, '#')); // # represents dirt
    
    // Create some empty tunnels and rocks
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1) {
                game.map[i][j] = '#';  // Border is always dirt
            } else if (rand() % 10 == 0) {
                game.map[i][j] = 'O';  // 10% chance of rock
            }
        }
    }
    
    // Place player
    game.map[game.playerY][game.playerX] = 'P';
    
    // Create enemies
    for (int i = 0; i < 3; i++) {
        int x, y;
        do {
            x = rand() % (WIDTH - 2) + 1;
            y = rand() % (HEIGHT - 2) + 1;
        } while ((x == game.playerX && y == game.playerY) || game.map[y][x] == 'O');
        
        game.enemies.push_back({x, y});
        game.map[y][x] = 'E';
    }
    
    // Initialize clocks
    game.enemyMoveClock.restart();
    game.pumpClock.restart();
}

void handleInput(GameState& game, sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            int newX = game.playerX;
            int newY = game.playerY;
            
            // Handle Space key (pump) separately
            if (event.key.code == sf::Keyboard::Space) {
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
                    if (game.map[pumpY][pumpX] != 'E') {
                        game.map[pumpY][pumpX] = '*';
                    }
                    game.pumpClock.restart();
                }
                return;
            }
            
            // Handle Escape key separately
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
                return;
            }
            
            // Handle movement keys
            switch (event.key.code) {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    newY--;
                    game.pumpDirection = UP;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    newY++;
                    game.pumpDirection = DOWN;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    newX--;
                    game.pumpDirection = LEFT;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    newX++;
                    game.pumpDirection = RIGHT;
                    break;
                default:
                    return;
            }
            
            if (isValidMove(game, newX, newY)) {
                // Remove player from old position
                game.map[game.playerY][game.playerX] = ' ';
                
                // If moving into dirt, increase score
                if (game.map[newY][newX] == '#') {
                    game.score += 10;
                }
                
                // Update player position
                game.playerX = newX;
                game.playerY = newY;
                game.map[game.playerY][game.playerX] = 'P';
            }
        }
    }
}

void updateGame(GameState& game) {
    // Update pumps
    updatePumps(game);
    
    // Move enemies every 0.5 seconds
    if (game.enemyMoveClock.getElapsedTime().asSeconds() >= 0.5) {
        moveEnemies(game);
        game.enemyMoveClock.restart();
    }
    
    // Check if player is caught by enemy
    for (const auto& enemy : game.enemies) {
        if (enemy.first == game.playerX && enemy.second == game.playerY) {
            game.gameOver = true;
            return;
        }
    }
    
    // Check win condition (all enemies defeated)
    if (game.enemies.empty()) {
        game.gameOver = true;
    }
}

bool isValidMove(const GameState& game, int x, int y) {
    // Check boundaries
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }
    
    // Can't move through rocks
    if (game.map[y][x] == 'O') {
        return false;
    }
    
    return true;
}

void moveEnemies(GameState& game) {
    for (size_t i = 0; i < game.enemies.size(); i++) {
        // Remove enemy from old position
        game.map[game.enemies[i].second][game.enemies[i].first] = ' ';
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
        if (isValidMove(game, newX, newY) && game.map[newY][newX] != 'P') {
            enemyX = newX;
            enemyY = newY;
        }
    }
    
    // Place enemies back on the map
    for (const auto& enemy : game.enemies) {
        game.map[enemy.second][enemy.first] = 'E';
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
            game.map[it->second][it->first] = ' ';
            it = game.pumps.erase(it);
        } else {
            ++it;
        }
    }
    
    // Clear pumps after a short time (1 second)
    if (game.pumpClock.getElapsedTime().asSeconds() >= 1.0 && !game.pumps.empty()) {
        for (const auto& pump : game.pumps) {
            if (game.map[pump.second][pump.first] == '*') {
                game.map[pump.second][pump.first] = ' ';
            }
        }
        game.pumps.clear();
    }
}

void drawGame(GameState& game, sf::RenderWindow& window, sf::Font& font, 
              sf::RectangleShape& playerShape, sf::RectangleShape& enemyShape, 
              sf::RectangleShape& dirtShape, sf::RectangleShape& rockShape, 
              sf::RectangleShape& pumpShape) {
    
    // Draw the map
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            switch (game.map[i][j]) {
                case '#': // Dirt
                    dirtShape.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                    window.draw(dirtShape);
                    break;
                case 'O': // Rock
                    rockShape.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                    window.draw(rockShape);
                    break;
                case '*': // Pump
                    pumpShape.setPosition(j * TILE_SIZE + 5, i * TILE_SIZE + 5);
                    window.draw(pumpShape);
                    break;
                default:
                    break;
            }
        }
    }
    
    // Draw enemies
    for (const auto& enemy : game.enemies) {
        enemyShape.setPosition(enemy.first * TILE_SIZE + 4, enemy.second * TILE_SIZE + 4);
        window.draw(enemyShape);
    }
    
    // Draw player
    playerShape.setPosition(game.playerX * TILE_SIZE + 2, game.playerY * TILE_SIZE + 2);
    window.draw(playerShape);
    
    // Draw score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(game.score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, HEIGHT * TILE_SIZE + 10);
    window.draw(scoreText);
}
