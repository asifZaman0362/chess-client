#include "menu.hpp"

#include "assetmanager.hpp"
#include "game.hpp"
#include "logger.hpp"
#include "prefs.hpp"
#include "widgets.hpp"

namespace zifmann {
namespace chess {

MenuScene::MenuScene(sf::RenderWindow *window) {
    m_window = window;
    CreateUI();
    m_audioPlayer.setBuffer(*AssetManager::GetAudioClip("menu.ogg"));
    m_audioPlayer.setLoop(true);
    if (!Prefs::GetInstance()->GetBool("musicdisabled")) {
        m_audioPlayer.play();
        m_music = true;
    }
}

void MenuScene::CreateUI() {
    auto bg = new CanvasSprite(AssetManager::GetTexture("background.png"),
                               sf::IntRect(0, 0, 0, 0), sf::IntRect(0, 0, 0, 0),
                               Anchor::Left | Anchor::Top, ScaleMode::Scale);
    auto title = new CanvasText(
        "super mega chess", AssetManager::GetFont("kenneyblocks.ttf"),
        sf::IntRect(0, 50, 0, 0), Top | CentreH, Constant);
    title->SetCharacterSize(50);
    auto subtitle =
        new CanvasText("The best chess game on the internet!",
                       AssetManager::GetFont("kenneypixel.ttf"),
                       sf::IntRect(0, 100, 0, 0), CentreH | Top, Constant);
    subtitle->SetCharacterSize(40);

    sf::IntRect textureRect[] = {sf::IntRect(0, 0, 48, 16),
                                 sf::IntRect(48, 0, 48, 16),
                                 sf::IntRect(96, 0, 48, 16)};
    sf::IntRect labelOffset[] = {sf::IntRect(0, 0, 0, 0),
                                 sf::IntRect(0, 2, 0, 0),
                                 sf::IntRect(0, 4, 0, 0)};
    sf::IntRect textureRectSquare[] = {sf::IntRect(0, 0, 16, 16),
                                       sf::IntRect(16, 0, 16, 16),
                                       sf::IntRect(32, 0, 16, 16)};
    auto playButton = new LabeledButton(
        AssetManager::GetTexture("button.png"), textureRect,
        sf::IntRect(0, 16 * 4, 144, 16 * 3), labelOffset, CentreH | CentreV,
        Constant, "play", AssetManager::GetFont("kenneypixel.ttf"),
        m_eventSystem);
    playButton->SetCharacterSize(40);
    playButton->SetAction([]() -> void { log_debug("hello from button!"); });
    playButton->SetLabelColor(sf::Color::Black);
    playButton->SetCharacterSize(40);
    playButton->SetAction([]() -> void { log_debug("hello from button!"); });
    playButton->SetLabelColor(sf::Color::Black);
    auto exitButton = new LabeledButton(
        AssetManager::GetTexture("button.png"), textureRect,
        sf::IntRect(0, 0, 144, 16 * 3), labelOffset, CentreH | CentreV,
        Constant, "exit", AssetManager::GetFont("kenneypixel.ttf"),
        m_eventSystem);
    exitButton->SetAction([]() -> void { Game::Quit(); });
    exitButton->SetLabelColor(sf::Color::White);
    exitButton->SetCharacterSize(40);
    exitButton->SetTint(sf::Color(255, 100, 80));
    auto musicButton =
        new SpriteButton(AssetManager::GetTexture("music.png"),
                         textureRectSquare, sf::IntRect(-30, -64, 48, 48),
                         CentreH | CentreV, Constant, m_eventSystem);
    musicButton->SetAction([this]() -> void { ToggleMusic(); });
    auto soundButton =
        new SpriteButton(AssetManager::GetTexture("sound.png"),
                         textureRectSquare, sf::IntRect(30, -64, 48, 48),
                         CentreH | CentreV, Constant, m_eventSystem);

    m_canvas.AddChild(bg);
    m_canvas.AddChild(title);
    m_canvas.AddChild(subtitle);
    m_canvas.AddChild(playButton);
    m_canvas.AddChild(exitButton);
    m_canvas.AddChild(musicButton);
    m_canvas.AddChild(soundButton);
    m_canvas.UpdateSize(sf::Vector2u(1000, 1000));
}

void MenuScene::Render(sf::RenderTarget &target) { m_canvas.draw(target); }

void MenuScene::Update(float dt) {}

void MenuScene::ProcessEvent(const sf::Event &event) {
    m_eventSystem.ProcessEvent(event, *m_window);
}

void MenuScene::DisableMusic() {
    m_audioPlayer.stop();
    Prefs::GetInstance()->SetBool("musicdisabled", true)->Save();
}

void MenuScene::EnableMusic() {
    m_audioPlayer.play();
    Prefs::GetInstance()->SetBool("musicdisabled", false)->Save();
}

void MenuScene::ToggleMusic() {
    m_music = !m_music;
    if (m_music)
        EnableMusic();
    else
        DisableMusic();
}

}  // namespace chess
}  // namespace zifmann
