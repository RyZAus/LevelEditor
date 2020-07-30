#pragma once
//This is our header file designed so we can call things to our main script easier
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include <functional>
#include "WindowHeader.h"
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
	bool mouseOver(sf::Vector2f windpos)
	{
		sf::FloatRect rBounds = sprite.getGlobalBounds();
		return rBounds.contains(windpos.x, windpos.y);
	}
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

class InputField : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rect, states);
		target.draw(text, states);
	}
public:
	sf::RectangleShape rect;
	sf::Text text;
	sf::Font font;
	int fontSize = 16;
	InputField()
	{
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setCharacterSize(fontSize);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Regular);
	}
	void UpdatePos(sf::Vector2f newPos)
	{
		rect.setSize(sf::Vector2f(text.getGlobalBounds().width + 6, text.getGlobalBounds().height + 6));
		rect.setPosition(newPos);
		text.setPosition(rect.getPosition().x + (rect.getSize().x / 400000), rect.getPosition().y);
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
	EditorClass();
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
	//setup variable to track selected brush
	Tile::Type curTileType;
	Actor::Type curActorType;
	//setup space for our toolbar
	ToolPanel tools;
	//setup tiles to select and paint with
	Tile tileButton[9];
	//setup tiles array
	//Tile tile[x][y];
	Tile** tile = new Tile * [x];

	//for another day / now edited
	bool Start(MainRenderWindow &mainWindow);
	void Update(MainRenderWindow &mainWindow);
	bool editorActive = false;
	std::string playerInput;
	InputField inputField;
	//saving
	void save(std::string fileName, Tile** incTile)
	{
		//write text to file
		std::ofstream myfile(fileName + ".sav");
		std::list<sf::Vector2i> coinPos;
		std::list<sf::Vector2i> enemyPos;
		std::list<sf::Vector2i> spikePos;
		sf::Vector2i playerPos;
		sf::Vector2i exitPos;
		if (myfile.is_open())
		{
			std::cout << "Saving Tiles \n";
			for (int i = 0; i < x; i++)
			{
				for (int j = 0; j < y; j++)
				{
					switch (incTile[i][j].type)
					{
						//sky = 0, platform = 1, lava = 2
					case Tile::Type::Sky:
						//print the type of tile to my file
						myfile << "0";
						break;
					case Tile::Type::Platform:
						myfile << "1";
						break;
					case Tile::Type::Lava:
						myfile << "2";
						break;
					}
					myfile << ",";
					switch (incTile[i][j].actor.type)
					{				
					case Actor::Type::Coin:
						coinPos.push_back(sf::Vector2i(i, j));
						break;
					case Actor::Type::Enemy:
						enemyPos.push_back(sf::Vector2i(i, j));
						break;
					case Actor::Type::Spike:
						spikePos.push_back(sf::Vector2i(i, j));
						break;
					case Actor::Type::Player:
						playerPos = sf::Vector2i(i, j);
						break;
					case Actor::Type::Exit:
						exitPos = sf::Vector2i(i, j);
						break;
					}
				}
				myfile << "\n";
			}
			//loop through all the lists and add them to the save file. Then add the exit positions
			//coins
			if (!coinPos.empty())
			{
				std::cout << "Saving Coins! \n";
				myfile << "Coin";
				std::list<sf::Vector2i>::iterator cIt;
				for (cIt = coinPos.begin(); cIt != coinPos.end(); cIt++)
				{
					sf::Vector2i curCoinPos = sf::Vector2i(cIt->x, cIt->y);
					std::cout << "Saving coin at: " << curCoinPos.x << "," << curCoinPos.y << "\n";
					myfile << '(' << curCoinPos.x << ',' << curCoinPos.y << ')';
				}
				myfile << "\n";
			}
			//enemies
			if (!enemyPos.empty())
			{
				std::cout << "Saving Enemies! \n";
				myfile << "Enemy";
				std::list<sf::Vector2i>::iterator eIt;
				for (eIt = enemyPos.begin(); eIt != enemyPos.end(); eIt++)
				{
					sf::Vector2i curEnemyPos = sf::Vector2i(eIt->x, eIt->y);
					std::cout << "Saving enemy at: " << curEnemyPos.x << "," << curEnemyPos.y << "\n";
					myfile << '(' << curEnemyPos.x << ',' << curEnemyPos.y << ')';
				}
				myfile << "\n";
			}
			//spikes
			if (!spikePos.empty())
			{
				std::cout << "Saving Spikes! \n";
				myfile << "Spike";
				std::list<sf::Vector2i>::iterator sIt;
				for (sIt = spikePos.begin(); sIt != spikePos.end(); sIt++)
				{
					sf::Vector2i curSpikePos = sf::Vector2i(sIt->x, sIt->y);
					std::cout << "Saving spike at: " << curSpikePos.x << "," << curSpikePos.y << "\n";
					myfile << '(' << curSpikePos.x << ',' << curSpikePos.y << ')';
				}
				myfile << "\n";
			}
			//player
			std::cout << "Saving player at: " << playerPos.x << ',' << playerPos.y << "\n";
			myfile << "Player" << '(' << playerPos.x << ',' << playerPos.y << ')' << "\n";
			//exit
			std::cout << "Saving exit at: " << exitPos.x << ',' << exitPos.y << "\n";
			myfile << "Exit" << '(' << exitPos.x << ',' << exitPos.y << ')' << "\n";
			//all done
			myfile.close();
			std::cout << "Saving Complete! \n";
		}
		else
		{
			std::cout << "Can't open save file! \n";
		}
	}
	//loading
	void load(std::string fileName, Tile** incTile)
	{
		std::string line;
		std::ifstream myfile(fileName + ".sav");
		if (myfile.is_open())
		{
			int a = 0;
			int b = 0;
			std::string saveHold;
			while (std::getline(myfile, line))
			{
				if (b == 0)
				{
					std::cout << "Loading Tiles \n";
				}
				if (b < x)
				{
					for (int i = 0; i < line.size(); i++)
					{
						switch (line[i])
						{
						case ',':
							a++;
							break;
						case '0':
							incTile[b][a].ChangeTile(Tile::Type::Sky);
							break;
						case '1':
							incTile[b][a].ChangeTile(Tile::Type::Platform);
							break;
						case '2':
							incTile[b][a].ChangeTile(Tile::Type::Lava);
							break;
						}
						incTile[b][a].RefreshTile();
						incTile[b][a].ChangeActor(Actor::Type::None);
					}
				}
				else if (b >= x) //loading actors
				{
					//check the first two characters on the lines after tiles are loaded
					//[c][o]ins, [s][p]ikes, [e][n]emies
					std::string lineHold = line;
					int curStart;
					int curEnd = 0;
					std::string posString;
					if (line[0] == 'C' && line[1] == '0') //[c][o]ins
					{
						std::cout << "Loading Coins! \n";
						while (curEnd < lineHold.length() && lineHold.find('(') != std::string::npos) //if the ")" isnt the last line
						{
							curStart = lineHold.find('('); //find the first bracket of one of the positions
							curEnd = lineHold.find(')'); //find the closing bracket thats next in line
							posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1)); //cut the numbers out
							std::cout << "Loaded coin at: " << posString << "\n"; //we now have something like 5,15
							std::string xStr = posString.substr(0, posString.find(','));
							std::string yStr = posString.substr(posString.find(','), posString.length());
							incTile[std::stoi(xStr)][std::stoi(yStr)].ChangeActor(Actor::Type::Coin);
							lineHold[curStart] = '<';
							lineHold[curEnd] = '>';
						}
					}
					else if (line[0] == 'E' && line[1] == 'n') //[e][n]imies
					{
						std::cout << "Loading Enemies! \n";
						while (curEnd < lineHold.length() && lineHold.find('(') != std::string::npos) //if the ")" isnt the last line
						{
							curStart = lineHold.find('('); //find the first bracket of one of the positions
							curEnd = lineHold.find(')'); //find the closing bracket thats next in line
							posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1)); //cut the numbers out
							std::cout << "Loaded enemy at: " << posString << "\n"; //we now have something like 5,15
							std::string xStr = posString.substr(0, posString.find(','));
							std::string yStr = posString.substr(posString.find(',') + 1, posString.length());
							incTile[std::stoi(xStr)][std::stoi(yStr)].ChangeActor(Actor::Type::Enemy);
							lineHold[curStart] = '<';
							lineHold[curEnd] = '>';
						}
					}
					else if (line[0] == 'S' && line[1] == 'p') //[s][p]ikes
					{
						std::cout << "Loading Spikes! \n";
						while (curEnd < lineHold.length() && lineHold.find('(') != std::string::npos) //if the ")" isnt the last line
						{
							curStart = lineHold.find('('); //find the first bracket of one of the positions
							curEnd = lineHold.find(')'); //find the closing bracket thats next in line
							posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1)); //cut the numbers out
							std::cout << "Loaded spike at: " << posString << "\n"; //we now have something like 5,15
							std::string xStr = posString.substr(0, posString.find(','));
							std::string yStr = posString.substr(posString.find(',') + 1, posString.length());
							incTile[std::stoi(xStr)][std::stoi(yStr)].ChangeActor(Actor::Type::Spike);
							lineHold[curStart] = '<';
							lineHold[curEnd] = '>';
						}
					}
					else if (line[0] == 'P' && line[1] == 'l') //[p][l]ayer
					{
						std::cout << "Loading Player! \n";
						while (curEnd < lineHold.length() && lineHold.find('(') != std::string::npos) //if the ")" isnt the last line
						{
							curStart = lineHold.find('('); //find the first bracket of one of the positions
							curEnd = lineHold.find(')'); //find the closing bracket thats next in line
							posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1)); //cut the numbers out
							std::cout << "Loaded player at: " << posString << "\n"; //we now have something like 5,15
							std::string xStr = posString.substr(0, posString.find(','));
							std::string yStr = posString.substr(posString.find(',') + 1, posString.length());
							incTile[std::stoi(xStr)][std::stoi(yStr)].ChangeActor(Actor::Type::Player);
							lineHold[curStart] = '<';
							lineHold[curEnd] = '>';
						}
					}
					else if (line[0] == 'E' && line[1] == 'x') //[e][x]it
					{
						std::cout << "Loading Exit! \n";
						while (curEnd < lineHold.length() && lineHold.find('(') != std::string::npos) //if the ")" isnt the last line
						{
							curStart = lineHold.find('('); //find the first bracket of one of the positions
							curEnd = lineHold.find(')'); //find the closing bracket thats next in line
							posString = lineHold.substr(curStart + 1, curEnd - (curStart + 1)); //cut the numbers out
							std::cout << "Loaded exit at: " << posString << "\n"; //we now have something like 5,15
							std::string xStr = posString.substr(0, posString.find(','));
							std::string yStr = posString.substr(posString.find(',') + 1, posString.length());
							incTile[std::stoi(xStr)][std::stoi(yStr)].ChangeActor(Actor::Type::Exit);
							lineHold[curStart] = '<';
							lineHold[curEnd] = '>';
						}
					}
				}
				b += 1;
				a = 0;
			}
			myfile.close();
			std::cout << "File loaded! \n";
		}
		else
		{
			std::cout << "Cannot open save file to load! \n";
		}
	}
	//printing our tiles out (debug)
};

