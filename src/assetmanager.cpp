#include "assetmanager.hpp"
#include "logger.hpp"
#include <filesystem>

namespace zifmann {
    namespace chess {

        std::unordered_map<const char*, sf::Texture> AssetManager::m_textures;
        std::unordered_map<const char*, sf::SoundBuffer> AssetManager::m_audioClips;
        std::unordered_map<const char*, sf::Font> AssetManager::m_fonts;

        using std::filesystem::path;

        sf::Texture* AssetManager::GetTexture(const char* filename) {
            if (m_textures.find(filename) == m_textures.end()) {
                sf::Texture texture;
                auto base = path("Assets/Sprites");
                auto file = path(filename);
                if (texture.loadFromFile(base / file)) {
                    m_textures.insert({filename, texture});
                    return &m_textures[filename];
                } else {
                    log_error("Failed to load texture! File not found: ", filename);
                    return nullptr;
                }
            } else {
                return &m_textures[filename];
            }
        }

        sf::SoundBuffer* AssetManager::GetAudioClip(const char* filename) {
            if (m_audioClips.find(filename) == m_audioClips.end()) {
                sf::SoundBuffer clip;
                auto base = path("Assets/Audio");
                auto file = path(filename);
                if (clip.loadFromFile(base / file)) {
                    m_audioClips.insert({filename, clip});
                    return &m_audioClips[filename];
                } else {
                    log_error("Failed to load audio clip! File not found: ", filename);
                    return nullptr;
                }
            } else {
                return &m_audioClips[filename];
            }
        }

        sf::Font* AssetManager::GetFont(const char* filename) {
            if (m_fonts.find(filename) == m_fonts.end()) {
                sf::Font font;
                auto base = path("Assets/Fonts");
                auto file = path(filename);
                if (font.loadFromFile(base / file)) {
                    m_fonts.insert({filename, font});
                    return &m_fonts[filename];
                } else {
                    log_error("Failed to load audio clip! File not found: ", filename);
                    return nullptr;
                }
            } else {
                return &m_fonts[filename];
            }
        }

    }
}
