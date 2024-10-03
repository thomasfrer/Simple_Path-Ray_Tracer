#include <SFML/Graphics.hpp>
#include <vector>
#include <omp.h>
#include <iostream> // Only for debugging purposes at the moment.
#include "Library.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

const int numSamples = 1000;
const int maxDepth = 8;
const vec3 eye = vec3(0.f, 0.f, 5.f);

const float brightness = (2.f * 3.1415926f) * (2.f / float(numSamples));

vec3 pathTrace(Ray ray, std::vector<Object*> objects, int depth) {
	if (depth > maxDepth)
		return vec3(0.f);

	double minT = 10000000.f;
	HitInfo hi, finalHi;

	for (int i = 0; i < objects.size(); i++) {
		hi = objects[i]->hit(ray);
		
		if (hi.hit && hi.t < minT) {
			finalHi = hi;
			minT = hi.t;
		}
	}

	if (!finalHi.hit)
		return vec3(0.f);

	if (finalHi.emissive)
		return finalHi.color;

	float p = 0.8f;
	float randomNumber = random();

	if (randomNumber > p)
		return vec3(0.f);

	Ray randomRay;
	randomRay.origin = finalHi.hitLocation;
	randomRay.direction = importanceSampleHemisphere(finalHi.normal);

	float cosine = fabs(dot(-ray.direction, finalHi.normal));
	vec3 originalColor = finalHi.color;
	vec3 newColor = pathTrace(randomRay, objects, depth + 1) * cosine;
	vec3 color = newColor * originalColor;

	return color / p;

	return vec3(0.f);
}

