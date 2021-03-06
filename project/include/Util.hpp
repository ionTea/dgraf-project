#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <string>

float get_length(sf::Vector3f const & v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float get_length_sqared(sf::Vector3f const & v) {
	return (v.x*v.x + v.y*v.y + v.z*v.z);
}

float dot_product(sf::Vector3f const & v, sf::Vector3f const & u) {
	return v.x * u.x + v.y * u.y + v.z * u.z;
}

float angle(sf::Vector3f const & v, sf::Vector3f const & u) {
	return acos((dot_product(v, u)) / (get_length(v) * get_length(u)));

}
sf::Vector3f set_length(sf::Vector3f v, float length) {
	float l = get_length(v);
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
