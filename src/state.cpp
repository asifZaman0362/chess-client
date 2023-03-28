#include "state.hpp"

#include <utility>

namespace zifmann {
namespace chess {

std::stack<std::unique_ptr<State>> StateManager::m_states;
bool StateManager::m_removeRequest;
std::unique_ptr<State> StateManager::m_stateToBeInserted;

void State::OnQuit() {}

void State::OnDestroy() {}

void StateManager::LoadState(std::unique_ptr<State> state, bool replace) {
    if (replace && !m_states.empty()) {
        m_removeRequest = true;
        m_stateToBeInserted = std::move(state);
    } else
        m_states.push(std::move(state));
}

void StateManager::UnloadState() {
    if (!m_states.empty()) {
        m_states.top().get()->OnDestroy();
        m_states.pop();
    }
}

void StateManager::Render(sf::RenderTarget &target) {
    if (!m_states.empty()) m_states.top().get()->Render(target);
}

void StateManager::Update(float dt) {
    if (m_removeRequest && !m_states.empty()) {
        m_removeRequest = false;
        UnloadState();
        if (m_stateToBeInserted) m_states.push(std::move(m_stateToBeInserted));
    }
    if (!m_states.empty()) m_states.top().get()->Update(dt);
}

void StateManager::ProcessEvent(const sf::Event &event) {
    if (!m_states.empty()) m_states.top().get()->ProcessEvent(event);
}

void StateManager::OnQuit() {
    if (!m_states.empty()) m_states.top().get()->OnQuit();
}

}  // namespace chess
}  // namespace zifmann
