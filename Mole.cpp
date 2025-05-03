#include "Mole.h"
#include <iostream> // For basic error checking output

Mole::Mole() :
    textureUp(nullptr),
    textureHit(nullptr),
    position(0.f, 0.f),
    state(MoleState::HIDDEN),
    timeToStayActive(sf::Time::Zero),
    activeTimer(sf::Time::Zero),
    hitDisplayTimer(sf::Time::Zero)
{}

void Mole::setup(const sf::Texture& texHole, const sf::Texture& texUp, const sf::Texture& texHit, const sf::Vector2f& pos) {
    position = pos;

    spriteHole.setTexture(texHole);
    spriteHole.setScale(0.65f, 0.65f);
    spriteHole.setPosition(position);

    textureUp = &texUp;
    textureHit = &texHit;

    spriteMole.setTexture(*textureUp);
    // Scale down the mole sprite (adjust 0.5f to your desired scale)
    spriteMole.setScale(0.45f, 0.45f);  // Taille de la taupe

    // Ajustez l'origine pour un ancrage cohérent
    sf::FloatRect moleBounds = spriteMole.getLocalBounds();
    spriteMole.setOrigin(moleBounds.width / 2.f, moleBounds.height * 0.65f); // Ajustez l'origine pour le bas de la taupe
    // Positionnez la taupe au centre du trou   

    // Position ajustée (+ testez /1.8f au lieu de /1.5f)
    spriteMole.setPosition(
        position.x + spriteHole.getGlobalBounds().width / 2.f,
        position.y + spriteHole.getGlobalBounds().height / 1.8f  // Ajustez ce diviseur
    );

    setState(MoleState::HIDDEN);
}
void Mole::update(sf::Time dt) {
    switch (state) {
        case MoleState::UP:
            activeTimer += dt;
            if (activeTimer >= timeToStayActive) {
                setState(MoleState::HIDDEN); 
            }
            break;

        case MoleState::HIT:
            hitDisplayTimer += dt;
            if (hitDisplayTimer >= hitDisplayDuration) {
                setState(MoleState::HIDDEN); 
            }
            break;

        case MoleState::HIDDEN:
        default:
            break;
    }
}

void Mole::draw(sf::RenderWindow& window) {
    // Always draw the hole
    window.draw(spriteHole);

    // Draw the mole only if it's not hidden
    if (state == MoleState::UP || state == MoleState::HIT) {
         // Add rising/hiding states if implemented
        window.draw(spriteMole);
    }
}

bool Mole::isClicked(const sf::Vector2f& mousePos) {
    if (state == MoleState::UP) {
        
        sf::FloatRect hitbox = spriteMole.getGlobalBounds();
        hitbox.height *= 0.8f;  
        hitbox.top += hitbox.height * 0.2f; 
        
     
        return hitbox.contains(mousePos);
    }
    return false;
}

void Mole::whack() {
    if (state == MoleState::UP) {
        setState(MoleState::HIT);
   
    }
}

void Mole::popUp(sf::Time duration) {
    if (state == MoleState::HIDDEN) {
        timeToStayActive = duration;
        setState(MoleState::UP);
        activeTimer = sf::Time::Zero;
        hitDisplayTimer = sf::Time::Zero;
        spriteMole.setTexture(*textureUp, true); 
    }
}

void Mole::hide() {
    setState(MoleState::HIDDEN);
}


bool Mole::isActive() const {

    return state == MoleState::UP;
}

void Mole::setState(MoleState newState) {
     state = newState;
     switch(state) {
         case MoleState::HIDDEN:
            break;
         case MoleState::UP:
            spriteMole.setTexture(*textureUp, true);
            activeTimer = sf::Time::Zero;
            break;
         case MoleState::HIT:
            spriteMole.setTexture(*textureHit, true);
            hitDisplayTimer = sf::Time::Zero;
            break;
     }
}