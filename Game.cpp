#include "Game.hpp"

// Initializer functions
void Game::initWindow() {
    // Creates a SFML window using options from a window.ini file
    this->window = new sf::RenderWindow(sf::VideoMode(this->SCREEN_WIDTH, this->SCREEN_HEIGHT), "Shooting Gallery Zahkthar Edition");
    //this->window->setFramerateLimit(this->FRAMERATE_LIMIT);
    this->window->setMouseCursorVisible(false);
}

void Game::initVariables() {
    srand((unsigned int)time(0));

    this->currentMouseState = false;
    this->oldMouseState = false;

    // el_famoso_musique_de_fond.ogg
    if(!this->backgroundMusicBuffer.loadFromFile("ressources/Sounds/el_famoso_musique_de_fond.ogg")) {
        this->window->close();
    }
    this->backgroundMusic.setBuffer(backgroundMusicBuffer);
    this->backgroundMusic.setVolume(1);
    this->backgroundMusic.setLoop(true);
    this->backgroundMusic.play();

    // bg_blue.png -> 256 * 256
    if(!this->backgroundTexture.loadFromFile("ressources/Stall/bg_blue.png", sf::IntRect(0, 0, 256, 256))) {
        this->window->close();
    }
    this->backgroundSprite.setTexture(this->backgroundTexture);
    this->backgroundSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
    this->backgroundSprite.scale(this->SCREEN_WIDTH / 256.0f, this->SCREEN_HEIGHT / 256.0f);

    // crosshair_outline_small.png -> 46 * 46
    if(!this->crosshairTexture.loadFromFile("ressources/HUD/crosshair_outline_small.png", sf::IntRect(0, 0, 46, 46))) {
        this->window->close();
    }
    this->crosshairSprite.setTexture(this->crosshairTexture);
    this->crosshairSprite.setOrigin(23, 23); // 23 = 46 / 2
    this->crosshairSprite.setPosition(sf::Vector2f(this->SCREEN_WIDTH / 2.0f, this->SCREEN_HEIGHT / 2.0f));

    // shot_blue_small.png -> 20 * 20
    if(!this->shotTexture.loadFromFile("ressources/Objects/shot_blue_small.png", sf::IntRect(0, 0, 20, 20))) {
        this->window->close();
    }
    this->shotSprite.setTexture(this->shotTexture);
    this->shotSprite.setOrigin(10, 10); // 10 = 20 / 2
    this->shotSprite.setPosition(sf::Vector2f(this->SCREEN_WIDTH / 2.0f, this->SCREEN_HEIGHT / 2.0f));

    // shot.ogg
    if(!this->shotSoundBuffer.loadFromFile("ressources/Sounds/shot.ogg")) {
        this->window->close();
    }
    this->shotSound.setBuffer(shotSoundBuffer);
    this->shotSound.setVolume(5);
    this->shotSound.play();

    // target_colored.png -> 128 * 128
    if(!this->targetTexture.loadFromFile("ressources/Objects/target_colored.png", sf::IntRect(0, 0, 128, 128))) {
        this->window->close();
    }
    this->targetSprite.setTexture(this->targetTexture);
    this->targetSprite.setOrigin(64, 64); // 64 = 128 / 2
    this->targetSprite.setPosition(sf::Vector2f(this->SCREEN_WIDTH / 2.0f, this->SCREEN_HEIGHT / 2.0f));

    this->userScore = 0;
    this->userHighScore = 0;

    // MexicanTequila.ttf
    if(!this->scoreFont.loadFromFile("ressources/Fonts/MexicanTequila.ttf")) {
        this->window->close();
    }
    this->highscoreText.setFont(this->scoreFont);
    this->highscoreText.setString("Highscore : 0");
    this->highscoreText.setColor(sf::Color::Black);
    this->highscoreText.setPosition(0, 0);

    this->scoreText.setFont(this->scoreFont);
    this->scoreText.setString("Score : 0");
    this->scoreText.setColor(sf::Color::Black);
    this->scoreText.setPosition(0.8*this->SCREEN_WIDTH, 0);
}

// Constructors/Destructors
Game::Game() {
    this->initVariables();
    this->initWindow();
}

Game::~Game() {
    delete this->window;
}

// Functions
void Game::updateSFMLEvents() {
    while(this->window->pollEvent(this->sfEvent)) {
        if(this->sfEvent.type == sf::Event::Closed) {
            this->window->close();
        }
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->currentMouseState = true;
    } else {
        this->currentMouseState = false;
    }
}

void Game::update() {
    this->updateSFMLEvents();

    this->crosshairSprite.setPosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
        this->backgroundMusicVolume += 1;
        this->backgroundMusic.setVolume(this->backgroundMusicVolume);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
        if(backgroundMusicVolume - 1 >= 0) {
            this->backgroundMusicVolume -= 1;
        }
        this->backgroundMusic.setVolume(this->backgroundMusicVolume);
    }

    if(this->currentMouseState == true && this->oldMouseState == false) {
        this->isShotVisible = true;
        this->shotSound.play();
        this->shotSprite.setPosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

        if((sf::Mouse::getPosition(*window).x >= targetSprite.getPosition().x - 64) &&
           (sf::Mouse::getPosition(*window).x <= targetSprite.getPosition().x + 64) &&
           (sf::Mouse::getPosition(*window).y >= targetSprite.getPosition().y - 64) &&
           (sf::Mouse::getPosition(*window).y <= targetSprite.getPosition().y + 64)) {
                targetSprite.setPosition(std::rand()%(this->SCREEN_WIDTH-128)+64, std::rand()%(this->SCREEN_HEIGHT-128)+64);
                this->userScore += 1;
        } else { // Si le player rate la cible
            this->userScore = 0;
        }

        if(this->userScore > this->userHighScore) {
            this->userHighScore = this->userScore;
        }
    }

    this->highscoreText.setPosition(0.01f*this->SCREEN_WIDTH, 0);
    this->scoreText.setPosition(0.85f*this->SCREEN_WIDTH, 0);

    this->scoreText.setString("Score : " + std::to_string(this->userScore));
    this->highscoreText.setString("Highscore : " + std::to_string(this->userHighScore));

    this->oldMouseState = this->currentMouseState; // Pour éviter qu'il y ait 60 clics par secondes
}

void Game::render() {
    this->window->clear();

    this->window->draw(this->backgroundSprite);

    this->window->draw(this->targetSprite);

    if(this->isShotVisible == true) {
        this->window->draw(this->shotSprite);
    }

    this->window->draw(this->crosshairSprite);

    this->window->draw(this->scoreText);
    this->window->draw(this->highscoreText);

    this->window->display();
}

void Game::run() {
    while(this->window->isOpen()) {
        this->update();
        this->render();
    }
}
