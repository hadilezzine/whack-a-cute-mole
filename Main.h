#ifndef DEFINES_H
#define DEFINES_H

#include <SFML/System/Time.hpp>

// Window
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "Whack-a-Cute-Mole!";

// Game States
enum class GameState {
    WELCOME,
    PLAYING,
    GAME_OVER
};

// Timing
const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f); // 60 FPS
const sf::Time GAME_DURATION = sf::seconds(30.0f);     // How long a game lasts
const sf::Time MOLE_TIME_UP_MIN = sf::seconds(0.6f);
const sf::Time MOLE_TIME_UP_MAX = sf::seconds(1.3f);
const sf::Time MOLE_SPAWN_TIME_MIN = sf::seconds(0.3f);
const sf::Time MOLE_SPAWN_TIME_MAX = sf::seconds(0.9f);
const sf::Time GAME_OVER_DELAY = sf::seconds(1.5f); // Small delay before showing Game Over text

// Mole Grid (Example: 3x3 grid)
const int GRID_ROWS = 3;
const int GRID_COLS = 3;
const int MOLE_COUNT = GRID_ROWS * GRID_COLS;
const float GRID_START_X = 150.0f;
const float GRID_START_Y = 150.0f;
const float GRID_SPACING_X = 180.0f;
const float GRID_SPACING_Y = 130.0f;


const std::string FONT_PATH = "assets/CuteFont.ttf"; 
const std::string BG_TEXTURE_PATH = "assets/background.jfif";
const std::string MOLE_HOLE_TEXTURE_PATH = "assets/hole.png";
const std::string MOLE_UP_TEXTURE_PATH = "assets/mole_up.png";
const std::string MOLE_HIT_TEXTURE_PATH = "assets/mole_hit.png";
const std::string HAMMER_TEXTURE_PATH = "assets/hammer.png"; 
const std::string HIGHSCORE_FILE = "highscore.txt";

#endif // DEFINES_H