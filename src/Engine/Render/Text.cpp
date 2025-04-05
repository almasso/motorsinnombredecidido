#include "Text.h"
#include "Font.h"
#include "Transform.h"
#include "RenderManager.h"
#include <Load/ResourceHandler.h>
#include <Core/ComponentData.h>
#include <Utils/Rect.h>
#include <Utils/Error.h>


bool Text::remakeTexture() {
    delete _texture;
    const Font* font = ResourceHandler<Font>::Instance()->get(std::to_string(_fontSize)+_font);
    if (!font) {
        _texture = nullptr;
        return false;
    }
    _texture = new TextTexture(font, _color, _text, _size.getX(), _size.getY(), _centerText);
    return _texture->getTexture() != nullptr;
}

Text::Text(ComponentData const *data) :
    ComponentTemplate(data),
    _texture(nullptr),
    _fontSize(12),
    _size(1, 1),
    _centerText(false) {
}

bool Text::init() {
    _text = _data->getData<std::string>("text");
    _font = _data->getData<std::string>("font");
    _fontSize = _data->getData<int>("fontSize",12);
    _color = _data->getData<int>("color",0xFFFFFFFF);
    _centerText = _data->getData<bool>("centerText",false);
    _size = _data->getVector("size",{100,100});
    if (!remakeTexture()) return false;
    return RenderComponent::init();
}

bool Text::render(RenderManager *manager) {
    Vector2 size = _transform->getGlobalScale() * _size;
    Vector2 position = _transform->getGlobalPosition() - size/2.f;
    Rect rectangle = {position.getX(), position.getY(), size.getX(), size.getY()};
    return manager->drawText(rectangle, _texture, _transform->getGlobalRotation());
}

bool Text::setText(const std::string &text) {
    if (_text == text) return true;
    _text = text;
    return remakeTexture();
}

bool Text::setFont(const std::string &font) {
    if (_font == font) return true;
    _font = font;
    return remakeTexture();
}

bool Text::setFontSize(int fontSize) {
    if (_fontSize == fontSize) return true;
    _fontSize = fontSize;
    return remakeTexture();
}

bool Text::setColor(const Color &color) {
    if (_color == color) return true;
    _color = color;
    return remakeTexture();
}

bool Text::setSize(const Vector2 &size) {
    if (_size == size) return true;
    _size = size;
    return remakeTexture();
}

bool Text::setCenterText(bool center) {
    if (_centerText == center) return true;
    _centerText = center;
    return remakeTexture();
}

const std::string& Text::getText() const {
    return _text;
}

std::string Text::getFont() const {
    return std::to_string(_fontSize)+_font;
}

const Vector2 & Text::getSize() const {
    return _size;
}

void Text::RegisterToLua(sol::state &lua) {
    sol::usertype<Text> type = lua.new_usertype<Text>("Text");
    type["setText"] = &Text::setText;
    type["get"] = &Text::get;
}
