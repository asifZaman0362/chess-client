#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>

namespace zifmann {
namespace chess {

typedef sf::Vector2<unsigned int> Vector2ui;

class SpritesheetAnimation {
   private:
    sf::Sprite *m_sprite;
    Vector2ui m_frameSize;
    Vector2ui m_start;
    int m_frameCount;
    bool m_repeat;
    float m_frameInterval;

    float m_timer = 0;
    int m_frameIdx = 0;
    bool m_playing = false;
    bool m_finished = false;

   public:
    SpritesheetAnimation() = default;
    bool Create(sf::Sprite *sprite, const Vector2ui &frameSize,
                const Vector2ui &start, const int frameCount,
                const float frameInterval, bool repeat = true);
    void Play();
    void Pause();
    void Reset();
    void Update(float dt);
};

}  // namespace chess
}  // namespace zifmann

#endif
