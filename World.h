#pragma once

#include "Utility.h"
#include "Object.h"

#include <SFML\Graphics.hpp>

#include <thread>
#include <mutex>

class World {
private:
	ObjectContainer objects;
	LightContainer lights;

	std::thread *t;

	sf::Image im;
	Settings s;

	bool end;

	void LoadObjects();
public:
	void Init(const Settings& st);
	void StartCalculating();
	void Delete();
	sf::Image GetImage() { return im; }
};

int Trace(const ObjectContainer& obj, const LightContainer& lights, const Settings& s, sf::Image* im,
	unsigned startX, unsigned startY,
	unsigned endX, unsigned endY, const bool& end
	);