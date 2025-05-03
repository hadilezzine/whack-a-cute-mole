#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

// Define a named constant for the color
const sf::Color DarkDirtBrown(101, 67, 33);

void Game::centerOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

Game::Game() :
    mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Close),
    mCurrentState(GameState::WELCOME),
    mScore(0),
    mHighScore(0),
    mTimeRemaining(GAME_DURATION),
    mNextMoleSpawnTime(sf::Time::Zero),
    mRandomEngine(std::random_device{}()),
    mIntDistribution(0, MOLE_COUNT - 1),
    mIsHammerVisible(false)
{
    mWindow.setFramerateLimit(60);
    mWindow.setMouseCursorVisible(true);
    loadAssets();
    setupTexts();
    setupMoles();
    setupHammer();
    loadHighScore();
    changeState(GameState::WELCOME);
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TIME_PER_FRAME) {
            timeSinceLastUpdate -= TIME_PER_FRAME;
            processEvents();
            update(TIME_PER_FRAME);
        }
        render();
    }
}

// Event handling
void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                mWindow.close();
                break;

            case sf::Event::KeyPressed:
                 // Allow starting/restarting with Enter key
                 if (event.key.code == sf::Keyboard::Enter) {
                    if (mCurrentState == GameState::WELCOME) {
                        changeState(GameState::PLAYING);
                    } else if (mCurrentState == GameState::GAME_OVER) {
                        changeState(GameState::WELCOME);
                    }
                 }
                 // handlePlayerInput(event.key.code, true); // If other keys needed
                break;

             // case sf::Event::KeyReleased:
             //    handlePlayerInput(event.key.code, false);
             //    break;

            case sf::Event::MouseButtonPressed:
                handleMouseInput(event.mouseButton.button, true, {event.mouseButton.x, event.mouseButton.y});
                break;

             // case sf::Event::MouseButtonReleased:
             //    handleMouseInput(event.mouseButton.button, false, {event.mouseButton.x, event.mouseButton.y});
             //    break;

            case sf::Event::MouseMoved:
                 if (mIsHammerVisible) {
                    mHammerSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));
                 }
                 break;

            default:
                break;
        }
    }
}

// Update game logic based on state
void Game::update(sf::Time dt) {
    switch (mCurrentState) {
        case GameState::WELCOME:
            updateWelcome(dt);
            break;
        case GameState::PLAYING:
            updatePlaying(dt);
            break;
        case GameState::GAME_OVER:
            updateGameOver(dt);
            break;
    }
     // Update hammer position if it's visible regardless of state (optional)
     if (mIsHammerVisible) {
        mHammerSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));
     }
}

// Render graphics based on state
void Game::render() {
    mWindow.clear(sf::Color(180, 220, 255)); // Light blue clear color, or draw background

    mWindow.draw(mBackgroundSprite); // Draw background first

    switch (mCurrentState) {
        case GameState::WELCOME:
            renderWelcome();
            break;
        case GameState::PLAYING:
            renderPlaying();
            break;
        case GameState::GAME_OVER:
            renderGameOver();
            break;
    }

    // Draw hammer on top if visible
    if (mIsHammerVisible) {
        mWindow.draw(mHammerSprite);
    }

    mWindow.display();
}

// --- Asset Loading & Setup ---

