#include "assetmanager.hpp"

#include <filesystem>
#include <fstream>

#include "logger.hpp"

namespace zifmann {
namespace chess {

std::unordered_map<const char *, sf::Texture> AssetManager::m_textures;
std::unordered_map<const char *, sf::SoundBuffer> AssetManager::m_audioClips;
std::unordered_map<const char *, sf::Font> AssetManager::m_fonts;
std::unordered_map<const char *, sf::Image> AssetManager::m_images;
std::unordered_map<const char *, std::vector<std::string>> AssetManager::m_text;

using std::filesystem::path;

sf::Texture *AssetManager::GetTexture(const char *filename) {
    if (m_textures.find(filename) == m_textures.end()) {
        sf::Texture texture;
        auto base = path("Assets/Sprites");
        auto file = path(filename);
        if (texture.loadFromFile(base / file)) {
            m_textures.insert({filename, texture});
            return &m_textures[filename];
        } else {
            log_error("Failed to load texture! File not found: %s", filename);
            return nullptr;
        }
    } else {
        return &m_textures[filename];
    }
}

sf::SoundBuffer *AssetManager::GetAudioClip(const char *filename) {
    if (m_audioClips.find(filename) == m_audioClips.end()) {
        sf::SoundBuffer clip;
        auto base = path("Assets/Audio");
        auto file = path(filename);
        if (clip.loadFromFile(base / file)) {
            m_audioClips.insert({filename, clip});
            return &m_audioClips[filename];
        } else {
            log_error("Failed to load audio clip! File not found: %s",
                      filename);
            return nullptr;
        }
    } else {
        return &m_audioClips[filename];
    }
}

sf::Font *AssetManager::GetFont(const char *filename) {
    if (m_fonts.find(filename) == m_fonts.end()) {
        sf::Font font;
        auto base = path("Assets/Fonts");
        auto file = path(filename);
        if (font.loadFromFile(base / file)) {
            m_fonts.insert({filename, font});
            return &m_fonts[filename];
        } else {
            log_error("Failed to load audio clip! File not found: %s",
                      filename);
            return nullptr;
        }
    } else {
        return &m_fonts[filename];
    }
}

sf::Image *AssetManager::GetImage(const char *filename) {
    if (m_images.find(filename) == m_images.end()) {
        sf::Image image;
        auto base = path("Assets/Sprites");
        auto file = path(filename);
        if (image.loadFromFile(base / file)) {
            m_images.insert({filename, image});
            return &m_images[filename];
        } else {
            log_error("Failed to load image! File not found: %s", filename);
            return nullptr;
        }
    } else {
        return &m_images[filename];
    }
}

std::vector<std::string> *AssetManager::GetText(const char *filename) {
    if (m_text.find(filename) == m_text.end()) {
        std::vector<std::string> lines;
        auto base = path("Assets/Text");
        auto file = path(filename);
        std::ifstream f(base / file);
        if (f.is_open()) {
            while (!f.eof()) {
                std::string string;
                f >> string;
                lines.push_back(string);
            }
            m_text.insert({filename, lines});
            return &m_text[filename];
        } else {
            log_error("Failed to load text! File not found: %s", filename);
            return nullptr;
        }
    } else
        return &m_text[filename];
}

}  // namespace chess
}  // namespace zifmann
