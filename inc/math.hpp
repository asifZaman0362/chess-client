#ifndef MATH_HPP
#define MATH_HPP

#include <SFML/System/Vector2.hpp>

namespace zifmann::chess {

float Distance(const sf::Vector2f& a, const sf::Vector2f& b);

float Magnitude(const sf::Vector2f& vec);

sf::Vector2f Normalized(const sf::Vector2f& vec);

sf::Vector2f MoveTowards(const sf::Vector2f& current,
                         const sf::Vector2f& target, float delta);

float Lerp(float a, float b, float t);

sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t);

}  // namespace zifmann::chess

#endif
