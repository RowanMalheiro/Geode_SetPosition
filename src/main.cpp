#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/Layout.hpp>
#include <String>

#define FONT "bigFont.fnt"

using namespace geode::prelude;

CCPoint pos = {100, 2000};
double rot = 0;
bool active = false;
PlayerObject* cur_play = nullptr;

// specify parameters for the setup function in the Popup<...> template
class settingsPopup : public geode::Popup<std::string const&> {
   protected:


       bool setup(std::string const&) override {
         this->setZOrder(100);
         m_x=pos.x;m_y=pos.y; m_rot=rot;

         m_mainLayer->setTouchEnabled(true);
         // convenience function provided by Popup
         // for adding/setting a title to the popup
         this->setTitle("Position Settings");
         
         auto popupmenu = CCMenu::create();
         popupmenu->setContentSize(m_mainLayer->getContentSize());
         m_mainLayer->addChildAtPosition(popupmenu, Anchor::Center);

         auto menu = CCMenu::create();
         menu->setLayout(
            RowLayout::create()
            ->setGap(10.f)
            ->setAxisAlignment(AxisAlignment::Center)
         );
         menu->setContentSize(m_mainLayer->getContentSize());
         popupmenu->addChildAtPosition(menu, Anchor::Center);

         auto xbox = TextInput::create(80, std::to_string(m_x), FONT);
         auto ybox = TextInput::create(80, std::to_string(m_y), FONT);
         auto rbox = TextInput::create(80, std::to_string(m_rot), FONT);
         xbox->setCallback([&](const std::string& txt){
            pos.x=(int)std::strtol(txt.c_str(), nullptr, 10);
         });
         ybox->setCallback([&](const std::string& txt){
            pos.y=(int)std::strtol(txt.c_str(), nullptr, 10);
         });
         rbox->setCallback([&](const std::string& txt){
            rot=(float)std::strtol(txt.c_str(), nullptr, 10);

         });
         menu->addChild(xbox);
         menu->addChild(ybox);
         menu->addChild(rbox);

         auto checkBox = CCSprite::createWithSpriteFrameName(active ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
         auto set = CCMenuItemSpriteExtra::create(checkBox, nullptr, m_mainLayer, menu_selector(settingsPopup::onSet));
         set->setPosition({menu->getPositionX(),menu->getPositionY()-40.f});
         set->setID("sp-setbtn");
         popupmenu->addChild(set);
         menu->updateLayout();
         return true;
       }

       
   
   public:

      int m_x, m_y, m_rot;


       static settingsPopup* create(std::string const& text) {
           auto ret = new settingsPopup();
           if (ret->initAnchored(240.f, 160.f, text)) {
               ret->autorelease();
               return ret;
           }

           delete ret;

           return nullptr;
      }
       void onSet(CCObject* obj){
         auto cckbx = static_cast<CCMenuItemSpriteExtra*>(obj);
         active = !active;
         cckbx->setSprite(CCSprite::createWithSpriteFrameName(active ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png"));
       }
};

class $modify(playobj, PlayerObject) {
   
   //put the icon at pos
   void update(float t){
      if(!active){
         return update(t);
      }
      setPosition(pos);
      setRotation(rot);
      PlayerObject::update(0.0f);
   }
};

class $modify(PlayLayer){
   
   struct Fields{
      PlayerObject* m_play = nullptr;
   };

   bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects){
      bool b = PlayLayer::init(level, useReplay, dontCreateObjects);
      cur_play = nullptr;
      m_fields->m_play = nullptr;
      active = false;
      return b;
   }

   void destroyPlayer(PlayerObject* p, GameObject* g){ 

      if(cur_play == nullptr){
         cur_play = p;
      }
      if(m_fields->m_play == nullptr){
         m_fields->m_play=static_cast<PlayerObject*>(g);
         p->getPosition(&pos.x, &pos.y);
         destroyPlayer(p,g);
         return;
      }
      destroyPlayer(p,g);
   }

   void resetLevel(){
      PlayLayer::resetLevel();
      if(cur_play != nullptr){
         cur_play->getPosition(&pos.x, &pos.y);
      }
      return;
   }

   void onExit(){
      cur_play=nullptr;
      PlayLayer::onExit();
   }
};
//Actually have camera follow the player (for 2.2 levels)
class $modify(gamelay, GJBaseGameLayer){

   void update(float f){
      if(!active){
         return GJBaseGameLayer::update(f);
      }
      updateCamera(0.001f);
      GJBaseGameLayer::update(f);
   }
};

class $modify(SPPauseLay, PauseLayer){
   void customSetup() {
      PauseLayer::customSetup();
      auto menu = this->getChildByID("left-button-menu");

      auto sprite = CCSprite::createWithSpriteFrameName("edit_flipXBtn_001.png");
      if(sprite==NULL) return;
   
      auto button = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(SPPauseLay::buttonPressed));
      button->setID("ps-pausebtn");
      menu->addChild(button);
      menu->updateLayout();
   }

   void buttonPressed(cocos2d::CCObject*){
      auto pu = settingsPopup::create("ex");
      this->addChild(pu);
   }
};
