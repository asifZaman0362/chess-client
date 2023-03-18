#include "state.hpp"
#include <utility>

void StateManager::LoadState(std::unique_ptr<State> state, bool replace) {
    if (replace && !m_states.empty()) m_states.pop();
    m_states.push(std::move(state));
}

void StateManager::UnloadState() {
    if (!m_states.empty()) m_states.pop();
}

void StateManager::Render() {
    m_states.top().get()->Render();
}

void StateManager::Update(float dt) {
    m_states.top().get()->Update(dt);
}

void StateManager::ProcessEvent(const sf::Event& event) {
    m_states.top().get()->ProcessEvent(event);
}
