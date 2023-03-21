#include "canvas.hpp"

namespace zifmann {
namespace chess {

CanvasItem::CanvasItem(std::unique_ptr<sf::Drawable> drawable, sf::IntRect rect,
                       Anchor anchor, ScaleMode scaleMode)
    : m_anchor(anchor),
      m_scale(sf::Vector2f(1, 1)),
      m_scaleMode(scaleMode),
      m_offset(rect),
      m_drawable(std::move(drawable)) {}

void CanvasItem::Calculate(const sf::IntRect &parent) {}

NineSliceSprite::NineSliceSprite(sf::Texture *tex, sf::Vector2u size,
                                 sf::Vector2u gap)
    : m_texture(*tex), m_sliceSize(size), m_sliceGap(gap) {}

}  // namespace chess
}  // namespace zifmann
