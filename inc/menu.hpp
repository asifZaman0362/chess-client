#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "canvas.hpp"
#include "eventsystem.hpp"
#include "state.hpp"

namespace zifmann {
namespace chess {

class SpriteButton;

class MenuScene : public State {
   private:
    Canvas m_canvas;
    sf::Sound m_audioPlayer;
    EventSystem m_eventSystem;
    sf::RenderWindow *m_window;
    bool m_music, m_sound;

   public:
    MenuScene(sf::RenderWindow *window);
    ~MenuScene();
    void Render(sf::RenderTarget &target);
    void Update(float dt);
    void ProcessEvent(const sf::Event &event);
    // Non-scene methods
   private:
    void CreateUI();
    void DisableSound(SpriteButton *);
    void EnableSound(SpriteButton *);
    void DisableMusic(SpriteButton *);
    void EnableMusic(SpriteButton *);
    void ToggleMusic(SpriteButton *);
    void ToggleSound(SpriteButton *);
    void LoadGameScene();
};

}  // namespace chess
}  // namespace zifmann