void Game::loadAssets() {
    if (!mFont.loadFromFile("C:\\Users\\HP\\Desktop\\taha\\images\\PixelPurl.ttf")) {
        std::cerr << "Error loading font: " << "C:\\Users\\HP\\Desktop\\taha\\images\\PixelPurl.ttf" << std::endl;
        mWindow.close(); // Can't proceed without font
    }
    if (!mBgTexture.loadFromFile("C:\\Users\\HP\\Desktop\\taha\\images\\background.jfif")) {
        std::cerr << "Error loading texture: " << "C:\\Users\\HP\\Desktop\\taha\\images\\background.jfif" << std::endl;
        // Maybe use a default color background instead of closing
    }
    mBackgroundSprite.setTexture(mBgTexture);

    // Scale background to exactly match window size
    float scaleX = static_cast<float>(WINDOW_WIDTH) / mBgTexture.getSize().x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / mBgTexture.getSize().y;
    mBackgroundSprite.setScale(scaleX, scaleY);

    if (!mHoleTexture.loadFromFile("C:\\Users\\HP\\Desktop\\taha\\images\\hole-removebg-preview.png")) {
        std::cerr << "Error loading texture: " << "C:\\Users\\HP\\Desktop\\taha\\images\\hole-removebg-preview.png" << std::endl;
        mWindow.close();
    }
    if (!mMoleUpTexture.loadFromFile("C:\\Users\\HP\\Desktop\\taha\\images\\mole.png")) {
        std::cerr << "Error loading texture: " << "C:\\Users\\HP\\Desktop\\taha\\images\\mole.png" << std::endl;
        mWindow.close();
    }
    if (!mMoleHitTexture.loadFromFile("C:\\Users\\HP\\Desktop\\taha\\images\\mole-whacked.png")) {
        std::cerr << "Error loading texture: " << "C:\\Users\\HP\\Desktop\\taha\\images\\mole-whacked.png" << std::endl;
        mWindow.close();
    }
    if (!mHammerTexture.loadFromFile("C:\\Users\\HP\\Desktop\\taha\\images\\hammer.png")) {
        std::cerr << "Warning: Could not load hammer texture: " << "C:\\Users\\HP\\Desktop\\taha\\images\\hammer.png" << std::endl;
        mIsHammerVisible = false; // Disable custom cursor if texture fails
    } else {
        mIsHammerVisible = true; // Enable if loaded
    }
    if (!mWhackBuffer.loadFromFile(SOUND_WHACK_PATH)) {
        std::cerr << "Failed to load whack sound!" << std::endl;
    }
    mWhackSound.setBuffer(mWhackBuffer);
    mWhackSound.setVolume(70);  // Set volume (0-100)

    if (!mBackgroundMusic.openFromFile(SOUND_BGMUSIC_PATH)) {
        std::cerr << "Failed to load background music!" << std::endl;
    }
    mBackgroundMusic.setVolume(30);  // Set volume (0-100)
    mBackgroundMusic.setLoop(true);
    mBackgroundMusic.setLoop(true);

}

void Game::setupTexts() {
    // --- Generic Setup ---
    auto setupTextStyle = [&](sf::Text& text, unsigned int size, sf::Color color = sf::Color::Black) {
        text.setFont(mFont);
        text.setCharacterSize(size);
        text.setFillColor(color);
        // text.setOutlineColor(sf::Color::Black); // Optional outline
        // text.setOutlineThickness(1.f);
    };

    // Center Origin Helper
    auto centerOrigin = [](sf::Text& text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    };

    // --- Specific Texts ---
    setupTextStyle(mWelcomeText,100, sf::Color(0,0, 0)); // Dark Green
    mWelcomeText.setString("Whack-a-Cute-Mole!");
    centerOrigin(mWelcomeText);
    mWelcomeText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f);

    setupTextStyle(mInstructionText, 45, sf::Color::White); // White
    mInstructionText.setString("Click the Moles!\nPress Enter or Click to Start");
    centerOrigin(mInstructionText);
    mInstructionText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    setupTextStyle(mScoreText, 30, sf::Color::White);
    mScoreText.setPosition(20.f, 10.f); // Top Left

    setupTextStyle(mTimerText, 30, sf::Color::White);
    // Position dynamically based on window width
    // We'll set string later, need size info first
    mTimerText.setString("Time: 00");
    sf::FloatRect timerBounds = mTimerText.getLocalBounds();
    mTimerText.setPosition(WINDOW_WIDTH - timerBounds.width - 90.f, 10.f); // Top Right (approx)

    setupTextStyle(mGameOverText,160, sf::Color::Red);
    mGameOverText.setString("GAME OVER!");
    centerOrigin(mGameOverText);
    mGameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f);

    setupTextStyle(mFinalScoreText, 50, sf::Color::White);
    centerOrigin(mFinalScoreText); // Origin set now, position later
    mFinalScoreText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    setupTextStyle(mHighScoreText, 50, DarkDirtBrown); // Dark Dirt Brown
    centerOrigin(mHighScoreText);
    mHighScoreText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 50.f);

    setupTextStyle(mRestartText, 50, sf::Color::White);
    mRestartText.setString("Press Enter or Click to Return to Menu");
    centerOrigin(mRestartText);
    mRestartText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 100.f);
}

