#include "overlay.hpp"

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

    strcpy(xbuf, std::to_string(ModData::pos.x).c_str());
    strcpy(ybuf, std::to_string(ModData::pos.y).c_str());
    strcpy(rbuf, std::to_string(ModData::pos.z).c_str());

    if(ModData::isActive){
        PlatformToolbox::showCursor();
        PlatformToolbox::toggleLockCursor(false);
    }

    ImGui::Begin("SetPosition");
    ImGui::InputText("x pos", xbuf, IM_ARRAYSIZE(xbuf), ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputText("y pos", ybuf, IM_ARRAYSIZE(ybuf), ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputText("rot", rbuf, IM_ARRAYSIZE(rbuf), ImGuiInputTextFlags_CharsDecimal);

    ImGui::Selectable("Enable Mod", &ModData::isActive);

    ImGui::End();

    ModData::pos.x = std::stof(xbuf);
    ModData::pos.y = std::stof(ybuf);
    ModData::pos.z = std::stof(rbuf);
}


