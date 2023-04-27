#include "widgets.hpp"

#include "cursor.hpp"
#include "eventsystem.hpp"
#include "logger.hpp"

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
    // auto rect = GetRect();
    //  auto pos = m_sprite.getPosition();
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

sf::FloatRect InteractiveSprite::GetRect() {
    return m_sprite.getGlobalBounds();
}

TextField::TextField(sf::Texture *texture, sf::IntRect textureRect[3],
                     sf::IntRect offset, int anchor, ScaleMode scaleMode,
                     sf::Font *font, sf::String placeholder,
                     EventSystem &events)
    : InteractiveSprite(texture, textureRect, offset, anchor, scaleMode,
                        events),
      m_text(placeholder, font,
             sf::IntRect(10, 0, offset.width - 10, offset.height),
             Left | CentreV, Constant),
      m_string(placeholder) {
    events.AddRawKeyListener(this);
}

void TextField::OnKeyDown(sf::Keyboard::Key key) {
    // start initial timer
    if (!IsFocused()) return;
    if (key == sf::Keyboard::LShift || key == sf::Keyboard::RShift)
        m_shift = true;
    else {
        if (key == sf::Keyboard::BackSpace && !m_string.isEmpty()) {
            m_string.erase(m_string.getSize() - 1, 1);
        }
        if (key == sf::Keyboard::LShift) {
            m_shift = true;
        } else {
            if (key <= 25) {
                m_string += (char)(key + (m_shift ? 'A' : 'a'));
            } else if (25 < key && key < 34) {
                if (!m_shift) m_string += (char)(key + int('0'));
            }
        }
        m_text.GetText().setString(m_string);
    }
}

void TextField::OnKeyUp(sf::Keyboard::Key key) {
    if (!IsFocused()) return;
    if (key == sf::Keyboard::LShift || key == sf::Keyboard::RShift)
        m_shift = false;
}

void TextField::Update(float dt) {}

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

void TextField::Calculate(const sf::IntRect &parent) {
    InteractiveSprite::Calculate(parent);
    auto rect = GetRect();
    m_text.Calculate(sf::IntRect(rect.left, rect.top, rect.width, rect.height));
}

std::string TextField::GetText() const { return m_string; }

}  // namespace chess
}  // namespace zifmann
