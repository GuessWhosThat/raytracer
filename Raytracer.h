#pragma once

#include <SFML\Graphics.hpp>

#include "World.h"

class Tracer {
private:
	//SFML
	sf::RenderWindow window;
	sf::Event e;
	sf::Clock clock;

	sf::Time delta, update;

	sf::Texture texture;
	sf::Sprite sprite;
	
	//TRACING
	World world;

	Settings s;

	//PRIVATE FUNCTIONS
	void LoadSettings();
public:
	Tracer();
	~Tracer();

	void Run();
};