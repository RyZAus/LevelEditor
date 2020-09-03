#pragma once
#include "Editor.h"
#include "MenuHeader.h"
struct Collision
{
	bool hit;
	sf::Vector2f dir;
};
class Player : public Actor
{
public:
	Player();
	int lives;
	int coins;
	bool isDead;
	//collision hitbox, check next loc for collision
	sf::FloatRect nextRect;
	//if we can move, this is the position we move to
	sf::Vector2f nextPos;
	//smooth physics
	sf::Vector2f veloctiy;
	//Jumpy boi
	bool isGrounded;
	float jumpSpeed;
	float speed;

	Collision CollisionCheck(sf::FloatRect other);

	void Refresh();
	void Respawn();
	bool isAnyKeyPressed();
	sf::Vector2f startPos;

	virtual sf::Vector2f getPosition();
	virtual void setPosition(sf::Vector2f p);
};
class Enemy : public Actor
{
public:
	Enemy();
	bool isDead;
	void Killed();
};
class CounterButs : public sf::Drawable, public sf::Transformable
{
private:
	//virtual allows us to override an exsisting function / allows us to edit previous things like "void draw" which have nothing in them
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(spriteB, states);
		target.draw(text, states);
	}
public:
	int textSize = 30;
	float rectOutline = 1;
	sf::RectangleShape rect;
	sf::Texture gameButText;
	//sets our sprite to be a sprite variable we can use
	sf::Sprite spriteB;
	sf::Font font;
	sf::Text text;
	sf::FloatRect rBounds;
	sf::FloatRect sBounds;
	//init
	CounterButs()
	{
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setCharacterSize(textSize);
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Regular);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(rectOutline);
		//set visuals
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(rectOutline);

		//If statement to determine if our texture is loading or not
		if (!gameButText.loadFromFile("PNG/button.png"))
		{
			std::cout << "Error with loading" << std::endl;
		}
		else
		{
			std::cout << "If all else fails" << std::endl;
		}
		spriteB.setTexture(gameButText);
		sf::Vector2f targetSize(200.0f, 65.0f);
		spriteB.setScale(targetSize.x / spriteB.getLocalBounds().width, targetSize.y / spriteB.getLocalBounds().height);
	}
};
class GameCounters
{
public:
	CounterButs coinsCounter;
	CounterButs livesCounter;
	GameCounters()
	{
		
		//Edited from 6 to 10 to prevent words being cut off on the main menu
		coinsCounter.rect.setSize(sf::Vector2f(coinsCounter.text.getGlobalBounds().width + 10, coinsCounter.text.getGlobalBounds().height + 10));
		coinsCounter.rect.setPosition(sf::Vector2f(1300 - (1025 / 2 - (coinsCounter.rect.getSize().x / 2)), 350 - (650 / 2 - (coinsCounter.rect.getSize().y / 2))));
		coinsCounter.text.setPosition(coinsCounter.rect.getPosition().x + (coinsCounter.rect.getSize().x / 400000), coinsCounter.rect.getPosition().y);
		//Sets our sprite position for level editor
		coinsCounter.spriteB.setPosition(coinsCounter.rect.getPosition().x / 1.05, coinsCounter.rect.getPosition().y / 1.5);
		//game button
		//Edited from 6 to 10 to prevent words being cut off on the main menu
		livesCounter.rect.setSize(sf::Vector2f(livesCounter.text.getGlobalBounds().width + 9, livesCounter.text.getGlobalBounds().height + 10));
		livesCounter.rect.setPosition(sf::Vector2f(1200 - (1025 / 2 + 150 - (livesCounter.rect.getSize().x / 2)), 350 - (650 / 2 - (livesCounter.rect.getSize().y / 2))));
		livesCounter.text.setPosition(livesCounter.rect.getPosition().x + (livesCounter.rect.getSize().x / 400000), livesCounter.rect.getPosition().y);
		//Sets our sprite position for game button
		livesCounter.spriteB.setPosition(livesCounter.rect.getPosition().x / 1.1, livesCounter.rect.getPosition().y / 1.5);
	}
};
class GameClass
{
public:
	GameClass();
	bool Start(MainRenderWindow& mainWindow);
	void Update(MainRenderWindow& mainWindow, MainMenu& mainMenu);
	bool gameActive = false;
	static const int x = 30;
	static const int y = 20;
	Player player;
	GameCounters gameCounters;
	//enemy array called enemies // std::vector<empty> enemies;
	void SaveScore();
	void ResetLevel();
	void GameOver();

	Tile** tile = new Tile * [x];
	void LoadLevel(std::string levelName, Tile** incTile);
	//delta time stuff
	sf::Clock clock;
	float deltaTime;
	//physics
	float gravity;
	float friction;
	int coins;
	sf::SoundBuffer deathSB;
	sf::SoundBuffer coinSB;
	sf::SoundBuffer killSB;
	sf::Sound sound;
};
int sign(int x);
int sign(float x);