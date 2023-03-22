#include "widgets.hpp"

namespace zifmann {
namespace chess {

SpriteButton::SpriteButton(sf::Texture *texture, sf::IntRect textureRect,
                           sf::IntRect offset, int anchor, ScaleMode scaleMode)
    : CanvasItem(offset, anchor, scaleMode) {}

}  // namespace chess
}  // namespace zifmann
