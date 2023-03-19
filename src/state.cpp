#include "state.hpp"
#include <utility>

namespace zifmann {
namespace chess {

std::stack<std::unique_ptr<State>> StateManager::m_states;

void StateManager::LoadState(std::unique_ptr<State> state, bool replace) {
    if (replace && !m_states.empty()) m_states.pop();
    m_states.push(std::move(state));
}

void StateManager::UnloadState() {
    if (!m_states.empty()) m_states.pop();
}

void StateManager::Render(sf::RenderTarget& target) {
    m_states.top().get()->Render(target);
}

void StateManager::Update(float dt) {
    m_states.top().get()->Update(dt);
}

void StateManager::ProcessEvent(const sf::Event& event) {
    m_states.top().get()->ProcessEvent(event);
}

}}
