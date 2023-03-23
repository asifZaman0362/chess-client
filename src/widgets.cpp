#include "widgets.hpp"

#include "logger.hpp"

namespace zifmann {
namespace chess {

void MouseClickable::OnHoverEnter() { m_hovered = true; }

void MouseClickable::OnHoverLeave() {
    m_hovered = false;
    m_down = false;
}

void MouseClickable::OnMouseMove(sf::Vector2i newPosition) {
    if (GetRect().contains(sf::Vector2f(newPosition.x, newPosition.y))) {
        if (!m_hovered) {
            OnHoverEnter();
        }
    } else if (m_hovered)
        OnHoverLeave();
}

void MouseClickable::OnMouseButtonUp(sf::Mouse::Button button) {
    if (m_hovered && m_down && button == sf::Mouse::Left) {
        m_down = false;
        OnReleased();
        OnTrigger();
    }
}

void MouseClickable::OnMouseButtonDown(sf::Mouse::Button button) {
    if (m_hovered && button == sf::Mouse::Left) {
        OnPushed();
        m_down = true;
    }
}

void MouseClickable::SetAction(std::function<void()> action) {
    m_action = action;
}

void MouseClickable::OnTrigger() { m_action(); }

SpriteButton::SpriteButton(sf::Texture *texture, sf::IntRect textureRect[3],
                           sf::IntRect offset, int anchor, ScaleMode scaleMode)
    : CanvasSprite(texture, textureRect[0], offset, anchor, scaleMode),
      m_spriteStates{textureRect[0], textureRect[1], textureRect[2]} {}

void SpriteButton::OnHoverEnter() {
    MouseClickable::OnHoverEnter();
    m_sprite.setTextureRect(m_spriteStates[1]);
}

void SpriteButton::OnHoverLeave() {
    MouseClickable::OnHoverLeave();
    m_sprite.setTextureRect(m_spriteStates[0]);
}

void SpriteButton::OnPushed() { m_sprite.setTextureRect(m_spriteStates[2]); }

void SpriteButton::OnReleased() { m_sprite.setTextureRect(m_spriteStates[0]); }

sf::FloatRect SpriteButton::GetRect() { return m_sprite.getGlobalBounds(); }

LabeledButton::LabeledButton(sf::Texture *texture, sf::IntRect textureRect[3],
                             sf::IntRect offset, sf::IntRect labelOffset[3],
                             int anchor, ScaleMode scaleMode,
                             sf::String labelString, sf::Font *font)
    : SpriteButton(texture, textureRect, offset, anchor, scaleMode),
      m_label(labelString, font, labelOffset[0], CentreH | CentreV, Constant),
      m_labelState{labelOffset[0], labelOffset[1], labelOffset[2]} {}

void LabeledButton::OnHoverEnter() {
    SpriteButton::OnHoverEnter();
    m_label.SetOffset(m_labelState[1]);
}

void LabeledButton::OnHoverLeave() {
    SpriteButton::OnHoverEnter();
    m_label.SetOffset(m_labelState[0]);
}

void LabeledButton::OnPushed() {
    SpriteButton::OnPushed();
    m_label.SetOffset(m_labelState[2]);
}

void LabeledButton::OnReleased() {
    SpriteButton::OnReleased();
    m_label.SetOffset(m_labelState[0]);
}

void LabeledButton::draw(sf::RenderTarget &target) const {
    SpriteButton::draw(target);
    m_label.draw(target);
}

void LabeledButton::Calculate(const sf::IntRect &parent) {
    SpriteButton::Calculate(parent);
    auto rect = GetRect();
    m_label.Calculate(
        sf::IntRect(rect.left, rect.top, rect.width, rect.height));
}

void LabeledButton::SetLabelColor(const sf::Color color) {
    m_label.GetText().setFillColor(color);
}
}  // namespace chess
}  // namespace zifmann
