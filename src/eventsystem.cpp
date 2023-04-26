#include "eventsystem.hpp"

namespace zifmann {
namespace chess {

EventSystem::~EventSystem() { ClearAllListeners(); }

void EventSystem::ProcessEvent(const sf::Event &event,
                               sf::RenderWindow &window) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            for (auto &listener : m_rawKeyListeners)
                listener->OnKeyDown(event.key.code);
            for (auto &listener : m_keyListeners[event.key.code])
                listener->OnKeyDown();
            break;
        case sf::Event::KeyReleased:
            for (auto &listener : m_rawKeyListeners)
                listener->OnKeyUp(event.key.code);
            for (auto &listener : m_keyListeners[event.key.code])
                listener->OnKeyDown();
            break;
        case sf::Event::MouseMoved:
            // log_debug("Mouse moved: %i %i", sf::Mouse::getPosition(window).x,
            // sf::Mouse::getPosition(window).y);
            for (auto &listener : m_mouseListeners)
                listener->OnMouseMove(sf::Mouse::getPosition(window));
            break;
        case sf::Event::MouseButtonPressed:
            for (auto &listener : m_mouseListeners)
                listener->OnMouseButtonDown(event.mouseButton.button);
            break;
        case sf::Event::MouseButtonReleased:
            for (auto &listener : m_mouseListeners)
                listener->OnMouseButtonUp(event.mouseButton.button);
            break;
        default:
            break;
    }
}

void EventSystem::AddKeyListener(KeyListener *listener, sf::Keyboard::Key key) {
    auto list = m_keyListeners[key];
    if (list.empty())
        list.push_back(listener);
    else if (std::find(list.begin(), list.end(), listener) == list.end()) {
        list.push_back(listener);
    }
}

void EventSystem::RemoveKeyListener(KeyListener *listener,
                                    sf::Keyboard::Key key) {
    auto list = m_keyListeners[key];
    if (list.empty()) return;
    auto pos = std::find(list.begin(), list.end(), listener);
    if (pos != list.end()) {
        list.erase(pos);
    }
}

void EventSystem::AddRawKeyListener(RawKeyListener *listener) {
    if (m_rawKeyListeners.empty())
        m_rawKeyListeners.push_back(listener);
    else if (std::find(m_rawKeyListeners.begin(), m_rawKeyListeners.end(),
                       listener) == m_rawKeyListeners.end()) {
        m_rawKeyListeners.push_back(listener);
    }
}

void EventSystem::RemoveRawKeyListener(RawKeyListener *listener) {
    if (m_rawKeyListeners.empty()) return;
    auto pos =
        std::find(m_rawKeyListeners.begin(), m_rawKeyListeners.end(), listener);
    if (pos != m_rawKeyListeners.end()) {
        m_rawKeyListeners.erase(pos);
    }
}

void EventSystem::AddMouseListener(MouseListener *listener) {
    if (m_mouseListeners.empty())
        m_mouseListeners.push_back(listener);
    else if (std::find(m_mouseListeners.begin(), m_mouseListeners.end(),
                       listener) == m_mouseListeners.end()) {
        m_mouseListeners.push_back(listener);
    }
}

void EventSystem::RemoveMouseListener(MouseListener *listener) {
    if (m_mouseListeners.empty()) return;
    auto pos =
        std::find(m_mouseListeners.begin(), m_mouseListeners.end(), listener);
    if (pos != m_mouseListeners.end()) {
        m_mouseListeners.erase(pos);
    }
}

void EventSystem::ClearAllListeners() {
    m_rawKeyListeners.clear();
    m_mouseListeners.clear();
    m_keyListeners.clear();
}

}  // namespace chess
}  // namespace zifmann
