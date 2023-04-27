#include "canvas.hpp"

#include <algorithm>
#include <cmath>

namespace zifmann {
namespace chess {

CanvasItem::CanvasItem(sf::IntRect rect, int anchor, ScaleMode scaleMode)
    : m_anchor(anchor),
      m_scale(sf::Vector2f(1, 1)),
      m_scaleMode(scaleMode),
      m_offset(rect) {}

void CanvasItem::SetOffset(const sf::IntRect &offset) {
    m_offset = offset;
    Calculate(m_parent);
}

void CanvasItem::Calculate(const sf::IntRect &parent) {
    // calculate the position first
    m_parent = parent;
    sf::Vector2i parentPosition, pos;
    parentPosition.x = parent.width / 2 + parent.left;
    parentPosition.y = parent.height / 2 + parent.top;
    if (m_scaleMode == Constant) {
        SetSize(sf::Vector2i(m_offset.width, m_offset.height));
        if ((m_anchor & Top) == Top) {
            pos.y = parent.top + m_offset.top;
        } else if ((m_anchor & Bottom) == Bottom) {
            pos.y = parent.top + parent.height - m_offset.top;
        } else if ((m_anchor & CentreV) == CentreV) {
            pos.y = parentPosition.y - m_offset.top - GetPixelSize().y / 2;
        }

        if ((m_anchor & Left) == Left) {
            pos.x = parent.left + m_offset.left;
        } else if ((m_anchor & Right) == Right) {
            pos.x = parent.left + parent.width - m_offset.left;
        } else if ((m_anchor & CentreH) == CentreH) {
            pos.x = parentPosition.x - m_offset.left - GetPixelSize().x / 2;
        }
        SetPosition(pos);
    } else {
        // here width is the right offset and height, the bottom offset
        int width = parent.width - (m_offset.left + m_offset.width);
        int height = parent.height - (m_offset.top + m_offset.height);
        pos.x = parent.left + m_offset.left;
        pos.y = parent.top + m_offset.top;
        SetSize(sf::Vector2i(width, height));
        SetPosition(pos);
    }
}

CanvasSprite::CanvasSprite(sf::Texture *texture, sf::IntRect textureRect,
                           sf::IntRect rect, int anchor, ScaleMode scaleMode)
    : CanvasItem(rect, anchor, scaleMode) {
    m_sprite.setTexture(*texture);
    if (textureRect.width != 0 && textureRect.height != 0)
        m_sprite.setTextureRect(textureRect);
}

void CanvasSprite::SetSize(sf::Vector2i size) {
    auto orig = m_sprite.getTextureRect();
    auto x = size.x / (float)orig.width;
    auto y = size.y / (float)orig.height;
    m_sprite.setScale(x, y);
}

void CanvasSprite::SetPosition(sf::Vector2i position) {
    m_sprite.setPosition(position.x, position.y);
}

sf::Vector2f CanvasSprite::GetPixelSize() const {
    auto rect = m_sprite.getGlobalBounds();
    return sf::Vector2f(rect.width, rect.height);
}

void CanvasSprite::draw(sf::RenderTarget &target) const {
    target.draw(m_sprite);
}

void CanvasItem::Update(float dt) {}

CanvasText::CanvasText(const sf::String &string, sf::Font *font,
                       sf::IntRect offset, int anchor, ScaleMode scaleMode)
    : CanvasItem(offset, anchor, scaleMode), m_string(string), m_font(font) {
    m_text.setFont(*font);
    m_text.setString(string);
}

sf::Vector2f round(const sf::Vector2f vector) {
    return sf::Vector2f{std::round(vector.x), std::round(vector.y)};
}

sf::Vector2i roundi(const sf::Vector2f vector) {
    return sf::Vector2i{(int)std::round(vector.x), (int)std::round(vector.y)};
}

sf::Vector2f getPosition(sf::FloatRect rect) {
    return sf::Vector2f(rect.left, rect.top);
}

sf::Vector2f getPosition(sf::IntRect rect) {
    return sf::Vector2f(rect.left, rect.top);
}

sf::Vector2f getSize(sf::FloatRect rect) {
    return sf::Vector2f(rect.width, rect.height);
}

sf::Vector2f getSize(sf::IntRect rect) {
    return sf::Vector2f(rect.width, rect.height);
}

void CanvasText::SetCharacterSize(const unsigned int size) {
    m_text.setCharacterSize(size);
}

void CanvasText::Calculate(const sf::IntRect &parent) {
    m_parent = parent;
    if (m_scaleMode == Scale) {  // best-fit
        auto size =
            getSize(m_parent) - sf::Vector2f(m_offset.left + m_offset.width,
                                             m_offset.top + m_offset.height);
        auto rounded = roundi(size);
        SetSize(rounded);
    }
    auto center = sf::Vector2f{m_text.getGlobalBounds().width,
                               m_text.getGlobalBounds().height} /
                  2.f;
    auto localBounds = center + sf::Vector2f{m_text.getLocalBounds().left,
                                             m_text.getLocalBounds().top};
    auto rounded = round(localBounds);
    float x = 0;
    float y = 0;
    float ox = 0;
    float oy = 0;
    if ((m_anchor & Top) == Top) {
        y = m_offset.top + m_parent.top;
    } else if ((m_anchor & Bottom) == Bottom) {
        y = m_parent.top + m_parent.height - m_offset.top;
        oy = getPosition(m_text.getLocalBounds()).y +
             getSize(m_text.getGlobalBounds()).y;
    } else if ((m_anchor & CentreV) == CentreV) {
        y = m_offset.top + m_parent.top + m_parent.height / 2.0f;
        oy = rounded.y;
    }

    if ((m_anchor & Left) == Left) {
        x = m_offset.left + m_parent.left;
    } else if ((m_anchor & Right) == Right) {
        x = -m_offset.left + m_parent.left + m_parent.width;
        ox = getPosition(m_text.getLocalBounds()).x +
             getSize(m_text.getGlobalBounds()).x;
    } else if ((m_anchor & CentreH) == CentreH) {
        x = m_offset.left + m_parent.left + m_parent.width / 2.0f;
        ox = rounded.x;
    }
    m_text.setOrigin(sf::Vector2(ox, oy));
    m_text.setPosition(x, y);
}

sf::FloatRect CanvasText::GetGlobalBounds() const {
    return m_text.getGlobalBounds();
}

sf::Text &CanvasText::GetText() { return m_text; }

int findBestFitCharacterSize(const sf::String &str, const sf::Font *font,
                             const sf::Vector2f &size) {
    int minSize = 1;
    int maxSize = 100;

    while (minSize < maxSize) {
        int csize = (minSize + maxSize) / 2;

        sf::Text text(str, *font, csize);
        sf::FloatRect bounds = text.getGlobalBounds();

        if (bounds.width <= size.x && bounds.height <= size.y) {
            minSize = csize + 1;
        } else {
            maxSize = csize - 1;
        }
    }

    return minSize;
}

void CanvasText::SetSize(sf::Vector2i size) {
    int characterSize = findBestFitCharacterSize(m_string, m_font,
                                                 sf::Vector2f(size.x, size.y));
    m_text.setCharacterSize(characterSize);
}

void CanvasText::SetPosition(sf::Vector2i position) {
    m_text.setPosition(position.x, position.y);
}

sf::Vector2f CanvasText::GetPixelSize() const {
    auto rect = m_text.getGlobalBounds();
    return sf::Vector2f(rect.width, rect.height);
}

void CanvasText::draw(sf::RenderTarget &target) const { target.draw(m_text); }

void Canvas::draw(sf::RenderTarget &target) {
    for (auto &item : m_items) {
        item->draw(target);
    }
}

void Canvas::SetCustomCursor(sf::Texture *cursorTexture, sf::IntRect rect) {
    m_customCursor = true;
    m_cursor.setTexture(*cursorTexture);
    m_cursor.setTextureRect(rect);
    m_cursorSize = rect;
}

void Canvas::UpdateCursorState(bool pointer) {
    auto s = m_cursorSize;
    if (pointer) {
        s.left = m_cursorSize.width;
    }
    m_cursor.setTextureRect(s);
}

void Canvas::Update(float dt) {
    for (auto child : m_items) {
        child->Update(dt);
    }
}

Canvas::Canvas(Canvas &other) {
    for (auto &item : other.m_items) {
        m_items.push_back(item);
    }
}

Canvas::Canvas(Canvas &&other) {
    for (auto &item : other.m_items) {
        m_items.push_back(item);
    }
    other.m_items.clear();
}

Canvas::~Canvas() {
    for (auto &item : m_items) delete item;
    m_items.clear();
}

void Canvas::AddChild(CanvasItem *item) { m_items.push_back(item); }

void Canvas::UpdateSize(sf::Vector2u size) {
    for (auto &item : m_items) {
        item->Calculate(sf::IntRect(0, 0, size.x, size.y));
    }
}

}  // namespace chess
}  // namespace zifmann
