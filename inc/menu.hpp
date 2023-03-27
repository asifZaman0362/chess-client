#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "canvas.hpp"
#include "eventsystem.hpp"
#include "state.hpp"

namespace zifmann {
namespace chess {

class MenuScene : public State {
   private:
    Canvas m_canvas;
    sf::Sound m_audioPlayer;
    EventSystem m_eventSystem;
    sf::RenderWindow *m_window;
    bool m_music, m_sound;

   public:
    MenuScene(sf::RenderWindow *window);
    void Render(sf::RenderTarget &target);
    void Update(float dt);
    void ProcessEvent(const sf::Event &event);
    // Non-scene methods
   private:
    void CreateUI();
    void DisableSound();
    void EnableSound();
    void DisableMusic();
    void EnableMusic();
    void ToggleMusic();
    void ToggleSound();
};

}  // namespace chess
}  // namespace zifmann
