#include "cursor.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

namespace zifmann {
namespace chess {

sf::RenderWindow* Cursor::m_window;
sf::Sprite* Cursor::m_sprite = nullptr;

void Cursor::SetWindow(sf::RenderWindow* window) { m_window = window; }

void Cursor::Setup(sf::RenderWindow* window, const sf::Texture* image,
                   const sf::IntRect& textureRect) {
    m_sprite = new sf::Sprite();
    window->setMouseCursorVisible(false);
    m_window = window;
    SetPixels(image, textureRect);
    SetNormal();
}

void Cursor::SetSize(sf::Vector2f size) {
    auto x = size.x / m_sprite->getGlobalBounds().width;
    auto y = size.y / m_sprite->getGlobalBounds().height;
    m_sprite->setScale(x, y);
}

void Cursor::SetPixels(const sf::Texture* image, const sf::IntRect& rect) {
    m_sprite->setTexture(*image);
    m_sprite->setTextureRect(rect);
}

void Cursor::SetNormal() {
    if (m_sprite) {
        auto rect = m_sprite->getTextureRect();
        m_sprite->setTextureRect(sf::IntRect(0, 0, rect.width, rect.height));
    } else {
        sf::Cursor cursor;
        cursor.loadFromSystem(sf::Cursor::Arrow);
        m_window->setMouseCursor(cursor);
    }
}

void Cursor::SetPointer() {
    if (m_sprite) {
        auto rect = m_sprite->getTextureRect();
        m_sprite->setTextureRect(
            sf::IntRect(rect.width, 0, rect.width, rect.height));
    } else {
        sf::Cursor cursor;
        cursor.loadFromSystem(sf::Cursor::Hand);
        m_window->setMouseCursor(cursor);
    }
}

void Cursor::Update() {
    if (!m_sprite) return;
    auto pos = sf::Mouse::getPosition(*m_window);
    m_sprite->setPosition(sf::Vector2f(pos.x, pos.y));
}

void Cursor::Draw(sf::RenderTarget& target) {
    if (m_sprite) target.draw(*m_sprite);
}

}  // namespace chess
}  // namespace zifmann
