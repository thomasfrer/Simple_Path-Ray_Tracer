#pragma once

#include <glm.hpp>
using namespace glm;

#define EPSILON 0.00001f

class HitInfo {
public:
	HitInfo() : t(0.f), hit(false), hitLocation(0.f), normal(0.f), color(0.f), emissive(false) {}

	float t;
	bool hit;
	vec3 hitLocation;
	vec3 normal;
	vec3 color;
	bool emissive;
};

class Ray {
public:	
	Ray() : origin(0.f), direction(0.f) {}

	vec3 origin;
	vec3 direction;
};

class Object {
public:
	Object() : color(0.f), emissive(false) {}

	vec3 color;
	bool emissive;

	virtual HitInfo hit(Ray ray) const = 0;
};

class Triangle : public Object {
public:
	Triangle() : v1(0.f), v2(0.f), v3(0.f) {}

	vec3 v1, v2, v3;

	HitInfo hit(Ray ray) const {
		HitInfo hi;

		vec3 originalNormal = normalize(cross(v2 - v1, v3 - v1));
		vec3 n = originalNormal;

		if (dot(n, ray.direction) > 0.f)
			n = -n;

		// Case 1 - Hit nothing
		if (fabs(dot(n, ray.direction)) < EPSILON)
			return hi;

		// Case 2 - Hit nothing
		float t = (dot(n, v1) - dot(ray.origin, n)) / dot(ray.direction, n);
		if (fabs(t) < 0.0005f)
			return hi;

		vec3 p = ray.origin + ray.direction * t;

		vec3 c1 = cross(v2 - v1, p - v1);
		vec3 c2 = cross(v3 - v2, p - v2);
		vec3 c3 = cross(v1 - v3, p - v3);

		// Case 3 - Not inside the triangle
		if (dot(c1, originalNormal) < 0 || dot(c2, originalNormal) < 0 || dot(c3, originalNormal) < 0)
			return hi;

		// We have hit something inside the triangle
		hi.hit = true;
		hi.t = t;
		hi.hitLocation = p;
		hi.normal = n;
		hi.color = color;
		hi.emissive = emissive;

		return hi;
	}
};

class Sphere : public Object {
public:
	Sphere() : radius(0.f), center(0.f) {}

	vec3 center;
	float radius;

	HitInfo hit(Ray ray) const {
		HitInfo hi;
		float t;

		vec3 temp = ray.origin - center;
		float a = dot(ray.direction, ray.direction);
		float b = 2.f * dot(temp, ray.direction);
		float c = dot(temp, temp) - radius * radius;

		float discriminant = b * b - 4 * a * c;

		if (discriminant > 0.f) {
			t = (-b - sqrt(discriminant)) / (2.f * a);

			if (t > EPSILON) {
				hi.hit = true;	
				hi.t = t;
				hi.normal = -normalize((ray.origin + t * ray.direction) - center);
				hi.hitLocation = ray.origin + t * ray.direction;
				hi.color = color;
				hi.emissive = emissive;

				return hi;
			}

			t = (-b - sqrt(discriminant)) / (2.f * a);
			if (t > EPSILON) {
				hi.hit = true;
				hi.t = t;
				hi.normal = -normalize((ray.origin + t * ray.direction) - center);
				hi.hitLocation = ray.origin + t * ray.direction;
				hi.color = color;
				hi.emissive = emissive;

				return hi;
			}
		}

		return hi;
	}
};

float random() {
	return ((float)rand() / RAND_MAX);
}

// Get random vector in hemisphere (half sphere)
vec3 randomVector() {
	vec3 d;

	do {
		d = 2.f * vec3(random(), random(), random()) - vec3(1.f);
	} while (dot(d, d) > 1.f);

	return normalize(d);
}

vec3 importanceSampleHemisphere(const vec3& normal) {
	float r1 = random();
    float r2 = random();

    float sinTheta = sqrt(1.0f - r2);
    float phi = 2 * 3.1415926f * r1;

    vec3 sample(sinTheta * cos(phi), sqrt(r2), sinTheta * sin(phi));

    // Create an orthonormal basis for the hemisphere around the normal
    vec3 up = abs(normal.z) < 0.999f ? vec3(0, 0, 1) : vec3(1, 0, 0);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    // Transform the sample to world coordinates
    return normalize(sample.x * tangent + sample.y * normal + sample.z * bitangent);
}
