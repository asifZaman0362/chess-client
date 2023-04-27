#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace zifmann {
namespace chess {
class AssetManager {
   private:
    static std::unordered_map<const char *, sf::Texture> m_textures;
    static std::unordered_map<const char *, sf::Image> m_images;
    static std::unordered_map<const char *, sf::SoundBuffer> m_audioClips;
    static std::unordered_map<const char *, sf::Font> m_fonts;
    static std::unordered_map<const char *, std::vector<std::string>> m_text;

   public:
    AssetManager() = default;
    ~AssetManager() = default;
    static sf::Texture *GetTexture(const char *filename);
    static sf::SoundBuffer *GetAudioClip(const char *filename);
    static sf::Font *GetFont(const char *filename);
    static sf::Image *GetImage(const char *filename);
    static std::vector<std::string> *GetText(const char *filename);
};
}  // namespace chess
}  // namespace zifmann

#endif