void Game::setupMoles() {
    mMoles.resize(MOLE_COUNT);
    
    // Calculer la largeur et hauteur totale de la grille
    float totalGridWidth = (GRID_COLS - 1) * GRID_SPACING_X + mHoleTexture.getSize().x * 0.5f;
    float totalGridHeight = (GRID_ROWS - 1) * GRID_SPACING_Y + mHoleTexture.getSize().y * 0.5f;
    
    // Calculer les positions de départ pour centrer la grille
    float startX = (WINDOW_WIDTH - totalGridWidth) / 2.f;
    float startY = (WINDOW_HEIGHT - totalGridHeight) / 2.f;
    
    for (int r = 0; r < GRID_ROWS; ++r) {
        for (int c = 0; c < GRID_COLS; ++c) {
            int index = r * GRID_COLS + c;
            sf::Vector2f pos(startX + c * GRID_SPACING_X,
                           startY + r * GRID_SPACING_Y);
            mMoles[index].setup(mHoleTexture, mMoleUpTexture, mMoleHitTexture, pos);
        }
    }
}

void Game::setupHammer() {
    if (mIsHammerVisible) {
        mHammerSprite.setTexture(mHammerTexture);
        mHammerSprite.setScale(0.5f, 0.5f);
        mHammerSprite.setOrigin(mHammerTexture.getSize().x * 0.7f, 
                              mHammerTexture.getSize().y * 0.2f);
    }
}




void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    
}

void Game::handleMouseInput(sf::Mouse::Button button, bool isPressed, sf::Vector2i position) {
    if (button == sf::Mouse::Left && isPressed) {
        sf::Vector2f worldPos = mWindow.mapPixelToCoords(position);

        switch (mCurrentState) {
            case GameState::WELCOME:
                changeState(GameState::PLAYING);
                break;

            case GameState::PLAYING:
                 for (auto& mole : mMoles) {
                    if (mole.isClicked(worldPos)) {
                        mole.whack();
                        mScore++;
                        mWhackSound.play();  
                        break;
                    }
                 }
                 break;

            case GameState::GAME_OVER:
                changeState(GameState::WELCOME);
                break;
        }
    }
}


void Game::changeState(GameState newState) {
    if (mCurrentState == GameState::PLAYING) {
        mBackgroundMusic.stop();
    }

    mCurrentState = newState;

    switch (mCurrentState) {
        case GameState::WELCOME:
             mWindow.setMouseCursorVisible(true);
             mIsHammerVisible = false;
             mHighScoreText.setString("High Score: " + std::to_string(mHighScore));
             centerOrigin(mHighScoreText);
             mHighScoreText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 50.f);
            break;

        case GameState::PLAYING:
            resetGame();
            if (mHammerTexture.getSize().x > 0) {
                mWindow.setMouseCursorVisible(false);
                mIsHammerVisible = true;
            } else {
                 mWindow.setMouseCursorVisible(true);
                 mIsHammerVisible = false;
            }
            mBackgroundMusic.play();  
            break;

        case GameState::GAME_OVER:
             mWindow.setMouseCursorVisible(true);
             mIsHammerVisible = false;
             mFinalScoreText.setString("Your Score: " + std::to_string(mScore));
             centerOrigin(mFinalScoreText);
             mFinalScoreText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
                mHighScoreText.setString("High Score: " + std::to_string(mHighScore));
            break;
    }
}
void Game::resetGame() {
    mScore = 0;
    mTimeRemaining = GAME_DURATION;
    mGameClock.restart();
    mMoleSpawnClock.restart();
    mNextMoleSpawnTime = randomTime(MOLE_SPAWN_TIME_MIN, MOLE_SPAWN_TIME_MAX);

   
    for (auto& mole : mMoles) {
        mole.hide();
    }
}




