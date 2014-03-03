//
//  StageLayer.cpp
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#include "StageLayer.h"
#include "Utility.h"

USING_NS_CC;

const char* bgFiles[] = {
    "SnowWalkerSky.png",    // bgSky
    "SnowWalkerCloudBack.png",    // bgCloudBack
    "SnowWalkerCloudFront.png",    // bgCloudFront
    "SnowWalkerCity.png",    // bgCity
    "SnowWalkerRiver.png",    // bgRiver
    "SnowWalkerBank.png",    // bgBank
    "SnowWalkerField.png",    // bgField
};

const Point bgUVs[] = {
    Point(0.5f, 1),    // bgSky
    Point(0.5f, 1),    // bgCloudBack
    Point(0.5f, 1),    // bgCloudFront
    Point(0.5f, 0.5f),    // bgCity
    Point(0.5f, 0.5f),    // bgRiver
    Point(0.5f, 0.5f),    // bgBank
    Point(0.5f, 0),    // bgField
};

const Point bgPositions[] = {
    Point(0, 1136),    // bgSky
    Point(0, 1136),    // bgCloudBack
    Point(0, 1136),    // bgCloudFront
    Point(0, 820),    // bgCity
    Point(0, 500),    // bgRiver
    Point(0, 660),    // bgBank
    Point(0, -20),    // bgField
};

const float bgScrlSpds[] = {
    4,    // bgSky
    32,    // bgCloudBack
    48,    // bgCloudFront
    64,    // bgCity
    160,    // bgRiver
    120,    // bgBank
    240,    // bgField
};

// on "init" you need to initialize your instance
bool StageLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    for(int i = 0; i < bgNum; ++i) {
        Point pos = bgPositions[i];
        pos.x += visibleSize.width/2 + origin.x;
        bgSprites[i] = Sprite::create(bgFiles[i]);
        bgSprites[i]->setPosition(pos);
        bgSprites[i]->setAnchorPoint(bgUVs[i]);
        auto sprite = Sprite::create(bgFiles[i]);
        sprite->setAnchorPoint(Point(0, 0));
        sprite->setPosition(Point(640, 0));
        bgSprites[i]->addChild(sprite, 0);
        this->addChild(bgSprites[i], 0);
    }

    return true;
}

void StageLayer::Update(float deltaTime) {
    for(int i = 0; i < bgNum; ++i) {
        Point pos = bgSprites[i]->getPosition();
        pos.x -= bgScrlSpds[i] * scrlRate * deltaTime;
        if(pos.x < -320)
            pos.x += 640;
        pos.y = bgPositions[i].y;
        bgSprites[i]->setPosition(pos);
    }
}

void StageLayer::onEnter() {
	Layer::onEnter();
}

void StageLayer::onExit() {

}
