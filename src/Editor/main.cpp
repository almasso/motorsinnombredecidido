#include "common/Editor.h"

int main(int argc, char** argv) {
    if(editor::Editor::Init())
        editor::Editor::GetInstance().mainLoop();
    editor::Editor::Destroy();
    return 0;
}