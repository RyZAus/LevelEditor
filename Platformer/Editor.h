#pragma once
//This is our header file designed so we can call things to our main script easier
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include <functional>
//make our class actor(layer1) both drawable and transformable
class Actor : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
		target.draw(sprite, states);
	}
public:
	sf::Sprite sprite;
	//declare our players aka layer 1
	enum Type
	{
		None,
		Player,
		Enemy,
		Coin,
		Spike,
		Exit
	};
	//sets our type to none by default
	Type type = None;
	//declare our textures for each of the above
	sf::Texture blankTexture;
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture coinTexture;
	sf::Texture spikeTexture;
	sf::Texture doorTexture;
	//Just to say this variable exists in memeory
	Actor(){}
	//check to see if the sprites can be loaded and if not return the error message to say which hasn't loaded. also useful so we can call it later
	void loadTextures()
	{
		if (!blankTexture.loadFromFile("SpritesPlatformer/None.png"))
		{
			std::cout << "Failed to load None.png";
		}
		if (!playerTexture.loadFromFile("SpritesPlatformer/Player.png"))
		{
			std::cout << "Failed to load Player.png";
		}
		if (!enemyTexture.loadFromFile("SpritesPlatformer/EnemyAlive.png"))
		{
			std::cout << "Failed to load EnemyAlive.png";
		}
		if (!coinTexture.loadFromFile("SpritesPlatformer/Coin.png"))
		{
			std::cout << "Failed to load Coin.png";
		}
		if (!spikeTexture.loadFromFile("SpritesPlatformer/Spike.png"))
		{
			std::cout << "Failed to load Spike.png";
		}
		if (!doorTexture.loadFromFile("SpritesPlatformer/Door.png"))
		{
			std::cout << "Failed to load Door.png";
		}
	}
	void init(int x, int y)
	{
		//loads the testure
		loadTextures();
		//uses position
		sprite.setPosition(x, y);
		//make our sprites update at the end
		RefreshActor();
	}
	void ChangeActor(Type t)
	{
		type = t;
		//make our sprites update at the end
		RefreshActor();
	}
	//makes our sprites refresh whenever we call this
	void RefreshActor()
	{
		//switch for our textures so we can call them
		switch (type)
		{
		case None:
			sprite.setTexture(blankTexture);
			break;
		case Player:
			sprite.setTexture(playerTexture);
			break;
		case Enemy:
			sprite.setTexture(enemyTexture);
			break;
		case Coin:
			sprite.setTexture(coinTexture);
			break;
		case Spike:
			sprite.setTexture(spikeTexture);
			break;
		case Exit:
			sprite.setTexture(doorTexture);
			break;
		}
	}
	//actor class ends here
};
//tiles are our layer 0 aka behind the player make our class tile(layer0) both drawable and transformable
class Tile : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
		//make sure to draw tiles as layer 0
		target.draw(actor, states);
	}
public:
	sf::Sprite sprite;
	//declare our players aka layer 0
	Actor actor;
	enum Type
	{
		Sky,
		Platform,
		Lava
	};
	//sets our type to sky by default
	Type type = Sky;
	//declare our textures for each of the above
	sf::Texture skyTexture;
	sf::Texture platformTexture;
	sf::Texture lavaTexture;
	//check to see if the sprites can be loaded and if not return the error message to say which hasn't loaded. also useful so we can call it later
	void loadTextures()
	{
		if (!skyTexture.loadFromFile("SpritesPlatformer/BlockSky.png"))
		{
			std::cout << "Failed to load BlockSky.png";
		}
		if (!platformTexture.loadFromFile("SpritesPlatformer/BlockPlatform.png"))
		{
			std::cout << "Failed to load BlockPlatform.png";
		}
		if (!lavaTexture.loadFromFile("SpritesPlatformer/BlockLava.png"))
		{
			std::cout << "Failed to load BlockLava.png";	
		}
	}
	void init(int x, int y)
	{
		//loads the testure
		loadTextures();
		//uses position
		sprite.setPosition(x, y);
		//make our sprites update at the end
		RefreshTile();
		actor.init(x, y);
	}
	void ChangeTile(Type t)
	{
		type = t;
		//make our sprites update at the end
		RefreshTile();
	}
	//we have to do this every time "tile.actor.ChangeActor(t)" this makes it so we can just type "tile.ChangeActor(t)"
	void ChangeActor(Actor::Type a)
	{
		actor.ChangeActor(a);
	}

	//makes our sprites refresh whenever we call this
	void RefreshTile()
	{
		//switch for our textures so we can call them
		switch (type)
		{
		case Sky:
			sprite.setTexture(skyTexture);
			break;
		case Platform:
			sprite.setTexture(platformTexture);
			break;
		case Lava:
			sprite.setTexture(lavaTexture);
			break;
		}
	}
	//tiles class ends here
};

