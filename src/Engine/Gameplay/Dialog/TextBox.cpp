#include "TextBox.h"
#include <Core/ComponentData.h>
#include <Input/InputManager.h>
#include <Load/ResourceHandler.h>
#include <Render/Font.h>
#include <Render/Text.h>
#include <Utils/Error.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Utils/Time.h>
#include <Utils/TimeManager.h>
#include <sol/state.hpp>

void TextBox::splitText(const std::string &fullText) {
    const Font* font = ResourceHandler<Font>::Instance()->get(_text->getFont());
    int maxWidth = _text->getSize().getX() * 0.97f;
    int maxHeight = _text->getSize().getY();
    int lineHeight = TTF_GetFontHeight(font->getFont());
    int maxLines = maxHeight / lineHeight;

    std::istringstream stream(fullText);
    std::string line;
    std::string paragraph;
    std::string word;
    std::string lineCheck;
    int textWidth;
    while (std::getline(stream, line, '\n')) {
        std::istringstream lineStream(line);
        int nLines = 1;
        while (lineStream >> word) {
            lineCheck += word + " ";
            TTF_GetStringSize(font->getFont(), lineCheck.c_str(), lineCheck.size(), &textWidth, nullptr);
            if (textWidth >= maxWidth) {
                paragraph.back() = '\n';
                lineCheck = word + " ";
                nLines++;
                if (nLines > maxLines) {
                    _dialog.push_back(paragraph);
                    paragraph.clear();
                    nLines = 1;
                }
            }
            paragraph += word + " ";
        }
        if (!paragraph.empty()) {
            _dialog.push_back(paragraph);
            paragraph.clear();
            lineCheck.clear();
        }
    }
}

TextBox::TextBox(ComponentData const *data) : ComponentTemplate(data) {
}

bool TextBox::init() {
    _text = _entity->getComponent<Text>();
    if (!_text) {
        Error::ShowError("","");
        return false;
    }
    _text->setText("");
    _characterDelay = _data->getData<float>("wordDelay", 0.015f);

    return true;
}

bool TextBox::update() {
    _timer += Time::deltaTime;
    if (_paragraphIter < _dialog.size()) {
        if (_charIter < _dialog[_paragraphIter].size()) {
            float delay = _characterDelay * (InputManager::GetState().mouse_pressed ? .1f : 1);
            if (_timer >= delay) {
                _timer = 0.0f;
                std::string current(1,_dialog[_paragraphIter][_charIter]);
                if (static_cast<unsigned char>(current[0]) > 0xC2 && _charIter+1 < _dialog[_paragraphIter].size()) {
                    _charIter++;
                    current += _dialog[_paragraphIter][_charIter];
                }
                _text->setText(_text->getText() + current);
                _charIter++;
            }
        } else {
            if (InputManager::GetState().mouse_up) {
                _timer = 0.0f;
                _charIter = 0;
                _paragraphIter++;
                _text->setText("");
                if (ended()) {
                    _entity->setActive(false);
                    _charIter = _paragraphIter = 0;
                    _dialog.clear();
                }
            }
        }
    }
    return true;
}

bool TextBox::ended() const {
    return _paragraphIter == _dialog.size();
}

void TextBox::setText(const std::string &fullText) {
    _entity->setActive(true);
    splitText(fullText);
}

void TextBox::RegisterToLua(sol::state& lua) {
    sol::usertype<TextBox> type = lua.new_usertype<TextBox>("TextBox");
    type["setText"] = &TextBox::setText;
    type["ended"] = &TextBox::ended;
    type["get"] = TextBox::get;
}
