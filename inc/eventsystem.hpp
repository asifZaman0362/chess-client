#ifndef EVENTSYSTEM_HPP
#define EVENTSYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace zifmann {
namespace chess {

class KeyListener {
   public:
    virtual void OnKeyDown() = 0;
    virtual void OnKeyUp() = 0;
};

class RawKeyListener {
   public:
    virtual void OnKeyDown(sf::Keyboard::Key key) = 0;
    virtual void OnKeyUp(sf::Keyboard::Key key) = 0;
};

class MouseListener {
   public:
    virtual void OnMouseButtonDown(sf::Mouse::Button button) = 0;
    virtual void OnMouseButtonUp(sf::Mouse::Button button) = 0;
    virtual void OnMouseMove(sf::Vector2i newPosition) = 0;
};

class EventSystem {
    std::unordered_map<sf::Keyboard::Key, std::vector<KeyListener *>>
        m_keyListeners;
    std::vector<RawKeyListener *> m_rawKeyListeners;
    std::vector<MouseListener *> m_mouseListeners;

   public:
    EventSystem() = default;
    void ProcessEvent(const sf::Event &event, sf::RenderWindow &window);
    void AddKeyListener(KeyListener *listener, sf::Keyboard::Key key);
    void RemoveKeyListener(KeyListener *listener, sf::Keyboard::Key key);
    void AddRawKeyListener(RawKeyListener *listener);
    void RemoveRawKeyListener(RawKeyListener *listener);
    void AddMouseListener(MouseListener *listener);
    void RemoveMouseListener(MouseListener *listener);
};

}  // namespace chess
}  // namespace zifmann

#endif
