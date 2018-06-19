#include "World.h"

#include <fstream>
#include <string>

void World::LoadObjects() {

	Material mat;
	Vector vec1, vec2;
	double t;
	std::shared_ptr<Sphere> s;
	std::shared_ptr<Plane> p;
	Light l;

	int state = 0;
	std::string buffer;
	std::ifstream file("scene.ini");

	while (file >> buffer) {
		switch (state) {
		case 0:
			if (buffer == "SPHERE") {
				state = 1;
			}
			else if (buffer == "PLANE") {
				state = 2;
			}
			else if (buffer == "MATERIAL") {
				state = 3;
			}
			else if (buffer == "LIGHT") {
				state = 4;
			}
			else if (buffer == "OBJECT") {
				state = 5;
			}
			break;
		case 1:
			if (file >> buffer) { vec1.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.z = std::stof(buffer); } else { break; }
			if (file >> buffer) { t = std::stof(buffer); }      else { break; }
			s = std::make_shared<Sphere>();
			s->Set(vec1, t, mat);
			objects.push_back(std::move(s));
			state = 0;
			break;
		case 2:
			if (file >> buffer) { vec1.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.z = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.z = std::stof(buffer); } else { break; }
			p = std::make_shared<Plane>();
			p->Set(vec1, vec2, mat);
			objects.push_back(std::move(p));
			state = 0;
			break;
		case 3:
			if (file >> buffer) { vec1.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.z = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.z = std::stof(buffer); } else { break; }
			mat.color = vec1;
			mat.shinines = vec2.x; mat.spec_factor = vec2.y; mat.reflection_index = vec2.z;
			state = 0;
			break;
		case 4:
			if (file >> buffer) { vec1.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec1.z = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.x = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.y = std::stof(buffer); } else { break; }
			if (file >> buffer) { vec2.z = std::stof(buffer); } else { break; }
			if (file >> buffer) { t = std::stof(buffer); } else { break; }
			if (!(file >> buffer)) { break; }
			l.position = vec1;
			l.color = vec2;
			l.intensity = t;
			if (buffer == "AMBIENT") { l.type = AMBIENT; }
			else if (buffer == "POINT") { l.type = POINT; }
			else if (buffer == "DIRECTIONAL") { l.type = DIRECTIONAL; }
			lights.push_back(Light(l));
			state = 0;
			break;
		case 5:
			state = 0;
			break;
		default:
			break;
		}
	}
}

void World::Init(const Settings& st) {
	s = st;
	t = nullptr;
	im.create(s.width, s.height);
	LoadObjects();
}

void World::Delete() {
	end = true;
	for (auto i = 0; i < s.threads; ++i) {
		t[i].join();
	}
	if (t != nullptr) {
		delete[] t;
	}
}

void World::StartCalculating() {
	end = false;
	t = new std::thread[s.threads];
	for (unsigned i = 0; i < s.threads; ++i) {
		t[i] = std::thread(Trace,
			objects, lights, s, &im,
			0, ((float)s.height / s.threads)*i,
			s.width, ((float)s.height / s.threads)*(i+1),
			end);
	}
}