void Game::updateWelcome(sf::Time dt) {
    
}

void Game::updatePlaying(sf::Time dt) {
    mTimeRemaining -= dt;
    if (mTimeRemaining <= sf::Time::Zero) {
        mTimeRemaining = sf::Time::Zero;
        changeState(GameState::GAME_OVER);
        return;
    }

    mScoreText.setString("Score: " + std::to_string(mScore));
    int seconds = static_cast<int>(mTimeRemaining.asSeconds());
    mTimerText.setString(std::string("Time: ") + (seconds < 10 ? "0" : "") + std::to_string(seconds));
    sf::FloatRect timerBounds = mTimerText.getLocalBounds();
    mTimerText.setPosition(WINDOW_WIDTH - timerBounds.width - 90.f, 10.f);

    
    for (auto& mole : mMoles) {
        mole.update(dt);
    }

    if (mMoleSpawnClock.getElapsedTime() >= mNextMoleSpawnTime) {
        mMoleSpawnClock.restart();
        mNextMoleSpawnTime = randomTime(MOLE_SPAWN_TIME_MIN, MOLE_SPAWN_TIME_MAX);

       
        int startIndex = mIntDistribution(mRandomEngine);
        int moleIndex = -1;

        for(int i = 0; i < MOLE_COUNT; ++i) {
            int current_index = (startIndex + i) % MOLE_COUNT;
            if (!mMoles[current_index].isActive() && mMoles[current_index].isHidden()) {
                moleIndex = current_index;
                break;
            }
        }

        if (moleIndex != -1) {
            sf::Time timeUp = randomTime(MOLE_TIME_UP_MIN, MOLE_TIME_UP_MAX);
            mMoles[moleIndex].popUp(timeUp);
        }
    }
}

void Game::updateGameOver(sf::Time dt) {
}



void Game::renderWelcome() {
    mWindow.draw(mWelcomeText);
    mWindow.draw(mInstructionText);
     mWindow.draw(mHighScoreText);
}

void Game::renderPlaying() {
    for (auto& mole : mMoles) {
        mole.draw(mWindow);
    }

    mWindow.draw(mScoreText);
    mWindow.draw(mTimerText);
}

void Game::renderGameOver() {
    mWindow.draw(mGameOverText);
    mWindow.draw(mFinalScoreText);
    mWindow.draw(mHighScoreText);
    mWindow.draw(mRestartText);
}


void Game::loadHighScore() {
    std::ifstream file("Highscore.txt");
    if (file.is_open()) {
        file >> mHighScore;
        if (file.fail()) { 
             std::cerr << "Warning: Failed to read high score from " << "Highscore.txt" << ". Resetting to 0." << std::endl;
             mHighScore = 0;
        }
        file.close();
    } else {
        std::cerr << "Warning: Could not open " << "Highscore.txt" << " for reading. High score starts at 0." << std::endl;
        mHighScore = 0; 
    }
}

void Game::saveHighScore() {
    std::ofstream file("Highscore.txt");
    if (file.is_open()) {
        file << mHighScore;
        file.close();
        if (file.fail()) {
            std::cerr << "Error: Failed to write high score to " << "Highscore.txt"<< "." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not open " << "Highscore.txt" << " for writing." << std::endl;
    }
}

bool Game::updateHighScore() {
    if (mScore > mHighScore) {
        mHighScore = mScore;
        return true; 
    }
    return false; 
}

float Game::randomFloat(float min, float max) {
     std::uniform_real_distribution<float> dist(min, max);
     return dist(mRandomEngine);
}

sf::Time Game::randomTime(sf::Time min, sf::Time max) {
    float seconds = randomFloat(min.asSeconds(), max.asSeconds());
    return sf::seconds(seconds);
}