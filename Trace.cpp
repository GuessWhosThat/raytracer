#include "World.h"

#include <random>

#define MAX_DEPTH 3

static std::mutex imagebarrier;

bool isInterrupting(const ObjectContainer& obj, const Vector& pos, const Ray& shadow) {
	Collision info;
	double d = Vector::distance(pos, shadow.point);

	for (auto &o : obj) {
		if (o->CalculateCollision(shadow, &info)) {
			if (info.distance < d && info.distance != 0.0f) {
				return true;
			}
		}
	}

	return false;
}

Vector Sample(const ObjectContainer& obj, const LightContainer& lights, const Ray& primary, unsigned depth) {
	if (depth > MAX_DEPTH) {
		return Vector();
	}

	Vector color, reflection_color, H;
	Ray shadow, reflection;
	Collision info, bestInfo;
	bestInfo.distance = 25000.0f;
	bool hit = false;
	for (auto &o : obj) {
		if (o->CalculateCollision(primary, &info)) {
			if (info.distance < bestInfo.distance && info.distance != 0.0f) {
				hit = true;
				bestInfo = info;
			}
		}
	}
	if (hit) {
		H = bestInfo.intersection;
		for (auto &l : lights) {
			//calculate reflection ray
			reflection.point = bestInfo.intersection;
			reflection.dir = bestInfo.normal * (Vector::dot(primary.dir * (-1.0f), bestInfo.normal) * 2.0f) - primary.dir * (-1.0f);

			if (l.type == POINT) {
				shadow.dir = Vector::normal(l.position - bestInfo.intersection);
				shadow.point = bestInfo.intersection + shadow.dir * 0.0001f;
				//if if shadow ray not intersecting
				if (!isInterrupting(obj, l.position, shadow)) {
					float lamber = std::fmax(Vector::dot(shadow.dir, bestInfo.normal), 0.0f);
					float distance = Vector::distance(l.position, bestInfo.intersection);
					distance = distance * distance;
					//Diffusion
					color.x += bestInfo.mat.color.x * l.color.x * l.intensity * lamber * (1.0f - bestInfo.mat.reflection_index) / distance;
					color.y += bestInfo.mat.color.y * l.color.y * l.intensity * lamber * (1.0f - bestInfo.mat.reflection_index) / distance;
					color.z += bestInfo.mat.color.z * l.color.z * l.intensity * lamber * (1.0f - bestInfo.mat.reflection_index) / distance;
					//Specular
					H = Vector::normal(shadow.dir + primary.dir * (-1.0f));
					float spec = std::pow(std::max(Vector::dot(bestInfo.normal, H), (double)0.0f), bestInfo.mat.shinines);
					color += l.color * spec * bestInfo.mat.spec_factor;
					//Reflecting
					if (bestInfo.mat.reflection_index > 0.1f) {
						reflection_color = Sample(obj, lights, reflection, depth + 1);
						color += reflection_color * bestInfo.mat.reflection_index;
					}
				}
			}
			else if(l.type == AMBIENT) {
				color.x += bestInfo.mat.color.x * l.color.x * l.intensity;
				color.y += bestInfo.mat.color.y * l.color.y * l.intensity;
				color.z += bestInfo.mat.color.z * l.color.z * l.intensity;
			}
			else if (l.type == DIRECTIONAL) {
				shadow.dir = Vector::normal(l.position *(-1.0f));
				shadow.point = bestInfo.intersection + shadow.dir * 0.0001f;
				//if shadow ray not intersecting
				if (!isInterrupting(obj, shadow.point + shadow.dir * 25000.0f, shadow)) {
					float lamber = std::fmax(Vector::dot(shadow.dir, bestInfo.normal), 0.0f);
					//Iluminate
					color.x += bestInfo.mat.color.x * l.color.x * l.intensity * lamber * (1.0f - bestInfo.mat.reflection_index);
					color.y += bestInfo.mat.color.y * l.color.y * l.intensity * lamber * (1.0f - bestInfo.mat.reflection_index);
					color.z += bestInfo.mat.color.z * l.color.z * l.intensity * lamber * (1.0f - bestInfo.mat.reflection_index);
					//Specular
					H = Vector::normal(shadow.dir + primary.dir * (-1.0f));
					float spec = std::pow(std::max(Vector::dot(bestInfo.normal, H), (double)0.0f), bestInfo.mat.shinines);
					color += l.color * spec * bestInfo.mat.spec_factor;
					//Reflecting
					if (bestInfo.mat.reflection_index > 0.1f) {
						reflection_color = Sample(obj, lights, reflection, depth + 1);
						color += reflection_color * bestInfo.mat.reflection_index;
					}
				}
			}
		}
	}
	else {
		color += Vector(0.75f, 1.0f, 1.0f);
	}
	color.Clamp(0.0f, 1.0f);
	return color;
}

int Trace(const ObjectContainer& obj, const LightContainer& lights, const Settings& s, sf::Image* im,
	unsigned startX, unsigned startY,
	unsigned endX, unsigned endY, const bool& end) {
	//RANDOM
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0f, 1.0f);
	//RANDOM

	//VARIABLES
	float relativeX, relativeY;
	float ratio = (float)s.width / (float)s.height;

	Ray primary;
	Vector average;
	//VARIABLES

	//ALGORITHM
	for (auto x = startX; x < endX; ++x) {
		for (auto y = startY; y < endY; ++y) {
			//PREPARE VARIABLES
			relativeX = x / (float)s.width;
			relativeY = y / (float)s.height;

			//COMPUTE THE SAMPLES
			average.Reset();
			for (auto i = 0; i < s.samples; ++i) {
				primary.dir = Vector::normal(Vector(
					2.0f*relativeX*ratio - ratio + dist(mt)/(float)s.width,
					2.0f*relativeY - 1.0f + dist(mt) / (float)s.height,
					1.0f
				));
				average += Sample(obj, lights, primary, 1);
			}
			average = average / s.samples;

			//ACCESS IMAGE
			{
				std::lock_guard<std::mutex> guard(imagebarrier);
				im->setPixel(x, s.height - y -1, average);
			}
		}
		if (end) {
			return 0;
		}
	}
	//ALGORITHM
	return 0;
}