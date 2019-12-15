#pragma once
#include <sstream>
#include <SFML/System/Vector2.hpp>

namespace sf {
	class Sprite;
	class Text;
}

template<typename T>
std::string toString(const T& value);

void centreOrigin(sf::Sprite& sprite);
void centreOrigin(sf::Text& text);

// Degree/radian conversion
float toDegree(float radian);
float toRadian(float degree);

// Vector operations
float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

// Random number generation
int	randomInt(int exclusiveMax);

#include "Utility.inl"