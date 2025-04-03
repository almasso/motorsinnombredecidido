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
    setText(
    "¡Hola! Bienvenido al mundo de los Pokémon. Mi nombre es Oak. Pero las personas me llaman Profesor Oak. Este es un mundo vasto y fascinante, lleno de criaturas llamadas Pokémon. Los Pokémon son seres extraordinarios que conviven con los seres humanos, y juntos comparten un sinfín de aventuras y desafíos. En este mundo, los Pokémon pueden ser compañeros leales, amigos y aliados, pero también pueden ser poderosos rivales en emocionantes batallas. Es un mundo donde cada Pokémon tiene su propia historia, habilidades únicas y, lo más importante, una relación especial con las personas que los cuidan.\n"
        "Los Pokémon no son solo criaturas para luchar. Son compañeros de vida que ayudan en tareas diarias, en la exploración y, en algunos casos, incluso en la protección de las personas. Sin embargo, no todos los Pokémon llevan una vida tranquila. Algunos de ellos están destinados a enfrentarse en épicas batallas para demostrar quién es el más fuerte. Es en estas batallas donde los entrenadores como tú entran en acción. El destino de los Pokémon y los entrenadores está entrelazado, y juntos, pueden alcanzar nuevas alturas.\n"
        "Este es el comienzo de tu propia aventura en el mundo Pokémon. Como futuro entrenador, tu misión será recorrer el mundo, capturar Pokémon, entrenarlos y, lo más importante, descubrir qué significa ser un verdadero entrenador. Pero antes de que comiences tu viaje, hay algo muy importante que debes hacer: elegir a tu primer compañero Pokémon. Elige sabiamente, porque este será tu compañero más cercano, y juntos, enfrentarán muchos desafíos. No solo te acompañará en tus batallas, sino que también será tu amigo, tu apoyo y tu guía mientras exploras este vasto mundo.\n"
        "Cada elección es importante, y no solo por la fuerza que tu Pokémon pueda tener. Los Pokémon tienen características que se ajustan a diferentes estilos de entrenamiento, y tú tendrás que aprender a conocerlos, a cuidarlos y a fortalecerlos. Así que, sin más preámbulo, elige a tu primer compañero Pokémon y prepárate para comenzar una aventura inolvidable. ¡El mundo Pokémon te espera!");

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
            }
        }
    }
    return true;
}

bool TextBox::ended() {
    return _paragraphIter == _dialog.size();
}

bool TextBox::setText(const std::string &fullText) {
    if (!ended()) return false;
    splitText(fullText);
    _wordIter = _paragraphIter = 0;
    return true;
}
