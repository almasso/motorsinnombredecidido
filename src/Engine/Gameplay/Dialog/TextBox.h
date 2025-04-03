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
        size_t _charIter {}, _paragraphIter {};
        void splitText(const std::string& fullText);
    public:
        TextBox(ComponentData const* data);
        bool init() override;
        bool update() override;
        bool ended() const;

        void setText(const std::string &fullText);

        static void RegisterToLua(sol::state& lua);
};

#endif //TEXTBOX_H
