#include "math.hpp"

#include <cmath>

namespace zifmann::chess {

float Lerp(float a, float b, float t) { return t * b + (1 - t) * a; }

float Magnitude(const sf::Vector2f& vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float Distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

sf::Vector2f Normalized(const sf::Vector2f& vec) {
    auto mag = Magnitude(vec);
    return {vec.x / mag, vec.y / mag};
}

sf::Vector2f MoveTowards(const sf::Vector2f& current,
                         const sf::Vector2f& target, float delta) {
    auto dist = Distance(current, target);
    float decision = dist < delta;
    auto normalized = Normalized(current);
    auto deltaVec = normalized * delta;
    return ((current + deltaVec) * (float)(!decision) + (decision * target));
}

sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t) {
    return {Lerp(a.x, b.x, t), Lerp(a.y, b.y, t)};
}
}  // namespace zifmann::chess
