#ifndef DEFINES_H
#define DEFINES_H

#include <SFML/System/Time.hpp>

// Window
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "Whack-a-Cute-Mole!";

enum class GameState {
    WELCOME,
    PLAYING,
    GAME_OVER
};

const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f); // 60 FPS
const sf::Time GAME_DURATION = sf::seconds(30.0f);     
const sf::Time MOLE_TIME_UP_MIN = sf::seconds(0.6f);
const sf::Time MOLE_TIME_UP_MAX = sf::seconds(1.3f);
const sf::Time MOLE_SPAWN_TIME_MIN = sf::seconds(0.3f);
const sf::Time MOLE_SPAWN_TIME_MAX = sf::seconds(0.9f);
const sf::Time GAME_OVER_DELAY = sf::seconds(1.5f); 

const int GRID_ROWS = 3;
const int GRID_COLS = 3;
const int MOLE_COUNT = GRID_ROWS * GRID_COLS;
const float GRID_START_X = 150.0f;
const float GRID_START_Y = 140.0f;
const float GRID_SPACING_X = 240.f;
const float GRID_SPACING_Y = 170.f;

// File Paths (MUST match your asset names and locations)
const std::string FONT_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\OpenSans-Bold.ttf"; // Replace with your font file
const std::string BG_TEXTURE_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\background.jfif";
const std::string MOLE_HOLE_TEXTURE_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\hole-removebg-preview.png";
const std::string MOLE_UP_TEXTURE_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\mole.png";
const std::string MOLE_HIT_TEXTURE_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\mole-whacked.png";
const std::string HAMMER_TEXTURE_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\hammer.png"; // Optional cursor
const std::string HIGHSCORE_FILE = "Highscore.txt";

const std::string SOUND_WHACK_PATH ="C:\\Users\\HP\\Desktop\\taha\\images\\whack.wav";
const std::string SOUND_BGMUSIC_PATH = "C:\\Users\\HP\\Desktop\\taha\\images\\background_music.wav";
#endif // DEFINES_H