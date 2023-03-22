#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "canvas.hpp"
#include "state.hpp"

namespace zifmann {
namespace chess {

class MenuScene : public State {
   private:
    Canvas m_canvas;
    sf::Sound m_audioPlayer;

   public:
    MenuScene();
    void Render(sf::RenderTarget &target);
    void Update(float dt);
    void ProcessEvent(const sf::Event &event);
};

}  // namespace chess
}  // namespace zifmann
