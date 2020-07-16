#include "Editor.h"

int main()
{
	EditorClass myEditor;
		if (!myEditor.Start())
		{
			return EXIT_FAILURE;
		}
	return myEditor.Update();
	return 0;

}
EditorClass::EditorClass()
{
	for (int i = 0; i < x; i++)
	{
		tile[i] = new Tile[y];
	}
}

bool EditorClass::Start()
{
	//setup our views aka if I click my top bar and it uses only that / menus
	toolsView = sf::View(sf::FloatRect(0, 0, windowWidth * 0.045f, windowHeight));
	toolsView.setViewport(sf::FloatRect(0, 0, 0.045f, 1));
	levelEditView = sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight));
	levelEditView.setViewport(sf::FloatRect(0.03f, 0, 1, 1));
	//Setup the window
	window.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor", sf::Style::Titlebar | sf::Style::Close);

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
			tile[i][j].init(i * 32 + ((windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}

	return true;
}

int EditorClass::Update()
{
	//editor loop aka while the window is open
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//our switch for event types
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		//prep window for displaying stuff
		window.clear(sf::Color::White);
		window.setView(toolsView);
		//track mouse pos
		worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		//bind our save load buttons to save load functions
		tools.saveButton.checkClick(std::bind(&EditorClass::save, this, tile), worldPos);
		tools.loadButton.checkClick(std::bind(&EditorClass::load, this, tile), worldPos);
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
			window.draw(tileButton[i]);
		}



		window.draw(tools);
		window.setView(levelEditView);
		//fixes the tile offset for the grid
		worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());

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
				window.draw(tile[i][j]);
			}
		}


		window.display();
	}	 	  
	return 0;
}
