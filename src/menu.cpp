#include "menu.hpp"

#include <cstdlib>
#include <ctime>
#include <memory>
#include <sstream>
#include <variant>

#include "assetmanager.hpp"
#include "game.hpp"
#include "gamescene.hpp"
#include "logger.hpp"
#include "message.hpp"
#include "networkclient.hpp"
#include "networkplayer.hpp"
#include "prefs.hpp"
#include "state.hpp"
#include "widgets.hpp"

namespace zifmann {
namespace chess {

std::string GetRandomUsername() {
    std::stringstream stream;
    auto nouns = AssetManager::GetText("nouns.txt");
    auto adjs = AssetManager::GetText("adj.txt");
    srand(time(0));
    size_t idx = rand() % adjs->size();
    stream << adjs->at(idx);
    idx = rand() % nouns->size();
    stream << nouns->at(idx);
    int num = rand() % 500;
    stream << num;
    return stream.str();
}

void MenuScene::StartGame(std::string username) {
    auto message = network::OutgoingMessage(network::Login, username);
    network::NetworkManager::AddCallback(
        network::Result, [this, username](network::IncomingMessage message) {
            auto res = std::get<network::Result_t>(message.data);
            if (res.status == network::Result_t::Ok) {
                network::NetworkPlayer::PLAYER_USERNAME = username;
                auto queueMessage =
                    network::OutgoingMessage(network::Enqueue, "");
                network::NetworkManager::SendMessage(queueMessage);
            }
        });
    network::NetworkManager::AddCallback(
        network::GameStarted, [this](network::IncomingMessage message) {
            auto res = std::get<std::string>(message.data);
            if (res == "White") {
                network::NetworkPlayer::PLAYER_WHITE_PIECES = true;
            } else if (res == "Black") {
                network::NetworkPlayer::PLAYER_WHITE_PIECES = false;
            }
            LoadGameScene();
        });
    network::NetworkManager::SendMessage(message);
}

MenuScene::MenuScene(sf::RenderWindow *window) {
    m_window = window;
    m_audioPlayer.setBuffer(*AssetManager::GetAudioClip("menu.ogg"));
    m_audioPlayer.setLoop(true);
    if (!Prefs::GetInstance()->GetBool("musicdisabled")) {
        // m_audioPlayer.play();
        m_music = true;
    }
    m_sound = !Prefs::GetInstance()->GetBool("sounddisabled");
    CreateUI();
}

MenuScene::~MenuScene() { m_eventSystem.ClearAllListeners(); }

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
    sf::IntRect musicRect[] = {sf::IntRect(0, m_music ? 0 : 16, 16, 16),
                               sf::IntRect(16, m_music ? 0 : 16, 16, 16),
                               sf::IntRect(32, m_music ? 0 : 16, 16, 16)};
    sf::IntRect soundRect[] = {sf::IntRect(0, m_sound ? 0 : 16, 16, 16),
                               sf::IntRect(16, m_sound ? 0 : 16, 16, 16),
                               sf::IntRect(32, m_sound ? 0 : 16, 16, 16)};
    auto playButton = new LabeledButton(
        AssetManager::GetTexture("button.png"), textureRect,
        sf::IntRect(0, 16 * 4, 144, 16 * 3), labelOffset, CentreH | CentreV,
        Constant, "play", AssetManager::GetFont("kenneypixel.ttf"),
        m_eventSystem);
    playButton->SetCharacterSize(40);
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
        new SpriteButton(AssetManager::GetTexture("music.png"), musicRect,
                         sf::IntRect(-30, -64, 48, 48), CentreH | CentreV,
                         Constant, m_eventSystem);
    musicButton->SetAction(
        [this, musicButton]() -> void { ToggleMusic(musicButton); });
    auto soundButton =
        new SpriteButton(AssetManager::GetTexture("sound.png"), soundRect,
                         sf::IntRect(30, -64, 48, 48), CentreH | CentreV,
                         Constant, m_eventSystem);
    soundButton->SetAction(
        [this, soundButton]() -> void { ToggleSound(soundButton); });
    auto usernameLabel = new CanvasText(
        "Enter username:", AssetManager::GetFont("kenneypixel.ttf"),
        sf::IntRect(0, -180, 144, 32), CentreH | CentreV, Constant);

