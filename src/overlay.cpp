#include "overlay.hpp"

#define SLIDER_RANGE 0.5f

static Overlay* s_overlay = nullptr;

Overlay* Overlay::get(){
    return s_overlay;
}

void Overlay::init(){
    s_overlay = new Overlay();
}

Overlay::Overlay(){
    ImGuiCocos::get().setup([] {
        ImGuiCocos::get().setVisible(false);
    }).draw([] {
        Overlay::get()->draw();
    });

}

void Overlay::toggle(){
    ImGuiCocos::get().toggle();
}

void Overlay::draw(){
    static char xbuf[32] = "";
    static char ybuf[32] = "";
    static char rbuf[32] = "";
    static float sliderspeed = 1.0f;

    float tempx = ModData::pos.x;
    float tempy = ModData::pos.y;
    float tempz = ModData::pos.z;

    strcpy(xbuf, std::to_string(ModData::pos.x).c_str());
    strcpy(ybuf, std::to_string(ModData::pos.y).c_str());
    strcpy(rbuf, std::to_string(ModData::pos.z).c_str());

    if(ModData::isActive){
        PlatformToolbox::showCursor();
        PlatformToolbox::toggleLockCursor(false);
    }

    ImGui::Begin("SetPosition");
    ImGui::InputText("x pos", xbuf, IM_ARRAYSIZE(xbuf), ImGuiInputTextFlags_CharsDecimal);
    ImGui::SliderFloat("x slider", &tempx, ModData::pos.x-sliderspeed, ModData::pos.x+sliderspeed);
    ImGui::InputText("y pos", ybuf, IM_ARRAYSIZE(ybuf), ImGuiInputTextFlags_CharsDecimal);
    ImGui::SliderFloat("y slider", &tempy, ModData::pos.y-sliderspeed, ModData::pos.y+sliderspeed);
    ImGui::InputText("rot", rbuf, IM_ARRAYSIZE(rbuf), ImGuiInputTextFlags_CharsDecimal);
    ImGui::SliderFloat("rot slider", &tempz, ModData::pos.z-sliderspeed, ModData::pos.z+sliderspeed);
    ImGui::SliderFloat("Slider Speed", &sliderspeed, 0.0f, 10.f);

    ImGui::Selectable("Enable Mod", &ModData::isActive);

    ImGui::End();

    //wtf is this code
    //worst code ever?
    if(tempx != ModData::pos.x){
        ModData::pos.x = tempx;
        return;
    }
    if(tempy != ModData::pos.y){
        ModData::pos.y = tempy;
        return;
    }
    if(tempz != ModData::pos.z){
        ModData::pos.z = tempz;
        return;
    }

    ModData::pos.x = std::stof(xbuf);
    ModData::pos.y = std::stof(ybuf);
    ModData::pos.z = std::stof(rbuf);
}


