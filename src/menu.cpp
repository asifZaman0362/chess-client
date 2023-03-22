#include "menu.hpp"

#include "assetmanager.hpp"

namespace zifmann {
namespace chess {

MenuScene::MenuScene() {
    auto bg = new CanvasSprite(AssetManager::GetTexture("background.png"),
                               sf::IntRect(0, 0, 0, 0), sf::IntRect(0, 0, 0, 0),
                               Anchor::Left | Anchor::Top, ScaleMode::Scale);
    auto title = new CanvasText(
        "super mega chess", AssetManager::GetFont("kenneyblocks.ttf"),
        sf::IntRect(0, 50, 500, 100), Top | CentreH, Constant);
    auto subtitle =
        new CanvasText("The best chess game on the internet!",
                       AssetManager::GetFont("kenneypixel.ttf"),
                       sf::IntRect(0, 100, 500, 100), Top | CentreH, Constant);
    m_audioPlayer.setBuffer(*AssetManager::GetAudioClip("menu.ogg"));
    m_audioPlayer.setLoop(true);
    m_audioPlayer.play();
    m_canvas.AddChild(bg);
    m_canvas.AddChild(title);
    m_canvas.AddChild(subtitle);
    m_canvas.UpdateSize(sf::Vector2u(1000, 1000));
}

void MenuScene::Render(sf::RenderTarget &target) { m_canvas.draw(target); }

void MenuScene::Update(float dt) {}

void MenuScene::ProcessEvent(const sf::Event &event) {}

}  // namespace chess
}  // namespace zifmann
