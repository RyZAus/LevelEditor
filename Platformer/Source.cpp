#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING // Silence experimental warning for VS2019
#include "Editor.h"
#include "MenuHeader.h"
#include "WindowHeader.h"
#include "Game.h"
#include <experimental/filesystem>
//faster and skips re type
namespace fs = std::experimental::filesystem;
std::vector<std::string> saves;
int curLevel = 0;
void GetAllSaveFiles()
{
	std::string path = "\.";
	std::string ext = ".sav";
	saves.clear();
	//auto function takes the directory and checks every file for the extension we are looking for ".sav" then store it in a string and a vector list
	for (auto& p : fs::recursive_directory_iterator(path))
	{
		if (p.path().extension() == ext)
		{
			std::cout << p.path().filename().generic_string() << '\n';
			std::string file = p.path().filename().generic_string();
			saves.push_back(file);
		}
	}
}
int main()
{
	EditorClass myEditor;
	MainMenu myMainMenu;
	GameClass myGame;
	GameOver gOver; //EDIT
	MainRenderWindow mainWindow;
	mainWindow.window.create(sf::VideoMode(mainWindow.windowWidth, mainWindow.windowHeight), "My Program", sf::Style::Titlebar | sf::Style::Close);
	while (mainWindow.window.isOpen())
	{
		sf::Event event;
		while (mainWindow.window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				mainWindow.close();
				break;
			}
		}
		switch (myMainMenu.myMode.currentMode)
		{
		case Mode::MainMenu:
			if (!myMainMenu.Start())
			{
				return EXIT_FAILURE;
			}
			myMainMenu.Update(mainWindow);
			break;
		case Mode::Editor:
			if (!myEditor.editorActive)
			{
				if (!myEditor.Start(mainWindow))
				{
					return EXIT_FAILURE;
				}
			}
			myEditor.Update(mainWindow);
			break;
		case Mode::Game:
			if (!myGame.gameActive)
			{
				if (!myGame.Start(mainWindow))
				{
					return EXIT_FAILURE;
				}
			}
			myGame.Update(mainWindow, myMainMenu);
			break;
		case Mode::GameOver: //EDIT
			if (!gOver.goActive)
			{
				if (!gOver.Start())
				{
					return EXIT_FAILURE;
				}
			}
			gOver.Update(mainWindow);
			break; //EDIT
		default:
			break;
		}
	}
	return 0;
}
bool MainMenu::Start()
{
	return true;
}
void MainMenu::Update(MainRenderWindow& mainWindow)
{
	while (menuActive)
	{
		mainWindow.worldPos = mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window));
		levelEditorButton.checkClick(std::bind(&MainMenu::ChangeMode, this, Mode::Editor), mainWindow.worldPos);
		gameButton.checkClick(std::bind(&MainMenu::ChangeMode, this, Mode::Game), mainWindow.worldPos);
		mainWindow.clear();
		mainWindow.window.draw(levelEditorButton);
		mainWindow.window.draw(gameButton);
		mainWindow.window.display();
	}
	return;
}
EditorClass::EditorClass()
{
	for (int i = 0; i < x; i++)
	{
		tile[i] = new Tile[y];
	}
}
bool EditorClass::Start(MainRenderWindow& mainWindow)
{
	//setup our views
	toolsView = sf::View(sf::FloatRect(0, 0, mainWindow.windowWidth * 0.045f, mainWindow.windowHeight));
	toolsView.setViewport(sf::FloatRect(0, 0, 0.045f, 1));
	levelEditView = sf::View(sf::FloatRect(0, 0, mainWindow.windowWidth, mainWindow.windowHeight));
	levelEditView.setViewport(sf::FloatRect(0.03f, 0, 1, 1));

	// old Setup the window
	// old window.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor", sf::Style::Titlebar | sf::Style::Close);

	//Setup variables to paint with
	curTileType = Tile::Type::Platform;
	curActorType = Actor::Type::Coin;

	for (int i = 0; i < 9; i++)
	{
		tileButton[i].init(10, (32 + 5) * i + 150);
	}
	//Setting each button to have a tile of type.
	//tileButton[0].type = Tile::Type::Sky;
	//tileButton[1].type = Tile::Type::Platform;
	//tileButton[2].type = Tile::Type::Lava;
	tileButton[0].ChangeTile(Tile::Type::Sky);
	tileButton[1].ChangeTile(Tile::Type::Platform);
	tileButton[2].ChangeTile(Tile::Type::Lava);
	tileButton[3].ChangeActor(Actor::Type::None);
	tileButton[4].ChangeActor(Actor::Type::Player);
	tileButton[5].ChangeActor(Actor::Type::Enemy);
	tileButton[6].ChangeActor(Actor::Type::Coin);
	tileButton[7].ChangeActor(Actor::Type::Spike);
	tileButton[8].ChangeActor(Actor::Type::Exit);

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			tile[i][j].init(i * 32 + ((mainWindow.windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}
	editorActive = true;
	return true;
}
void EditorClass::Update(MainRenderWindow& mainWindow)
{
	//prep window for displaying stuff
	mainWindow.window.clear(sf::Color::White);
	mainWindow.window.setView(toolsView);
	//track mouse pos
	worldPos = mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window));
	//bind our save load buttons to save load functions
	tools.saveButton.checkClick(std::bind(&EditorClass::save, this, playerInput, tile), worldPos);
	tools.loadButton.checkClick(std::bind(&EditorClass::load, this, playerInput, tile), worldPos);
	//loop through our tool bar to check for clicks
	for (int i = 0; i < 9; i++)
	{
		if (tileButton[i].mouseOver(worldPos))
		{
			tileButton[i].sprite.setScale(sf::Vector2f(1.1f, 1.1f));
			tileButton[i].actor.sprite.setScale(sf::Vector2f(1.1f, 1.1f));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (i < 3)
				{
					actorNotTile = false;
					curTileType = tileButton[i].type;
				}
				else
				{
					actorNotTile = true;
					curActorType = tileButton[i].actor.type;
				}
			}
		}
		else
		{
			tileButton[i].sprite.setScale(sf::Vector2f(1.f, 1.f));
			tileButton[i].actor.sprite.setScale(sf::Vector2f(1.f, 1.f));
		}
		mainWindow.window.draw(tileButton[i]);
	}
	mainWindow.window.draw(tools);
	mainWindow.window.setView(levelEditView);
	//fixes the tile offset for the grid
	worldPos = mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window), mainWindow.window.getView());
	//draw our tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			//check for click
			if (tile[i][j].mouseOver(worldPos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!actorNotTile)
				{
					tile[i][j].ChangeTile(curTileType);
				}
				else
				{
					tile[i][j].ChangeActor(curActorType);
				}
			}
			tile[i][j].RefreshTile();
			mainWindow.window.draw(tile[i][j]);
		}
	}
	sf::Event event;
	while (mainWindow.window.pollEvent(event))
	{
		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode == 8 && playerInput.size() != 0)
			{
				playerInput.pop_back();
				std::cout << playerInput << std::endl;
			}
			else if (event.text.unicode < 128)
			{
				playerInput += static_cast<char>(event.text.unicode);
				std::cout << playerInput << std::endl;
			}
			inputField.text.setString(playerInput);
		}
	}
	mainWindow.window.draw(inputField);
	mainWindow.window.display();
}
bool GameClass::Start(MainRenderWindow& mainWindow)
{
	deathSB.loadFromFile("SFX/death.wav");
	coinSB.loadFromFile("SFX/coin.wav");
	killSB.loadFromFile("SFX/death.wav");
	GetAllSaveFiles();
	//setup our game and init our tiles
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			tile[i][j].init(i * 32 + ((mainWindow.windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}
	//load save file
	if (saves.size() != 0)
	{
		LoadLevel(saves[curLevel], tile);
	}
	else
	{
		std::cout << "Error, no saved levels!" << std::endl;
	}
	player.nextPos = player.getPosition();
	gameActive = true;
	return true;
}
void GameClass::Update(MainRenderWindow& mainWindow, MainMenu& mainMenu)
{
	gameCounters.coinsCounter.text.setString("Coins: " + std::to_string(player.coins));
	gameCounters.livesCounter.text.setString("Lives: " + std::to_string(player.lives));
	mainWindow.window.clear(sf::Color::White);
	deltaTime = clock.restart().asSeconds();
	//controls
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//can we move right
		if (player.isGrounded)
		{
			player.veloctiy.x += player.speed * deltaTime;
			player.veloctiy.y += deltaTime;
		}
		else
		{
			player.veloctiy.x += player.speed / 3 * deltaTime;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//can we move left
		if (player.isGrounded)
		{
			player.veloctiy.x -= player.speed * deltaTime;
			player.veloctiy.y += deltaTime;
		}
		else
		{
			player.veloctiy.x -= player.speed / 3 * deltaTime;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (player.isGrounded)
		{
			player.isGrounded = false;
			player.veloctiy.y += -player.jumpSpeed + deltaTime;
		}
	}

	//friction
	if (player.isGrounded)
	{
		if (!player.isAnyKeyPressed())
		{
			player.veloctiy.x -= friction * deltaTime * (sign(player.veloctiy.x));
		}
	}
	//maximum absolute velocity
	if (abs(player.veloctiy.x) > 0.6f)
	{
		//slowly bring the characters velocity back to 0.6
		player.veloctiy.x = 0.6f * sign(player.veloctiy.x);
	}
	//max vertical velocity
	if (player.veloctiy.y < 1.0f)
	{
		player.veloctiy.y += gravity * deltaTime;
	}
	else if (player.veloctiy.y < -1.0f)
	{
		player.veloctiy.y = -1.0f;
	}
	//this is the players next potential position if they arent obstructed
	player.nextPos = player.getPosition() + player.veloctiy;
	//project a hit box
	player.nextRect = sf::FloatRect(player.nextPos, sf::Vector2f(32.f, 32.f));
	player.isGrounded = false;
	//loop for collision with tiles
	//return to this with for (int...
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			//draw our tiles from 1 - whatever i and j being in that place
			tile[i][j].RefreshTile();
			mainWindow.window.draw(tile[i][j]);
			//check for collisions
			if (tile[i][j].type == Tile::Type::Platform)
			{
				Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
				//did we hit the player
				if (pcol.hit)
				{
					//what position did we hit it?
					if (pcol.dir.x == 0)
					{
						//are we hitting it above or below?
						if (pcol.dir.y >= 0.0f)
						{
							//we are on top of the tile
							player.nextPos.y = tile[i][j].sprite.getGlobalBounds().top - 32 - 0.1f;
							//player is grounded as it has landed	
							player.isGrounded = true;
						}
						else
						{
							//we are on the bottom so we add however as we use 32 it should be an int you link to to prevent different size sprites causing issues
							player.nextPos.y = tile[i][j].sprite.getGlobalBounds().top + tile[i][j].sprite.getGlobalBounds().height + 0.1f;
							player.veloctiy.y = 0.0f;
						}
					}
					else //we hit horizontal instead
					{
						if (pcol.dir.x >= 0.0f)
						{
							//if you walk through walls you can add the - 0.1f
							player.nextPos.x = tile[i][j].sprite.getGlobalBounds().left - 32;
							player.veloctiy.x = 0.0f;
						}
						else
						{
							player.nextPos.x = tile[i][j].sprite.getGlobalBounds().left + tile[i][j].sprite.getGlobalBounds().width;
							player.veloctiy.x = 0.0f;
						}
					}
				}
			}
			else if (tile[i][j].type == Tile::Type::Lava)
			{
				Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
				if (pcol.hit)
				{
					//hit lava lose a life
					player.lives--;
					//SOUND BUFFER
					sound.setBuffer(deathSB);
					sound.play();
					//reset position
					player.Respawn();
					std::cout << "Player hit lava" << std::endl;
					if (player.lives == 0)
					{
						//game over screen needs to be here
						//
						mainWindow.close();
					}
				}
			}
			else if (tile[i][j].actor.type == Actor::Type::Coin)
			{
				//add coin then destroy by changing sprite to none
				Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
				if (pcol.hit)
				{
					std::cout << "Player Picked a Coin Up!" << std::endl;
					sound.setBuffer(coinSB);
					sound.play();
					player.coins++;					
					tile[i][j].ChangeActor(Actor::Type::None);
				}
			}
			else if (tile[i][j].actor.type == Actor::Type::Spike)
			{
				Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
				if (pcol.hit)
				{
					player.lives--;
					//SOUND BUFFER
					sound.setBuffer(deathSB);
					sound.play();
					player.Respawn();
					std::cout << "Player hit a spike" << std::endl;
					if (player.lives == 0)
					{
						//THIS SHOULD BE A END GAME SCREEN
						mainWindow.close();
					}
				}
			}
			else if (tile[i][j].actor.type == Actor::Type::Enemy)
			{
				Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
				if (pcol.hit)
				{
					//hit vertical
					if (pcol.dir.x == 0)
					{
						//is it above
						if (pcol.dir.y >= 0.0f)
						{
							//we're on top
							//kill enemy(temp code)
							sound.setBuffer(killSB);
							sound.play();
							tile[i][j].ChangeActor(Actor::Type::None);
						}
						else
						{
							//enemy above
							player.lives--;
							//SOUND BUFFER
							sound.setBuffer(deathSB);
							sound.play();
							player.Respawn();
							if (player.lives == 0)
							{
								//THIS SHOULD BE A END GAME SCREEN
								mainWindow.close();
							}
						}
					}
					else //horizontal hit insta death
					{
						player.lives--;
						//SOUND BUFFER
						sound.setBuffer(deathSB);
						sound.play();
						player.Respawn();
						if (player.lives == 0)
						{
							//THIS SHOULD BE A END GAME SCREEN
							mainWindow.close();
						}
					}
				}
			}
			else if (tile[i][j].actor.type == Actor::Type::Exit)
			{
				Collision pcol = player.CollisionCheck(tile[i][j].sprite.getGlobalBounds());
				if (pcol.hit)
				{
					if (curLevel != saves.size() - 1)
					{
						curLevel++;
						LoadLevel(saves[curLevel], tile);						
					}
					else
					{
						//Return to win screen
						mainMenu.ChangeMode(Mode::GameOver);
						return;
					}
				}
			}
		}
	}
	//set player pos
	player.setPosition(player.nextPos);
	//draw all our stuff
	mainWindow.window.draw(gameCounters.coinsCounter);
	mainWindow.window.draw(gameCounters.livesCounter);
	mainWindow.window.draw(player);
	mainWindow.window.display();
}
bool GameOver::Start()
{
	//Start the game over screen
	return true;
}
void GameOver::Update(MainRenderWindow& mainWindow)
{
	//Clear our window to display GO
	mainWindow.window.clear(sf::Color::White);
	mainWindow.window.draw(goScreen);
	mainWindow.window.display();
}