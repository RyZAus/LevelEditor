#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

//ALLOWS US TO CHANGE THE 1 AND ONLY ACTIVE WINDOW
class MainRenderWindow
{
public:
	//Window size
	const int windowWidth = 1025;
	const int windowHeight = 650;
	//mouse position
	sf::Vector2f worldPos;
	sf::RenderWindow window;
	//functions
	//This Slims down mywindow.window.clear to mywindow.clear
	void clear()
	{
		window.clear(sf::Color::White);
	}
	//This slims down mywindow.window.close to mywindow.close
	void close()
	{
		window.close();
	}



};