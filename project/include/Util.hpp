#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <string>

float getLength(sf::Vector3f const & v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

sf::Vector3f setLength(sf::Vector3f v, float length) {
	float l = getLength(v);
	return sf::Vector3f(length * v.x / l, length * v.y / l, length * v.z / l);
}

void p(std::string s) {
	std::cout << s << std::endl;
}

void p(sf::Vector3f v) {
	std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

void p(float f) {
	std::cout << f << std::endl;
}

void p(int i) {
	std::cout << i << std::endl;
}
