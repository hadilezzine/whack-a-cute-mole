#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Mole.h"
#include "Defines.h"
#include <SFML/Audio.hpp>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    void centerOrigin(sf::Text& text); // Added this

    // State-specific functions
    void updateWelcome(sf::Time dt);
    void updatePlaying(sf::Time dt);
    void updateGameOver(sf::Time dt);
    void renderWelcome();
    void renderPlaying();
    void renderGameOver();

    // Helper functions
    void loadAssets();
    void setupTexts();
    void setupMoles();
    void setupHammer();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void handleMouseInput(sf::Mouse::Button button, bool isPressed, sf::Vector2i position);
    void changeState(GameState newState);
    void resetGame();
    void loadHighScore();
    void saveHighScore();
    bool updateHighScore();
    float randomFloat(float min, float max);
    sf::Time randomTime(sf::Time min, sf::Time max);

    // Window and state
    sf::RenderWindow mWindow;
    GameState mCurrentState;

    // Game objects
    std::vector<Mole> mMoles;
    sf::Sprite mBackgroundSprite;
    sf::Sprite mHammerSprite;
    bool mIsHammerVisible;

    // Text objects
    sf::Text mWelcomeText;
    sf::Text mInstructionText;
    sf::Text mScoreText;
    sf::Text mTimerText;
    sf::Text mGameOverText;
    sf::Text mFinalScoreText;
    sf::Text mHighScoreText;
    sf::Text mRestartText;

    // Resources
    sf::Font mFont;
    sf::Texture mBgTexture;
    sf::Texture mHoleTexture;
    sf::Texture mMoleUpTexture;
    sf::Texture mMoleHitTexture;
    sf::Texture mHammerTexture;

    // Game variables
    int mScore;
    int mHighScore;
    sf::Time mTimeRemaining;
    sf::Clock mGameClock;
    sf::Clock mMoleSpawnClock;
    sf::Time mNextMoleSpawnTime;

    // Random number generation
    std::default_random_engine mRandomEngine;
    std::uniform_int_distribution<int> mIntDistribution;

    sf::SoundBuffer mWhackBuffer;
    sf::SoundBuffer mMusicBuffer;
    
    sf::Sound mWhackSound;
    sf::Music mBackgroundMusic;
};

#endif // GAME_H