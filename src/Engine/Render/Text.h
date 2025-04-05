#ifndef TEXT_H
#define TEXT_H
#include "TextTexture.h"
#include "RenderComponent.h"
#include <Utils/Vector2.h>

class ComponentDerived(Text, RenderComponent) {
    TextTexture* _texture;
    std::string _text;
    std::string _font;
    int _fontSize;
    Color _color;
    Vector2 _size;
    bool _centerText;
    bool remakeTexture();
    public:
    Text(ComponentData const*data);
    bool init() override;
    bool render(RenderManager* manager) override;
    bool setText(const std::string& text);
    bool setFont(const std::string& font);
    bool setFontSize(int fontSize);
    bool setColor(const Color& color);
    bool setSize(const Vector2& size);
    bool setCenterText(bool center);
    const std::string& getText() const;
    std::string getFont() const;
    const Vector2& getSize() const;

    static void RegisterToLua(sol::state &lua);
};
#endif //TEXT_H
