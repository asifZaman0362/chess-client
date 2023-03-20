#include "animation.hpp"

namespace zifmann {
    namespace chess {

        bool SpritesheetAnimation::Create(sf::Sprite *sprite, const Vector2ui& frameSize,
                const Vector2ui& start, const int frameCount, const float frameInterval, bool repeat) 
        {
            if (!sprite) return false;
            auto texture = sprite->getTexture();
            if (texture) {
                auto area = texture->getSize() - m_start;
                if (area.x >= frameSize.x * frameCount && area.y >= frameSize.y) {
                    m_sprite = sprite;
                    m_frameSize = frameSize;
                    m_start = start;
                    m_frameCount = frameCount;
                    m_frameInterval = frameInterval;
                    m_repeat = repeat;
                    return true;
                }
                return false;
            } else return false;
        }

        void SpritesheetAnimation::Play() {
            m_playing = true;
        }

        void SpritesheetAnimation::Pause() {
            m_playing = false;
        }

        void SpritesheetAnimation::Reset() {
            m_playing = false;
            m_timer = 0;
            m_sprite->setTextureRect(sf::IntRect(m_start.x, m_start.y, m_frameSize.x, m_frameSize.y));
        }

        void SpritesheetAnimation::Update(float dt) {
            if (!m_playing || m_finished) return;
            m_timer += dt;
            if (m_timer > m_frameInterval) {
                m_timer = 0;
                if (m_frameIdx >= m_frameCount - 1) {
                    if (m_repeat) m_frameIdx = 0;
                    else return;
                } else {
                    m_frameIdx++;
                }
            } else return;
            auto rect = m_sprite->getTextureRect();
            rect.left = m_start.x + m_frameSize.x * m_frameIdx;
            m_sprite->setTextureRect(rect);
        }

    }
}