    sf::IntRect textfieldTexRect[] = {sf::IntRect(0, 0, 96, 16),
                                      sf::IntRect(96, 0, 96, 16),
                                      sf::IntRect(192, 0, 96, 16)};
    auto usernameInput = new TextField(
        AssetManager::GetTexture("textfield.png"), textfieldTexRect,
        sf::IntRect(0, 128, 144 * 2, 16 * 3), CentreH | CentreV,
        ScaleMode::Constant, AssetManager::GetFont("kenneypixel.ttf"),
        GetRandomUsername(), m_eventSystem);
    usernameInput->SetTextColor(sf::Color::Black);

    playButton->SetAction([this, usernameInput]() -> void {
        auto username = usernameInput->GetText();
        StartGame(username);
    });

    m_canvas.AddChild(bg);
    m_canvas.AddChild(title);
    m_canvas.AddChild(subtitle);
    m_canvas.AddChild(playButton);
    m_canvas.AddChild(exitButton);
    m_canvas.AddChild(musicButton);
    m_canvas.AddChild(soundButton);
    m_canvas.AddChild(usernameLabel);
    m_canvas.AddChild(usernameInput);
    m_canvas.UpdateSize(sf::Vector2u(1000, 1000));
}

void MenuScene::Render(sf::RenderTarget &target) { m_canvas.draw(target); }

void MenuScene::Update(float dt) { m_canvas.Update(dt); }

void MenuScene::ProcessEvent(const sf::Event &event) {
    m_eventSystem.ProcessEvent(event, *m_window);
}

void MenuScene::DisableMusic(SpriteButton *button) {
    m_audioPlayer.stop();
    sf::IntRect musicRect[] = {sf::IntRect(0, 16, 16, 16),
                               sf::IntRect(16, 16, 16, 16),
                               sf::IntRect(32, 16, 16, 16)};
    button->SetTextureRect(musicRect);
    Prefs::GetInstance()->SetBool("musicdisabled", true)->Save();
}

void MenuScene::EnableMusic(SpriteButton *button) {
    if (!Prefs::GetInstance()->GetBool("musicdisabled")) {
        // m_audioPlayer.play();
    }
    sf::IntRect musicRect[] = {sf::IntRect(0, 0, 16, 16),
                               sf::IntRect(16, 0, 16, 16),
                               sf::IntRect(32, 0, 16, 16)};
    button->SetTextureRect(musicRect);
    Prefs::GetInstance()->SetBool("musicdisabled", false)->Save();
}

void MenuScene::ToggleMusic(SpriteButton *button) {
    m_music = !m_music;
    if (m_music)
        EnableMusic(button);
    else
        DisableMusic(button);
}

void MenuScene::DisableSound(SpriteButton *button) {
    sf::IntRect musicRect[] = {sf::IntRect(0, 16, 16, 16),
                               sf::IntRect(16, 16, 16, 16),
                               sf::IntRect(32, 16, 16, 16)};
    button->SetTextureRect(musicRect);
    Prefs::GetInstance()->SetBool("sounddisabled", true)->Save();
}

void MenuScene::EnableSound(SpriteButton *button) {
    sf::IntRect musicRect[] = {sf::IntRect(0, 0, 16, 16),
                               sf::IntRect(16, 0, 16, 16),
                               sf::IntRect(32, 0, 16, 16)};
    button->SetTextureRect(musicRect);
    Prefs::GetInstance()->SetBool("sounddisabled", false)->Save();
}

void MenuScene::ToggleSound(SpriteButton *button) {
    m_sound = !m_sound;
    if (m_sound)
        EnableSound(button);
    else
        DisableSound(button);
}

void MenuScene::LoadGameScene() {
    StateManager::LoadState(std::make_unique<GameScene>(m_window));
}

}  // namespace chess
}  // namespace zifmann