class Button : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rect, states);
		target.draw(text, states);
	}

public:
	int textSize = 16;
	float rectOutline = 1;
	sf::RectangleShape rect;
	sf::Font font;
	sf::Text text;
	sf::FloatRect rBounds;

	Button()
	{
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setCharacterSize(textSize);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Regular);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(rectOutline);
	}
	bool mouseOver(sf::Vector2f windpos)
	{
		rBounds = rect.getGlobalBounds();
		if (rBounds.contains(windpos.x, windpos.y))
		{
			//if the button is being hovered over it will increase the thickness of the border
			rect.setOutlineThickness(rectOutline * 2);
			return true;
		}
		else
		{
			//otherwise leave the thickness the same
			rect.setOutlineThickness(rectOutline);
			return false;
		}
	}
	//check for a click if so pass a function as an argument to store
	void checkClick(std::function<void()> f, sf::Vector2f windpos)
	{
		//if the left mouse is pressed while hovering over the button it will call function "f"
		if (mouseOver(windpos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			f();
		}
	}





};

class ToolPanel : public sf::Drawable, public sf::Transformable
//hold our buttons for us
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(loadButton, states);
		target.draw(saveButton, states);
	}

public:
	Button loadButton;
	Button saveButton;

	ToolPanel()
	{
		loadButton.text.setString("Load");
		loadButton.rect.setSize(sf::Vector2f(loadButton.text.getGlobalBounds().width + 6, loadButton.text.getGlobalBounds().height + 6));
		loadButton.rect.setPosition(sf::Vector2f(2, 2));
		loadButton.text.setPosition(loadButton.rect.getPosition().x + (loadButton.rect.getSize().x / 400000), loadButton.rect.getPosition().y);
		saveButton.text.setString("Save");
		saveButton.rect.setSize(sf::Vector2f(saveButton.text.getGlobalBounds().width + 6, saveButton.text.getGlobalBounds().height + 6));
		saveButton.rect.setPosition(sf::Vector2f(2, 27));
		saveButton.text.setPosition(saveButton.rect.getPosition().x + (saveButton.rect.getSize().x / 400000), saveButton.rect.getPosition().y);
	}
};

class EditorClass
{
public:
	//our grid size static = never change
	static const int x = 30;
	static const int y = 20;
	bool actorNotTile = false;
	const int windowWidth = 1025;
	const int windowHeight = 650;

	// Mouse position
	sf::Vector2f worldPos;
	sf::View toolsView;
	sf::View levelEditView;
	sf::RenderWindow window;
	//setup variable to track selected brush
	Tile::Type curTileType;
	Actor::Type curActorType;
	//setup space for our toolbar
	ToolPanel tools;
	//setup tiles to select and paint with
	Tile tileButton[9];
	//setup tiles array
	Tile tile[x][y];

	//for another day
	bool Start();
	int Update();

	//saving

	//loading

	//printing our tiles out (debug)
};

