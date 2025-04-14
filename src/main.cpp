#include "includes.hpp"


$execute{
   
   ModData::pos=Vec3{
      .x = 0,
      .y = 105,
      .z = 0
   };
   ModData::isActive= false;
   ModData::curPlay = nullptr;
};

// specify parameters for the setup function in the Popup<...> template
class settingsPopup : public geode::Popup<std::string const&> {
   protected:


       bool setup(std::string const&) override {
         this->setZOrder(100);

         m_x=ModData::pos.x;
         m_y=ModData::pos.y;
         m_rot=ModData::pos.z;
         
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

         auto xbox = TextInput::create(80, std::to_string(m_x).substr(0,8), FONT);
         auto ybox = TextInput::create(80, std::to_string(m_y).substr(0,8), FONT);
         auto rbox = TextInput::create(80, std::to_string(m_rot).substr(0,8), FONT);
         xbox->setFilter(VALIDCHARS);
         ybox->setFilter(VALIDCHARS);
         rbox->setFilter(VALIDCHARS);
         
         xbox->setCallback([&](const std::string& txt){
            m_x=(float)std::strtod(txt.c_str(), nullptr);
            ModData::pos.x = m_x;
         });
         ybox->setCallback([&](const std::string& txt){
            m_y=(float)std::strtod(txt.c_str(), nullptr);
            ModData::pos.y=m_y;
         });
         rbox->setCallback([&](const std::string& txt){
            m_rot=(float)std::strtod(txt.c_str(), nullptr);
            ModData::pos.z=m_rot;
         });
         menu->addChild(xbox);
         menu->addChild(ybox);
         menu->addChild(rbox);

         auto checkBox = CCSprite::createWithSpriteFrameName(ModData::isActive ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
         auto set = CCMenuItemSpriteExtra::create(checkBox, nullptr, m_mainLayer, menu_selector(settingsPopup::onSet));
         set->setPosition({menu->getPositionX(),menu->getPositionY()-40.f});
         set->setID("sp-setbtn");
         popupmenu->addChild(set);
         menu->updateLayout();
         return true;
       }

       void updateSavedPos(){
         ModData::pos.x=m_x;
         ModData::pos.y=m_y;
         ModData::pos.z=m_rot;
       }
       

       
   
   public:

      float m_x, m_y, m_rot;


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
         ModData::isActive = !ModData::isActive;
         cckbx->setSprite(CCSprite::createWithSpriteFrameName(ModData::isActive ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png"));
       }
};

class $modify(playobj, PlayerObject) {
   
   //put the icon at pos
   void update(float t){
      if(!ModData::isActive){
         return update(t);
      }
      setPos();
      PlayerObject::update(0.0f);
   }

   void setPos(){
      auto pos = ModData::pos;
      CCPoint posxy {pos.x,pos.y};
      setPosition(posxy);
      setRotation(pos.z);
   }
};

class $modify(PlayLayer){
   
   struct Fields{
      PlayerObject* m_play = nullptr;
   };

   bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects){
      bool b = PlayLayer::init(level, useReplay, dontCreateObjects);
      ModData::curPlay = nullptr;
      m_fields->m_play = nullptr;
      ModData::isActive = false;
      return b;
   }

   void destroyPlayer(PlayerObject* p, GameObject* g){ 
      Vec3 tempPos = ModData::pos;

      if(ModData::curPlay == nullptr){
         ModData::curPlay = p;
      }
      if(m_fields->m_play == nullptr){
         m_fields->m_play=static_cast<PlayerObject*>(g);
         p->getPosition(&tempPos.x, &tempPos.y);
         ModData::pos = tempPos;
         destroyPlayer(p,g);
         return;
      }
      destroyPlayer(p,g);
   }

   void resetLevel(){
      Vec3 tpos = ModData::pos;
      PlayLayer::resetLevel();
      if(ModData::curPlay != nullptr){
         ((PlayerObject*)ModData::curPlay)->getPosition(&tpos.x, &tpos.y);
         ModData::pos=tpos;
      }
      return;
   }

   void onExit(){
      ModData::curPlay = nullptr;
      PlayLayer::onExit();
   }
};
//Actually have camera follow the player (for 2.2 levels)
class $modify(gamelay, GJBaseGameLayer){

   void update(float f){
      if(!ModData::isActive){
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
