#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace zifmann {
namespace chess {

class Cursor {
   private:
    static sf::Sprite* m_sprite;
    static sf::RenderWindow* m_window;

   public:
    Cursor() = default;
    // This function sets the default window for use with system cursors
    // Can be ommitted if no calls to any cursor functions are made
    static void SetWindow(sf::RenderWindow* window);
    // This function is to be called when the window is created or later before
    // processing any scenes, as it sets up the initial state It may be avoided
    // in case no custom cursor is required but any call to setting the cursor
    // type will segfault without calling this function at least once
    static void Setup(sf::RenderWindow* window, const sf::Texture* texture,
                      const sf::IntRect& textureRect);
    // This function updates the images used by the cursor
    static void SetPixels(const sf::Texture* image, const sf::IntRect& rect);
    // This function sets the scale of the sprite to match the given size
    static void SetSize(sf::Vector2f size);
    // This function sets the cursor state to normal and updates the pixels to
    // the appropriate texture
    static void SetNormal();
    // This function sets the cursor state to pointer and updates the pixels to
    // match
    static void SetPointer();
    // This function updates the position of the sprite
    static void Update();
    // This function draws the cursor
    static void Draw(sf::RenderTarget& target);
};

}  // namespace chess
}  // namespace zifmann
#endif
