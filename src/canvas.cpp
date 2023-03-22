#include "canvas.hpp"

#include <algorithm>

#include "logger.hpp"

namespace zifmann {
namespace chess {

CanvasItem::CanvasItem(sf::IntRect rect, int anchor, ScaleMode scaleMode)
    : m_anchor(anchor),
      m_scale(sf::Vector2f(1, 1)),
      m_scaleMode(scaleMode),
      m_offset(rect) {}

void CanvasItem::Calculate(const sf::IntRect &parent) {
    // calculate the position first
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

CanvasText::CanvasText(const sf::String &string, sf::Font *font,
                       sf::IntRect offset, int anchor, ScaleMode scaleMode)
    : CanvasItem(offset, anchor, scaleMode), m_string(string), m_font(*font) {
    m_text.setFont(*font);
    m_text.setString(string);
}

int findBestFitCharacterSize(const sf::String &str, const sf::Font &font,
                             const sf::Vector2f &size) {
    int minSize = 1;
    int maxSize = 100;

    while (minSize < maxSize) {
        int csize = (minSize + maxSize) / 2;

        sf::Text text(str, font, csize);
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
    /*
     * This was my first approach and I guess this would work for monospaced
     * fonts but not for variable width fonts
     *
    auto csize = m_text.getCharacterSize(); size_t prev = 0, next = 0;
    size_t max = 0, lines = 1;
    while (true) {
        next = m_string.find('\n', prev);
        if (next == std::string::npos) {
            size_t len = m_string.getSize() - prev;
            if (len > max) max = len;
            break;
        }
        size_t len = next - prev;
        if (len > max) max = len;
        prev = next + 1;
        lines++;
    } */
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
