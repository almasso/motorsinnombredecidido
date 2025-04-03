#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <string>
#include <vector>
#include <Core/ComponentTemplate.h>

class Text;

class ComponentClass(TextBox) {
    private:
        Text* _text {};
        std::vector<std::string> _dialog;
        float _characterDelay {};
        float _timer {};
        size_t _wordIter {}, _paragraphIter {};
        void splitText(const std::string& fullText);
    public:
        TextBox(ComponentData const* data);
        bool init() override;
        bool update() override;
        bool ended();
        bool setText(const std::string& fullText);
};

#endif //TEXTBOX_H
