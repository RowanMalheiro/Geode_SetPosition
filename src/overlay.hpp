#pragma once

#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>
#include <string>
#include <cstring>
#include "includes.hpp"

class Overlay{
    public:
    static Overlay* get();
    static void init();
    Overlay();
    void toggle();
    void draw();
    bool& getEnabled();
    void setPos(float x, float y, float rot);
};