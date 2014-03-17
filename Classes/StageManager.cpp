//
//  StageManager.cpp
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#include "StageManager.h"
#include "Utility.h"
#include "MainScene.h"

USING_NS_CC;

namespace {
    struct BGData {
        const char* file;
        Point uv;
        Point pos;
        int layer;
        float scrlRate;
        bool repeat;
        bool add;
    } bgDatas[] = {
        {   "SnowWalkerSky.png",          
            Point(0.5f, 1),    
            Point(0, 1136),      
            layer::bgBack,
            0.015f, true
        },    
        {   "SnowWalkerCloudBack.png",    
            Point(0.5f, 1),    
            Point(0, 1136),      
            layer::bgBack,
            0.1f, true  
        },
        {   "SnowWalkerCloudFront.png",   
            Point(0.5f, 1),    
            Point(0, 1136),      
            layer::bgBack,
            0.2f, true  
        },
        {   "SnowWalkerCity.png",         
            Point(0.5f, 0.5f), 
            Point(0, 820),       
            layer::bgBack,
            0.25f, true  
        },
        {   "SnowWalkerRiver.png",        
            Point(0.5f, 0.5f), 
            Point(0, 500),       
            layer::bgBack,
            0.5f, true 
        },
        {   "SnowWalkerBank.png",         
            Point(0.5f, 0.5f), 
            Point(0, 660),       
            layer::bgBack,
            0.5f, true 
        },
        {   "SnowWalkerField.png",        
            Point(0.5f, 0),    
            Point(0, -20),       
            layer::bgBack,
            1, true 
        },
        {   "SnowWalkerBuilding.png",     
            Point(0, 0),        
            Point(-320, 220),   
            layer::bgFront,
            1, false
        },
        {   "SnowWalkerDoorLight.png",    
            Point(0, 0.5f),      
            Point(-320, 360),  
            layer::bgFront,
            1, false, true
        }
    };
}

// on "init" you need to initialize your instance
StageManager::StageManager()
{
    for(int i = 0; i < bgNum; ++i)
        bgSprites[i] = NULL;

    Initialize();
}

void StageManager::Initialize() {

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    for(int i = 0; i < bgNum; ++i) {
        BGData& bgData = bgDatas[i];
        Point pos = bgData.pos;
        pos.x += visibleSize.width/2 + origin.x;

        if(bgSprites[i] != NULL) {
            bgSprites[i]->removeAllChildrenWithCleanup(true);
            bgSprites[i]->getParent()->removeChild(bgSprites[i], true);
            bgSprites[i] = NULL;
        }
        bgSprites[i] = Sprite::create(bgData.file);
        bgSprites[i]->setPosition(pos);
        bgSprites[i]->setAnchorPoint(bgData.uv);

        if(bgData.repeat) {
            auto sprite = Sprite::create(bgData.file);
            sprite->setAnchorPoint(Point(0, 0));
            sprite->setPosition(Point(640, 0));
            bgSprites[i]->addChild(sprite, 0);
        }
        if(bgData.add)
            bgSprites[i]->setBlendFunc(BlendFunc::ADDITIVE);
        MainScene::Get()->addChild(bgSprites[i], bgData.layer);
    }    
}

void StageManager::Update(float deltaTime) {
    for(int i = 0; i < bgNum; ++i) {
        BGData& bgData = bgDatas[i];
        Point pos = bgSprites[i]->getPosition();
        pos.x -= scrlSpd * bgData.scrlRate * deltaTime;
        if(bgData.repeat) {
            if(pos.x < -320)
                pos.x += 640;
        }
        pos.y = bgData.pos.y;
        bgSprites[i]->setPosition(pos);
    }
}
