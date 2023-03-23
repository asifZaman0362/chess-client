#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <functional>

#include "canvas.hpp"
#include "eventsystem.hpp"

namespace zifmann {
namespace chess {

class MouseClickable : MouseListener {
   private:
    bool m_hovered;
    bool m_down;
    std::function<void()> m_action;

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
};

class SpriteButton : public CanvasSprite, public MouseClickable {
   protected:
    sf::IntRect m_spriteStates[3];

   public:
    SpriteButton(sf::Texture *texture, sf::IntRect textureRect[3],
                 sf::IntRect offset, int anchor, ScaleMode scaleMode);
    virtual void OnHoverEnter() override;
    virtual void OnHoverLeave() override;
    virtual void OnPushed() override;
    virtual void OnReleased() override;
    virtual sf::FloatRect GetRect() override;
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
                  ScaleMode scaleMode, sf::String labelString, sf::Font *font);
    void SetLabelColor(const sf::Color color);
    virtual void OnHoverEnter() override;
    virtual void OnHoverLeave() override;
    virtual void OnPushed() override;
    virtual void OnReleased() override;
    virtual void draw(sf::RenderTarget &target) const override;
    virtual void Calculate(const sf::IntRect &parent) override;
};

}  // namespace chess
}  // namespace zifmann

#endif
