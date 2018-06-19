#include "Utility.h"

Vector::Vector() {
	x = 0; y = 0; z = 0;
}

Vector::Vector(float X, float Y, float Z) {
	x = X; y = Y; z = Z;
}

Vector::Vector(const Vector& r) {
	x = r.x; y = r.y; z = r.z;
}

void Vector::operator=(const Vector& r) {
	x = r.x; y = r.y; z = r.z;
}

void Vector::operator+=(const Vector& r) {
	x += r.x; y += r.y; z += r.z;
}

void Vector::operator-=(const Vector& r) {
	x -= r.x; y -= r.y; z -= r.z;
}

void Vector::operator*=(float s) {
	x *= s; y *= s; z *= s;
}

void Vector::operator/=(float s) {
	x /= s; y /= s; z /= s;
}

Vector Vector::operator*(float s) const {
	return Vector(x * s, y * s, z * s);
}

Vector Vector::operator/(float s) const {
	return Vector(x / s, y / s, z / s);
}

Vector Vector::operator+(const Vector& r) const {
	return Vector(x + r.x, y + r.y, z + r.z);
}

Vector Vector::operator-(const Vector& r) const {
	return Vector(x - r.x, y - r.y, z - r.z);
}

inline double Vector::dot(const Vector& u, const Vector& v) {
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

double Vector::angle(const Vector& u, const Vector& v) {
	double len = dot(u, u)*dot(v, v);
	if (len == 0) {
		return 0;
	}
	return acos(dot(u, v) / sqrt(len));
}

inline double Vector::len(const Vector& u) {
	return sqrt(dot(u, u));
}

double Vector::distance(const Vector& u, const Vector& v) {
	return Vector::len(u - v);
}

Vector Vector::normal(const Vector& u) {
	double len = dot(u, u);
	if (len == 0) {
		return Vector();
	}
	return u / sqrt(len);
}

void Vector::Reset() {
	x = 0; y = 0; z = 0;
}

void Vector::Clamp(float min, float max) {
	if (x < min) { x = min; }
	if (y < min) { y = min; }
	if (z < min) { z = min; }
	if (x > max) { x = max; }
	if (y > max) { y = max; }
	if (z > max) { z = max; }
}

Vector::operator sf::Color() const {
	return sf::Color(x * 255, y * 255, z * 255);
}