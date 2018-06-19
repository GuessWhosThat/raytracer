#include "Object.h"

Light::Light() {
	intensity = 0;
	type = AMBIENT;
}

Light::Light(const Light& r) {
	color = r.color;
	position = r.position;
	intensity = r.intensity;
	type = r.type;
}

void Sphere::Set(const Vector& p, float r, const Material& m) {
	position = p;
	radius = r;
	mat = m;
}

bool Sphere::CalculateCollision(const Ray& ray, Collision* out) const {
	Vector m = ray.point - position;
	float b = 2.0f * Vector::dot(m, ray.dir);
	float c = Vector::dot(m, m) - radius * radius;

	if (c > 0 && b > 0) {
		return false;
	}
	float discr = b * b - 4 * c;

	if (discr < 0) {
		return false;
	}

	auto t = (-b - sqrt(discr))/2.0f;
	out->distance = t;
	out->intersection = ray.point + ray.dir * t;
	out->mat = mat;
	out->normal = Vector::normal(out->intersection - position);

	return true;
}

void Plane::Set(const Vector& n, const Vector& p, const Material& m) {
	mat = m;
	normal = Vector::normal(n);
	d = -Vector::dot(n, p);
}

bool Plane::CalculateCollision(const Ray& ray, Collision* out) const {
	double ndir = Vector::dot(ray.dir, normal);
	if (std::fabs(ndir) < 0.0001f) {
		return false;
	}
	double np = Vector::dot(ray.point, normal);
	double t = (- d - np) / ndir;
	if (t <= 0.0f) {
		return false;
	}
	out->mat = mat;
	out->distance = t;
	out->normal = normal;
	out->intersection = ray.point + ray.dir * t;
	return true;
}