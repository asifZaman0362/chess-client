#include <SFML/Graphics.hpp>
#include <vector>

namespace zifmann {
namespace chess {

enum Anchor {
    CentreH = 0,
    Left = 1,
    Right = 2,
    CentreV = 4,
    Top = 8,
    Bottom = 16
};

enum ScaleMode { Scale, Constant };

class CanvasItem {
   private:
    Anchor m_anchor;
    ScaleMode m_scaleMode;
    sf::Vector2f m_scale;
    sf::IntRect m_offset;
    std::unique_ptr<sf::Drawable> m_drawable;

   public:
    CanvasItem(std::unique_ptr<sf::Drawable> drawable, sf::IntRect rect,
               Anchor anchor, ScaleMode scaleMode);
    void Calculate(const sf::IntRect &parent);
};

class NineSliceSprite : public sf::Drawable {
   private:
    sf::Sprite m_inner;
    sf::Texture m_texture;
    sf::Vector2u m_sliceSize;
    sf::Vector2u m_sliceGap;

   public:
    NineSliceSprite(sf::Texture *texture, sf::Vector2u size, sf::Vector2u gap);
    void draw(sf::RenderTarget &target, sf::RenderStates &states);
};

class Canvas : public sf::Drawable {
   private:
    std::vector<std::unique_ptr<CanvasItem>> m_items;

   public:
    void AddChild(std::unique_ptr<CanvasItem> item);
    void draw(sf::RenderTarget &target, sf::RenderStates &states);
};

}  // namespace chess
}  // namespace zifmann
