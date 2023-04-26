#include "widgets.hpp"

#include "cursor.hpp"

namespace zifmann {
namespace chess {

void MouseClickable::OnHoverEnter() {
    m_hovered = true;
    Cursor::SetPointer();
}

void MouseClickable::OnHoverLeave() {
    m_hovered = false;
    m_down = false;
    Cursor::SetNormal();
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
    } else if (!m_hovered && button == sf::Mouse::Left) {
        m_focus = false;
    }
}

void MouseClickable::SetAction(std::function<void()> action) {
    m_action = action;
    m_actionAdded = true;
}

void MouseClickable::OnTrigger() {
    m_focus = true;
    if (m_actionAdded) m_action();
}

bool MouseClickable::IsFocused() { return m_focus; }

SpriteButton::SpriteButton(sf::Texture *texture, sf::IntRect textureRect[3],
                           sf::IntRect offset, int anchor, ScaleMode scaleMode,
                           EventSystem &events)
    : InteractiveSprite(texture, textureRect, offset, anchor, scaleMode,
                        events) {}

sf::FloatRect SpriteButton::GetRect() { return m_sprite.getGlobalBounds(); }

LabeledButton::LabeledButton(sf::Texture *texture, sf::IntRect textureRect[3],
                             sf::IntRect offset, sf::IntRect labelOffset[3],
                             int anchor, ScaleMode scaleMode,
                             sf::String labelString, sf::Font *font,
                             EventSystem &events)
    : SpriteButton(texture, textureRect, offset, anchor, scaleMode, events),
      m_label(labelString, font, labelOffset[0], CentreH | CentreV, Constant),
      m_labelState{labelOffset[0], labelOffset[1], labelOffset[2]} {}

void LabeledButton::OnHoverEnter() {
    SpriteButton::OnHoverEnter();
    m_label.SetOffset(m_labelState[1]);
}

void LabeledButton::OnHoverLeave() {
    SpriteButton::OnHoverLeave();
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
    // auto pos = m_sprite.getPosition();
    auto size = m_sprite.getGlobalBounds();
    m_label.Calculate(
        sf::IntRect(size.left, size.top, size.width, size.height));
}

void LabeledButton::SetLabelColor(const sf::Color color) {
    m_label.GetText().setFillColor(color);
}

void LabeledButton::SetCharacterSize(int size) {
    m_label.SetCharacterSize(size);
}

InteractiveSprite::InteractiveSprite(sf::Texture *texture,
                                     sf::IntRect textureRect[3],
                                     sf::IntRect offset, int anchor,
                                     ScaleMode scaleMode, EventSystem &events)
    : CanvasSprite(texture, textureRect[0], offset, anchor, scaleMode),
      m_spriteStates{textureRect[0], textureRect[1], textureRect[2]} {
    events.AddMouseListener(this);
}

void InteractiveSprite::SetTextureRect(sf::IntRect rect[3]) {
    m_spriteStates[0] = rect[0];
    m_spriteStates[1] = rect[1];
    m_spriteStates[2] = rect[2];
    m_sprite.setTextureRect(m_spriteStates[0]);
}

void InteractiveSprite::OnHoverEnter() {
    MouseClickable::OnHoverEnter();
    m_sprite.setTextureRect(m_spriteStates[1]);
}

void InteractiveSprite::OnHoverLeave() {
    MouseClickable::OnHoverLeave();
    m_sprite.setTextureRect(m_spriteStates[0]);
}

void InteractiveSprite::OnPushed() {
    m_sprite.setTextureRect(m_spriteStates[2]);
}

void InteractiveSprite::OnReleased() {
    m_sprite.setTextureRect(m_spriteStates[0]);
}

void InteractiveSprite::SetTint(const sf::Color &color) {
    m_sprite.setColor(color);
}

void TextField::OnKeyDown(sf::Keyboard::Key key) {
    if (!IsFocused()) return;
    if (key == sf::Keyboard::LShift || key == sf::Keyboard::RShift)
        m_shift = true;
    else
        m_key = key;
}

void TextField::OnKeyUp(sf::Keyboard::Key key) {
    if (!IsFocused()) return;
    if (key == sf::Keyboard::LShift || key == sf::Keyboard::RShift)
        m_shift = false;
    else
        m_key = sf::Keyboard::KeyCount;
}

void TextField::Update() {
    if (m_key == sf::Keyboard::BackSpace && !m_string.isEmpty()) {
        m_string.erase(m_string.getSize() - 1, 1);
    }
    if (m_key == sf::Keyboard::LShift) {
        m_shift = true;
    } else {
        if (m_key <= 25) {
            m_string += (char)(m_key + (m_shift ? 'A' : 'a'));
        } else if (25 < m_key && m_key < 34) {
            if (!m_shift) m_string += (char)(m_key + int('0'));
        }
    }
}

void TextField::draw(sf::RenderTarget &target) const {
    InteractiveSprite::draw(target);
    m_text.draw(target);
}

void TextField::SetTextColor(const sf::Color color) {
    m_text.GetText().setFillColor(color);
}

void TextField::SetCharacterSize(int size) {
    m_text.GetText().setCharacterSize(size);
}

}  // namespace chess
}  // namespace zifmann
