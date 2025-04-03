#include "TextBox.h"
#include <Core/ComponentData.h>
#include <Input/InputManager.h>
#include <Load/ResourceHandler.h>
#include <Render/Font.h>
#include <Render/Text.h>
#include <Utils/RPGError.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Utils/Time.h>
#include <Utils/TimeManager.h>
#include <sol/state.hpp>

void TextBox::splitText(const std::string &fullText) {
    _dialog.clear();
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
        RPGError::ShowError("","");
        return false;
    }
    _text->setText("");
    _characterDelay = _data->getData<float>("wordDelay", 0.015f);

    return true;
}

bool TextBox::update() {
    _timer += Time::deltaTime;
    if (_paragraphIter < _dialog.size()) {
        if (_wordIter < _dialog[_paragraphIter].size()) {
            if (_timer >= _characterDelay) {
                _timer = 0.0f;
                _text->setText(_text->getText() + _dialog[_paragraphIter][_wordIter]);
                _wordIter++;
            }
        } else {
            std::cout << "Waiting for input... \n";
            if (InputManager::GetState().mouse_down) {
                _timer = 0.0f;
                _wordIter = 0;
                _paragraphIter++;
                _text->setText("");
                if (ended())
                    _entity->setActive(false);
            }
        }
    }
    return true;
}

bool TextBox::ended() const {
    return _paragraphIter == _dialog.size();
}

bool TextBox::setText(const std::string &fullText) {
    if (!ended()) return false;
    splitText(fullText);
    _wordIter = _paragraphIter = 0;
    return true;
}

void TextBox::RegisterToLua(sol::state& lua) {
    sol::usertype<TextBox> type = lua.new_usertype<TextBox>("TextBox");
    type["setText"] = &TextBox::setText;
    type["ended"] = &TextBox::ended;
    type["get"] = TextBox::get;
}
