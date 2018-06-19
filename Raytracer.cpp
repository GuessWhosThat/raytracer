#include "Raytracer.h"

#include <fstream>
#include <string>

void Tracer::LoadSettings() {
	std::ifstream file("settings.ini");
	std::string string;
	file >> string;
	s.width = std::stoul(string);
	file >> string;
	s.height = std::stoul(string);
	file >> string;
	s.threads = std::stoul(string);
	file >> string;
	s.samples = std::stoul(string);
	file.close();
}

Tracer::Tracer() {
	LoadSettings();
	sf::VideoMode vm(s.width, s.height);

	window.create(vm, "Tracer", sf::Style::Default);
	texture.create(s.width, s.height);
	sprite.setTexture(texture);
	world.Init(s);
	world.StartCalculating();
}

Tracer::~Tracer() {
	world.Delete();
}

void Tracer::Run() {
	while (window.isOpen()) {
		//TIME
		delta = clock.restart();
		update += delta;
		//TIME

		//INPUT
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		//INPUT

		//UPDATE
		if(update.asSeconds() > 0.5f) {
			update -= sf::seconds(0.5f);
			texture.update(world.GetImage());
		}
		//UPDATE

		//DRAW
		window.clear();
		window.draw(sprite);
		window.display();
		//DRAW
	}
}