#include "menu.hpp"

#include "assetmanager.hpp"
#include "logger.hpp"
#include "widgets.hpp"

namespace zifmann {
namespace chess {

MenuScene::MenuScene(sf::RenderWindow *window) {
    m_window = window;
    CreateUI();
    m_audioPlayer.setBuffer(*AssetManager::GetAudioClip("menu.ogg"));
    m_audioPlayer.setLoop(true);
    m_audioPlayer.play();
}

void MenuScene::CreateUI() {
    auto bg = new CanvasSprite(AssetManager::GetTexture("background.png"),
                               sf::IntRect(0, 0, 0, 0), sf::IntRect(0, 0, 0, 0),
                               Anchor::Left | Anchor::Top, ScaleMode::Scale);
    auto title = new CanvasText(
        "super mega chess", AssetManager::GetFont("kenneyblocks.ttf"),
        sf::IntRect(0, 50, 500, 100), Top | CentreH, Constant);
    auto subtitle =
        new CanvasText("The best chess game on the internet!",
                       AssetManager::GetFont("OpenSans.ttf"),
                       sf::IntRect(0, 0, 500, 100), Top | Left, Constant);

    sf::IntRect textureRect[] = {sf::IntRect(0, 0, 48, 16),
                                 sf::IntRect(48, 0, 48, 16),
                                 sf::IntRect(96, 0, 48, 16)};
    sf::IntRect labelOffset[] = {sf::IntRect(0, 0, 48, 16),
                                 sf::IntRect(0, 0, 0, 0),
                                 sf::IntRect(0, 0, 0, 0)};
    auto playButton = new LabeledButton(
        AssetManager::GetTexture("button.png"), textureRect,
        sf::IntRect(0, 16 * 4, 144, 16 * 3), labelOffset, CentreH | CentreV,
        Constant, "play", AssetManager::GetFont("OpenSans.ttf"));
    playButton->SetAction([]() -> void { log_debug("hello from button!"); });
    playButton->SetLabelColor(sf::Color::Black);
    auto exitButton = new LabeledButton(
        AssetManager::GetTexture("button.png"), textureRect,
        sf::IntRect(0, 0, 144, 16 * 3), labelOffset, CentreH | CentreV,
        Constant, "EXIT", AssetManager::GetFont("OpenSans.ttf"));
    exitButton->SetAction([]() -> void { log_debug("hello from button!"); });
    exitButton->SetLabelColor(sf::Color::Black);

    // m_canvas.AddChild(bg);
    // m_canvas.AddChild(title);
    m_canvas.AddChild(subtitle);
    // m_canvas.AddChild(playButton);
    m_canvas.AddChild(exitButton);
    m_canvas.UpdateSize(sf::Vector2u(1000, 1000));
}

void MenuScene::Render(sf::RenderTarget &target) { m_canvas.draw(target); }

void MenuScene::Update(float dt) {}

void MenuScene::ProcessEvent(const sf::Event &event) {
    m_eventSystem.ProcessEvent(event, *m_window);
}

}  // namespace chess
}  // namespace zifmann
