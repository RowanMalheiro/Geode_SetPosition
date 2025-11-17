#include "includes.hpp"
#include "Overlay.hpp"


$execute{
   ModData::pos=Vec3{
      .x = 0,
      .y = 105,.z = 0
   };
   ModData::isActive= false;
   ModData::curPlay = nullptr;
   Overlay::init();
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
      Overlay::get()->toggle();
   }
};
