#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <stack>

namespace zifmann {
namespace chess {

class State {
   public:
    virtual void Render(sf::RenderTarget &target) = 0;
    virtual void Update(float dt) = 0;
    virtual void ProcessEvent(const sf::Event &event) = 0;
    virtual void OnDestroy();
    virtual void OnQuit();
    virtual ~State() = default;
};

class StateManager {
   private:
    static std::stack<std::unique_ptr<State>> m_states;
    static bool m_removeRequest;
    static std::unique_ptr<State> m_stateToBeInserted;

   public:
    static void LoadState(std::unique_ptr<State> state, bool replace = true);
    static void UnloadState();
    static void Render(sf::RenderTarget &target);
    static void Update(float dt);
    static void ProcessEvent(const sf::Event &event);
    static void OnQuit();
};

}  // namespace chess
}  // namespace zifmann

#endif
