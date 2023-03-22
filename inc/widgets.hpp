#include "canvas.hpp"
#include "eventsystem.hpp"

namespace zifmann {
namespace chess {

class MouseClickable : MouseListener {
   private:
    bool m_hovered;

   public:
    void OnMouseButtonDown(sf::Mouse::Button button) override {}
    void OnMouseButtonUp(sf::Mouse::Button button) override {}
    void OnMouseMove(sf::Vector2i newPosition) override {}
    virtual void OnClicked() = 0;
};

class SpriteButton : CanvasItem, MouseClickable {
   private:
    bool m_hovered;
    CanvasSprite m_sprite;

   public:
    SpriteButton(sf::Texture *texture, sf::IntRect textureRect,
                 sf::IntRect offset, int anchor, ScaleMode scaleMode);
    void OnClicked() override;
};

class LabeledButton : CanvasItem, MouseClickable {
   private:
    CanvasSprite m_background;
    CanvasText m_label;
    sf::String m_labelString;

   public:
    LabeledButton();
};

}  // namespace chess
}  // namespace zifmann
