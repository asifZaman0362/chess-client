#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace zifmann {
namespace chess {

enum Anchor {
    CentreH = 0,
    Left = 1,
    Right = 2,
    CentreV = 4,
    Top = 8,
    Bottom = 16
};

enum ScaleMode { Scale, Constant };

class CanvasItem {
   protected:
    int m_anchor;
    ScaleMode m_scaleMode;
    sf::Vector2f m_scale;
    sf::IntRect m_offset;
    const sf::IntRect *m_parent;

   public:
    CanvasItem(sf::IntRect rect, int anchor, ScaleMode scaleMode);
    virtual ~CanvasItem() = default;
    void SetOffset(sf::IntRect offset);
    virtual void Calculate(const sf::IntRect &parent);
    virtual void SetSize(sf::Vector2i size) = 0;
    virtual void SetPosition(sf::Vector2i position) = 0;
    virtual sf::Vector2f GetPixelSize() const = 0;
    virtual void draw(sf::RenderTarget &target) const = 0;
};

class CanvasSprite : public CanvasItem {
   protected:
    sf::Sprite m_sprite;

   public:
    CanvasSprite(sf::Texture *texture, sf::IntRect textureRect,
                 sf::IntRect offset, int anchor, ScaleMode scaleMode);
    ~CanvasSprite() = default;
    void SetSize(sf::Vector2i size) override;
    void SetPosition(sf::Vector2i position) override;
    virtual sf::Vector2f GetPixelSize() const override;
    virtual void draw(sf::RenderTarget &target) const override;
};

class CanvasText : public CanvasItem {
   protected:
    sf::Text m_text;
    sf::Font m_font;
    sf::String m_string;

   public:
    CanvasText(const sf::String &string, sf::Font *font, sf::IntRect offset,
               int anchor, ScaleMode scaleMode);
    ~CanvasText() = default;
    sf::Text &GetText();
    void SetSize(sf::Vector2i size) override;
    void SetPosition(sf::Vector2i position) override;
    virtual void Calculate(const sf::IntRect &parent) override;
    virtual sf::Vector2f GetPixelSize() const override;
    void draw(sf::RenderTarget &target) const override;
    void SetCharacterSize(const unsigned int);
    void SetString(const sf::String &string);
    sf::FloatRect GetGlobalBounds() const;
};

class Canvas {
   protected:
    std::vector<CanvasItem *> m_items;

   public:
    Canvas() = default;
    Canvas(Canvas &);
    Canvas(Canvas &&);
    ~Canvas();
    void AddChild(CanvasItem *item);
    void draw(sf::RenderTarget &target);
    void UpdateSize(sf::Vector2u size);
};

}  // namespace chess
}  // namespace zifmann

#endif
