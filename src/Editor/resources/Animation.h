//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_ANIMATION_H
#define RPGBAKER_ANIMATION_H

#include "EditorResource.h"
#include <string>
#include <filesystem>
#include <vector>
#include <sol/sol.hpp>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Sprite;

    class Animation : public EditorResource{
    public:
        explicit Animation(Project* project);
        ~Animation();

        void init(const std::string& name, const std::vector<Sprite*>& frames, float timeBetweenFrames, bool loop);

        bool readFromLua(const std::string& name) final;

        void writeToLua() final;

        void writeToEngineLua(const std::string &platform) final;

        const std::string& getName() const;
        const std::vector<Sprite*>& getFrames() const;
        float getTimeBetweenFrames() const;
        float getTotalDuration() const;
        Sprite* getFrameAtTime(float time) const;
        bool isLoop() const;

        bool isInitialized() const;

        static void setAnimationsDirectory(const std::filesystem::path& animationsDirectory);

    private:
        static std::filesystem::path _animationsDirectory;
        bool _init = false;
        std::string _name;
        std::vector<Sprite*> _frames;
        float _timeBetweenFrames;
        bool _loop;
        Project* _project;

        static std::string getFilePath(const std::string& animationName);

        void init(const std::string& name, const sol::table& framesTable, float timeBetweenFrames, bool loop);
    };
}



#endif //ANIMATION_H