int main() {
	std::vector<Object*> objects;

	Sphere sphere;
	sphere.center = vec3(0.0f, -0.7f, -0.5f);
	sphere.radius = 0.3f;
	sphere.color = vec3(2.f);
	sphere.emissive = false;
	objects.push_back(&sphere);

	// Floor
	Triangle floor1 = Triangle();
	floor1.v1 = vec3(1.f, -1.f, 1.f);
	floor1.v2 = vec3(-1.f, -1.f, -1.f);
	floor1.v3 = vec3(-1.f, -1.f, 1.f);
	floor1.color = vec3(1.f);
	objects.push_back(&floor1);

	Triangle floor2 = Triangle();
	floor2.v1 = vec3(1.f, -1.f, 1.f);
	floor2.v2 = vec3(1.f, -1.f, -1.f);
	floor2.v3 = vec3(-1.f, -1.f, -1.f);
	floor2.color = vec3(1.f);
	objects.push_back(&floor2);

	// Light source
	Triangle light1 = Triangle();
	light1.v1 = vec3(0.5f, 0.99f, 0.5f);
	light1.v2 = vec3(-0.5f, 0.99f, -0.5f);
	light1.v3 = vec3(-0.5f, 0.99f, 0.5f);
	light1.color = vec3(1.f);
	light1.emissive = true;
	objects.push_back(&light1);

	Triangle light2 = Triangle();
	light2.v1 = vec3(0.5f, 0.99f, 0.5f);
	light2.v2 = vec3(0.5f, 0.99f, -0.5f);
	light2.v3 = vec3(-0.5f, 0.99f, -0.5f);
	light2.color = vec3(1.f);
	light2.emissive = true;
	objects.push_back(&light2);

	// Ceiling
	Triangle ceiling1 = Triangle();
	ceiling1.v1 = vec3(1.f, 1.f, 1.f);
	ceiling1.v2 = vec3(-1.f, 1.f, -1.f);
	ceiling1.v3 = vec3(-1.f, 1.f, 1.f);
	ceiling1.color = vec3(1.f);
	objects.push_back(&ceiling1);

	Triangle ceiling2 = Triangle();
	ceiling2.v1 = vec3(1.f, 1.f, 1.f);
	ceiling2.v2 = vec3(1.f, 1.f, -1.f);
	ceiling2.v3 = vec3(-1.f, 1.f, -1.f);
	ceiling2.color = vec3(1.f);
	objects.push_back(&ceiling2);

	// Left wall
	Triangle left1 = Triangle();
	left1.v1 = vec3(-1.f, -1.f, -1.f);
	left1.v2 = vec3(-1.f, 1.f, 1.f);
	left1.v3 = vec3(-1.f, -1.f, 1.f);
	left1.color = vec3(1.f, 0.f, 0.f);
	objects.push_back(&left1);

	Triangle left2 = Triangle();
	left2.v1 = vec3(-1.f, -1.f, -1.f);
	left2.v2 = vec3(-1.f, 1.f, -1.f);
	left2.v3 = vec3(-1.f, 1.f, 1.f);
	left2.color = vec3(1.f, 0.f, 0.f);
	objects.push_back(&left2);

	// Right wall
	Triangle right1 = Triangle();
	right1.v1 = vec3(1.f, 1.f, 1.f);
	right1.v2 = vec3(1.f, -1.f, -1.f);
	right1.v3 = vec3(1.f, -1.f, 1.f);
	right1.color = vec3(0.f, 1.f, 0.f);
	objects.push_back(&right1);

	Triangle right2 = Triangle();
	right2.v1 = vec3(1.f, -1.f, -1.f);
	right2.v2 = vec3(1.f, 1.f, 1.f);
	right2.v3 = vec3(1.f, 1.f, -1.f);
	right2.color = vec3(0.f, 1.f, 0.f);
	objects.push_back(&right2);

	// Back wall
	Triangle back1 = Triangle();
	back1.v1 = vec3(1.f, -1.f, -1.f);
	back1.v2 = vec3(-1.f, 1.f, -1.f);
	back1.v3 = vec3(-1.f, -1.f, -1.f);
	back1.color = vec3(1.f);
	objects.push_back(&back1);

	Triangle back2 = Triangle();
	back2.v1 = vec3(1.f, -1.f, -1.f);
	back2.v2 = vec3(1.f, 1.f, -1.f);
	back2.v3 = vec3(-1.f, 1.f, -1.f);
	back2.color = vec3(1.f);
	objects.push_back(&back2);

	double* image = new double[WINDOW_WIDTH * WINDOW_HEIGHT * 3];
	memset(image, 0.0, sizeof(double) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
	double* p = image;

	// Initialize adaptive sampling variables
	double* varianceBuffer = new double[WINDOW_WIDTH * WINDOW_HEIGHT * 3]; // Store pixel variance
	int* sampleCount = new int[WINDOW_WIDTH * WINDOW_HEIGHT];             // Track samples per pixel
	memset(varianceBuffer, 0.0, sizeof(double) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
	memset(sampleCount, 0, sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cornell Box Ray Tracer");

	sf::Image sfImage;
	sfImage.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	sf::Texture texture;
	texture.loadFromImage(sfImage);

	sf::Sprite sprite;
	sprite.setTexture(texture);

	// Declaring variables to prevent numerous redeclarations inside the main while loop
	HitInfo hi, finalHi;
	Ray ray;
	float i = 0.f, j = 0.f, minT = 10000000.f, s = 0;
	int r = 0, g = 0, b = 0;
	vec3 color = vec3(0.f);

	while (window.isOpen()) {

		if (s < numSamples) {

			#pragma omp parallel for collapse(2) private(ray, hi, finalHi, color, minT, i, j)
			for (int y = 0; y < WINDOW_HEIGHT; y++) {
				for (int x = 0; x < WINDOW_WIDTH; x++) {
					int index = (y * WINDOW_WIDTH + x) * 3;

					minT = 10000000.f;
					finalHi.hit = false;
					hi.hit = false;
					color = vec3(0.f);

					i = (2.f * float(x) / float(WINDOW_WIDTH)) - 1.f;
					j = (2.f * float(WINDOW_HEIGHT - y) / float(WINDOW_HEIGHT)) - 1.f;

					ray.origin = vec3(i, j, 1.2f);
					ray.direction = normalize(ray.origin - eye);

					// Check object intersections
					for (int k = 0; k < objects.size(); k++) {
						hi = objects[k]->hit(ray);
						if (hi.hit && hi.t < minT) {
							finalHi = hi;
							minT = hi.t;
						}
					}

					// Shade the object based on hit result
					if (finalHi.hit) {
						if (finalHi.emissive) {
							color = finalHi.color;  // Direct light contribution
						}
						else {
							Ray randomRay;
							randomRay.origin = finalHi.hitLocation;
							randomRay.direction = importanceSampleHemisphere(finalHi.normal);  // Indirect light sampling

							vec3 originalColor = finalHi.color;
							vec3 newColor = pathTrace(randomRay, objects, 0);
							color = newColor * originalColor;
							color *= brightness;
						}
					}

					// Accumulate the pixel color
					#pragma omp critical
					{
						image[index] += color.r;
						image[index + 1] += color.g;
						image[index + 2] += color.b;
					}
				}
			}

			// Update the image for display
			p = image;
			for (int e = 0; e < WINDOW_HEIGHT; e++) {
				for (int f = 0; f < WINDOW_WIDTH; f++) {
					r = clamp(pow(*p++, 1.0f / 2.2f) * 255, 0.0, 255.0);  // Gamma correction
					g = clamp(pow(*p++, 1.0f / 2.2f) * 255, 0.0, 255.0);
					b = clamp(pow(*p++, 1.0f / 2.2f) * 255, 0.0, 255.0);

					sfImage.setPixel(f, e, sf::Color(r, g, b));
				}
			}

			texture.loadFromImage(sfImage);
			sprite.setTexture(texture);

			s++;  // Increment sample counter
		}

		// Handle window events
		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}
}