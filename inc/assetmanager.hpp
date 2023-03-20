#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

namespace zifmann {
    namespace chess {
        class AssetManager {
            private:
                static std::unordered_map<const char*, sf::Texture> m_textures;
                static std::unordered_map<const char*, sf::SoundBuffer> m_audioClips;
                static std::unordered_map<const char*, sf::Font> m_fonts;
            public:
                AssetManager() = default;
                ~AssetManager() = default;
                static sf::Texture* GetTexture(const char* filename);
                static sf::SoundBuffer* GetAudioClip(const char* filename);
                static sf::Font* GetFont(const char* filename);
        };
    }
}
