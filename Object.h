#pragma once

#include "Utility.h"

#include <vector>
#include <memory>

class Object {
public:
	virtual bool CalculateCollision(const Ray& ray, Collision* out) const = 0;
};

enum LIGHT_TYPE {
	AMBIENT,
	DIRECTIONAL,
	POINT
};

struct Light {
	Vector position, color;
	double intensity;
	LIGHT_TYPE type;

	Light();
	Light(const Light&);
};

class Sphere :public Object {
protected:
	Material mat;
	Vector position;
	float radius;
public:
	void Set(const Vector& point, float r, const Material& m);
	bool CalculateCollision(const Ray& ray, Collision* out) const override;
};

class Plane :public Object {
protected:
	Material mat;
	Vector normal;
	float d;
public:
	void Set(const Vector& normal, const Vector& point, const Material& m);
	bool CalculateCollision(const Ray& ray, Collision* out) const override;
};

typedef std::vector<Light> LightContainer;
typedef std::vector<std::shared_ptr<Object>> ObjectContainer;