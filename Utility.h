#pragma once

#include <SFML\Graphics.hpp>

struct Settings {
	unsigned width, height;
	unsigned threads, samples;

	void operator=(const Settings& r) {
		width = r.width; height = r.height;
		threads = r.threads; samples = r.samples;
	}
};

struct Vector {
	float x, y, z;

	Vector();
	Vector(float X, float Y, float Z);
	Vector(const Vector&);

	void operator=(const Vector&);
	void operator+=(const Vector&);
	void operator-=(const Vector&);
	void operator*=(float);
	void operator/=(float);

	Vector operator*(float) const;
	Vector operator/(float) const;
	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;

	operator sf::Color() const;

	static inline double dot(const Vector& u, const Vector& v);
	static double angle(const Vector& u, const Vector& v);
	static inline double len(const Vector& u);
	static double distance(const Vector& u, const Vector& v);
	static Vector normal(const Vector& u);
	
	void Reset();
	void Clamp(float min, float max);
};

struct Ray {
	Vector point, dir;
};

struct Material {
	Vector color;
	float shinines, reflection_index;
	float spec_factor;
	
	void operator=(const Material& m) {
		color = m.color;
		shinines = m.shinines;
		reflection_index = m.reflection_index;
		spec_factor = m.spec_factor;
	}
};

struct Collision {
	Vector intersection, normal;
	float distance;
	Material mat;

	void operator=(const Collision& r) {
		intersection = r.intersection;
		normal = r.normal;
		mat = r.mat;
		distance = r.distance;
	}
};
