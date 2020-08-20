#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include "WindowHeader.h"

class Mode 
{
public:
	enum MenuMode
	{
		MainMenu,
		Editor,
		EditorLevelSelect,
		Game,
		GameLevelSelect,
		GameOver
	};
	//keep track of where we are and default to the main menu
	MenuMode currentMode = MainMenu;
};

class EditorLevelSelect {};

class GameLevelSelect {};

class MenuButton : public sf::Drawable, public sf::Transformable
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
	MenuButton()
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
		spriteB.setScale(targetSize.x / spriteB.getLocalBounds().width,	targetSize.y / spriteB.getLocalBounds().height);
	}
	bool mouseOver(sf::Vector2f windpos)
	{
		rBounds = rect.getGlobalBounds();
		if (rBounds.contains(windpos.x, windpos.y))
		{
			rect.setOutlineThickness(rectOutline * 2);
			sf::Vector2f targetSize(210.0f, 70.0f);
			spriteB.setScale(targetSize.x / spriteB.getLocalBounds().width, targetSize.y / spriteB.getLocalBounds().height);
			return true;
		}
		else
		{
			rect.setOutlineThickness(rectOutline);
			sf::Vector2f targetSize(200.0f, 65.0f);
			spriteB.setScale(targetSize.x / spriteB.getLocalBounds().width, targetSize.y / spriteB.getLocalBounds().height);
			return false;
		}
	}
	void checkClick(std::function<void()> f, sf::Vector2f windpos)
	{
		if (mouseOver(windpos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			f();
		}
	}
};

class MainMenu
{
public:
	MenuButton levelEditorButton;
	MenuButton gameButton;
	Mode myMode;
	MainMenu()
	{
		levelEditorButton.text.setString("Level Editor");
		//Edited from 6 to 10 to prevent words being cut off on the main menu
		levelEditorButton.rect.setSize(sf::Vector2f(levelEditorButton.text.getGlobalBounds().width + 10, levelEditorButton.text.getGlobalBounds().height + 10));
		levelEditorButton.rect.setPosition(sf::Vector2f(1025 - (1025 / 2 - (levelEditorButton.rect.getSize().x / 2)), 585 - (650 / 2 - (levelEditorButton.rect.getSize().y / 2))));
		levelEditorButton.text.setPosition(levelEditorButton.rect.getPosition().x + (levelEditorButton.rect.getSize().x / 400000), levelEditorButton.rect.getPosition().y);
		//Sets our sprite position for level editor
		levelEditorButton.spriteB.setPosition(levelEditorButton.rect.getPosition().x / 1.05, levelEditorButton.rect.getPosition().y / 1.05);
		//game button
		gameButton.text.setString("Play Game");
		//Edited from 6 to 10 to prevent words being cut off on the main menu
		gameButton.rect.setSize(sf::Vector2f(gameButton.text.getGlobalBounds().width + 9, gameButton.text.getGlobalBounds().height + 10));
		gameButton.rect.setPosition(sf::Vector2f(900 - (1025 / 2 + 150 - (gameButton.rect.getSize().x / 2)), 580 - (650 / 2 - (gameButton.rect.getSize().y / 2))));
		gameButton.text.setPosition(gameButton.rect.getPosition().x + (gameButton.rect.getSize().x / 400000), gameButton.rect.getPosition().y);
		//Sets our sprite position for game button
		gameButton.spriteB.setPosition(gameButton.rect.getPosition().x / 1.1, gameButton.rect.getPosition().y / 1.05);
	}
	sf::Vector2f worldPos;
	bool Start();
	void Update(MainRenderWindow& mainWindow);

	bool menuActive = true;

	void ChangeMode(Mode::MenuMode m)
	{
		myMode.currentMode = m;
		menuActive = false;
	}
};

class GameOver
{
public:
	MenuButton goScreen;
	Mode goMode;
	GameOver()
	{
		goScreen.text.setString("GAME OVER");
		goScreen.rect.setSize(sf::Vector2f(goScreen.text.getGlobalBounds().width + 6, goScreen.text.getGlobalBounds().height + 6));
		goScreen.rect.setPosition(sf::Vector2f(1025 - (1025 / 2 - (goScreen.rect.getSize().x / 2)), 650 - (650 / 2 - (goScreen.rect.getSize().y / 2))));
		goScreen.text.setPosition(goScreen.rect.getPosition().x + (goScreen.rect.getSize().x / 400000), goScreen.rect.getPosition().y);
	}
	sf::Vector2f worldPos;
	bool Start();
	void Update(MainRenderWindow& mainWindow);

	bool goActive = false;

	void ChangeMode(Mode::MenuMode k)
	{
		goMode.currentMode = k;
		goActive = true;
	}
};