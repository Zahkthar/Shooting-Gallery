#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"

class Game {

private:
    // Variables
    sf::RenderWindow *window;
    sf::Event sfEvent;

    bool currentMouseState;
    bool oldMouseState;

    const int SCREEN_WIDTH = 1080;
    const int SCREEN_HEIGHT = 720;
    const int FRAMERATE_LIMIT = 60;

    sf::SoundBuffer backgroundMusicBuffer;
    sf::Sound backgroundMusic;
    int backgroundMusicVolume = 1;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture crosshairTexture;
    sf::Sprite crosshairSprite;

    sf::Texture shotTexture;
    sf::Sprite shotSprite;
    bool isShotVisible = false;

    sf::SoundBuffer shotSoundBuffer;
    sf::Sound shotSound;

    sf::Texture targetTexture;
    sf::Sprite targetSprite;
    bool isTargetBeenHit = false;

    int userScore = 0;
    int userHighScore = 0;

    sf::Font scoreFont;
    sf::Text scoreText;
    sf::Text highscoreText;

    // Initialization
    void initWindow();
    void initVariables();
public:
    // Constructors/Destructors
    Game();
    ~Game();

    // Functions
    void updateSFMLEvents();
    void update();
    void render();
    void run();
};

#endif
