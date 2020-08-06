#include "Game.h"

GameClass::GameClass()
{
	//physics
	gravity = 0.5f;
	friction = 5.0f;
	//loop tiles to init
	for (int i = 0; i < x; i++)
	{
		tile[i] = new Tile[y];
	}
}

void GameClass::LoadLevel(std::string levelName, Tile** incTile)
{
	//cleanup current tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			incTile[i][j].type = Tile::Type::Sky;
			incTile[i][j].actor.type = Actor::Type::None;
			incTile[i][j].RefreshTile();
		}
	}
	//exsisting load
	std::string line;
	std::ifstream myfile(levelName);
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
						//set player to player
						player.type = Actor::Type::Player;
						//centre player on grid square
						player.init(stoi(xStr) * 32 + ((1025 / 2) - ((32 * x) / 2)), stoi(yStr) * 32);
						player.startPos = sf::Vector2f(stoi(xStr) * 32 + ((1025 / 2) - ((32 * x) / 2)), stoi(yStr) * 32);
						player.setPosition(player.startPos);
						player.Respawn();
						//incTile[std::stoi(xStr)][std::stoi(yStr)].ChangeActor(Actor::Type::Player);
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

void Player::Respawn()
{
	setPosition(startPos);
	Refresh();
}

Collision Player::CollisionCheck(sf::FloatRect other)
{
	Collision col;
	//is the position we want to move valid?
	float deltaX = (nextPos.x - (nextRect.width / 2)) - (other.left - other.width / 2);
	float deltaY = (nextPos.y - (nextRect.height / 2)) - (other.top - other.height / 2);
	//we want an absolute number (between 0 / 1 Whole number)
	float intersectX = abs(deltaX) - ((other.width / 2) + (nextRect.width / 2));
	float intersectY = abs(deltaY) - ((other.height / 2) + (nextRect.height / 2));
	//if they are both under 0 we have hit something (True or false)
	col.hit = (intersectX < 0.0f && intersectY < 0.0f);
	//what direction?
	if (col.hit)
	{
		//have we collided x?
		if (intersectX > intersectY)
		{
			if (deltaX > 0.0f)
			{
				col.dir = sf::Vector2f(intersectX, 0.0f);
			}
			else
			{
				col.dir = sf::Vector2f(-intersectX, 0.0f);
			}
		}
		else
		{
			if (deltaY > 0.0f)
			{
				col.dir = sf::Vector2f(0.0f, intersectY);
			}
			else
			{
				col.dir = sf::Vector2f(0.0f, -intersectY);
			}
		}
	}
	else
	{
		col.dir = sf::Vector2f(0.0f, 0.0f);
	}
	return col;
}

Player::Player()
{
	lives = 3;
	coins = 0;
	isDead = false;
	isGrounded = true;
	speed = 2.5f;
	jumpSpeed = 0.75f;
	veloctiy = sf::Vector2f(0.0f, 0.0f);
}

void Player::Refresh()
{
	Actor::RefreshActor();
	veloctiy = sf::Vector2f(0.0f, 0.0f);
	nextPos = getPosition();
}

sf::Vector2f Player::getPosition()
{
	return sprite.getPosition();
}
void Player::setPosition(sf::Vector2f p)
{
	sprite.setPosition(p);
}
int sign(int x)
{
	return (x > 0) - (x < 0);
}
int sign(float x)
{
	return (x > 0.0f) - (x < 0.0f);
}
void Enemy::Killed()
{
	type = Actor::Type::None;
	RefreshActor();
}
bool Player::isAnyKeyPressed()
{
	//when we increase i to 0 it checks to see if 0 is pressed because -1 is not a key
	for (int i = -1; i < sf::Keyboard::KeyCount; i++)
	{
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
		{
			return true;
		}
	}
	return false;
}