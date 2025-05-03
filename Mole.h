#ifndef MOLE_H
#define MOLE_H

#include <SFML/Graphics.hpp>
#include "Defines.h" // Include our defines

enum class MoleState {
    HIDDEN,
    RISING, // Optional state for animation
    UP,
    HIT,
    HIDING  // Optional state for animation
};

class Mole {
public:
    Mole();

    void setup(const sf::Texture& texHole, const sf::Texture& texUp, const sf::Texture& texHit, const sf::Vector2f& pos);
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window);

    bool isClicked(const sf::Vector2f& mousePos);
    void whack(); 
    void popUp(sf::Time duration); 
    void hide();   // Makes the mole go down immediately

    bool isActive() const; // Is the mole currently hittable?
    bool isHidden() const { return state == MoleState::HIDDEN; } // New method

private:
    sf::Sprite spriteHole;
    sf::Sprite spriteMole;
    sf::Texture const* textureUp; // Store pointers to textures managed elsewhere
    sf::Texture const* textureHit;

    sf::Vector2f position;
    MoleState state;
    sf::Time timeToStayActive;
    sf::Time activeTimer;
    sf::Time hitDisplayTimer; // How long to show the "hit" sprite
    const sf::Time hitDisplayDuration = sf::seconds(0.25f);

    void setState(MoleState newState);
};

#endif // MOLE_H