#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Window/Event.hpp>
#include <memory>
#include <stack>

class State {
    public:
        void Render();
        void Update(float dt);
        void ProcessEvent(const sf::Event& event);
};

class StateManager {
    private:
        std::stack<std::unique_ptr<State>> m_states;
    public:
        void LoadState(std::unique_ptr<State> state, bool replace = true);
        void UnloadState();
        void Render();
        void Update(float dt);
        void ProcessEvent(const sf::Event& event);
};

#endif
