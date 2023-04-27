#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <functional>
#include <vector>

#include "canvas.hpp"
#include "eventsystem.hpp"

namespace zifmann {
namespace chess {

class MouseClickable : public MouseListener {
   private:
    bool m_hovered;
    bool m_down;
    std::function<void()> m_action;
    bool m_actionAdded = false;
    bool m_focus = false;

   public:
    virtual void OnMouseButtonDown(sf::Mouse::Button button) override;
    virtual void OnMouseButtonUp(sf::Mouse::Button button) override;
    virtual void OnMouseMove(sf::Vector2i newPosition) override;
    virtual void OnHoverEnter();
    virtual void OnHoverLeave();
    virtual void SetAction(std::function<void()>);
    virtual sf::FloatRect GetRect() = 0;
    virtual void OnTrigger();
    virtual void OnPushed() = 0;
    virtual void OnReleased() = 0;
    bool IsFocused();
};

class InteractiveSprite : public CanvasSprite, public MouseClickable {
   protected:
    sf::IntRect m_spriteStates[3];

   public:
    InteractiveSprite(sf::Texture *texture, sf::IntRect textureRect[3],
                      sf::IntRect offset, int anchor, ScaleMode scaleMode,
                      EventSystem &events);
    virtual void OnHoverEnter() override;
    virtual void OnHoverLeave() override;
    virtual void OnPushed() override;
    virtual void OnReleased() override;
    virtual sf::FloatRect GetRect() override;
    void SetTextureRect(sf::IntRect rect[3]);
    void SetTint(const sf::Color &);
};

class SpriteButton : public InteractiveSprite {
   public:
    SpriteButton(sf::Texture *texture, sf::IntRect textureRect[3],
                 sf::IntRect offset, int anchor, ScaleMode scaleMode,
                 EventSystem &events);
    // virtual void draw(sf::RenderTarget &target) const override;
};

class LabeledButton : public SpriteButton {
   private:
    CanvasText m_label;
    sf::String m_labelString;
    sf::IntRect m_labelState[3];

   public:
    LabeledButton(sf::Texture *texture, sf::IntRect textureRect[3],
                  sf::IntRect offset, sf::IntRect labelOffset[3], int anchor,
                  ScaleMode scaleMode, sf::String labelString, sf::Font *font,
                  EventSystem &events);
    void SetLabelColor(const sf::Color color);
    void SetCharacterSize(int size);
    virtual void OnHoverEnter() override;
    virtual void OnHoverLeave() override;
    virtual void OnPushed() override;
    virtual void OnReleased() override;
    virtual void draw(sf::RenderTarget &target) const override;
    virtual void Calculate(const sf::IntRect &parent) override;
};

class TextField : public InteractiveSprite, public RawKeyListener {
   private:
    bool m_shift;
    CanvasText m_text;
    sf::String m_string;

   public:
    TextField(sf::Texture *texture, sf::IntRect textureRect[3],
              sf::IntRect offset, int anchor, ScaleMode scaleMode,
              sf::Font *font, sf::String placeholder, EventSystem &events);
    virtual void Update(float dt) override;
    void SetTextColor(const sf::Color color);
    void SetCharacterSize(int size);
    std::string GetText() const;
    virtual void OnPushed() override{};
    virtual void OnKeyDown(sf::Keyboard::Key key) override;
    virtual void OnKeyUp(sf::Keyboard::Key key) override;
    virtual void Calculate(const sf::IntRect &parent) override;
    virtual void draw(sf::RenderTarget &target) const override;
};

}  // namespace chess
}  // namespace zifmann

#endif
