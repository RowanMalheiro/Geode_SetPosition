#pragma once

#include <Geode/Geode.hpp>
#include <Geode/cocos/include/ccTypes.h>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/cocos/robtop/xml/pugixml.hpp>
#include <string>

#define FONT "bigFont.fnt"
#define VALIDCHARS "0123456789."

using namespace geode::prelude;

struct Vec3{
    float x;
    float y;
    float z;
};

class ModData{
    public:
    static inline Vec3 pos = {0.f,0.f,0.f};
    static inline bool isActive = false;
    static inline void* curPlay = nullptr;

};
