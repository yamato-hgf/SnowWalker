//
//  UILayer.cpp
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#include "UILayer.h"
#include "Utility.h"

#include "Define.h"

USING_NS_CC;

const char* uiFiles[] = {
    "SnowWalkerLifeFrame.png", // bgUILifeFrame
    "SnowWalkerLifeGauge.png", // bgUILifeGauge
    "SnowWalkerLifeWarning.png", // bgUILifeWarning
    "SnowWalkerProgress.png", // bgUIProgress
    "SnowWalkerUIUnder.png"    // bgUIUnder    
};

struct ButtonData {
    Point pos;
    float scale;
    button::Type type;
} buttonDatas[] = {
    { Point(64, 60), 64, button::Slow },
    { Point(135, 156), 64, button::Jump },
    { Point(196, 60), 64, button::Dash },
    { Point(400, 100), 96, button::Drink },
};

const Point uiUVs[] = {
    Point(0.5f, 0.5f), // bgUILifeFrame
    Point(0.0f, 0.5f), // bgUILifeGauge
    Point(0.5f, 0.5f), // bgUILifeWarning
    Point(0.5f, 0.5f), // bgUIProgress
    Point(0.5f, 0)    // bgUIUnder
};

const Point uiPositions[] = {
    Point(0, 924), // bgUILifeFrame
    Point(-309, 924), // bgUILifeGauge
    Point(128, 934), // bgUILifeWarning
    Point(0, 860), // bgUIProgress
    Point(0, -100)    // bgUIUnder    
};


// on "init" you need to initialize your instance
bool UILayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    scoreLabel = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    scoreLabel->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - scoreLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(scoreLabel, 1);

    // UIの生成
    for(int i = 0; i < uiNum; ++i) {
        Point pos = uiPositions[i];
        pos.x += visibleSize.width/2 + origin.x;
        uiSprites[i] = Sprite::create(uiFiles[i]);
        uiSprites[i]->setPosition(pos);
        uiSprites[i]->setAnchorPoint(uiUVs[i]);
        auto sprite = Sprite::create(uiFiles[i]);
        sprite->setAnchorPoint(Point(0, 0));
        sprite->setPosition(Point(640, 0));
        uiSprites[i]->addChild(sprite, 0);
        this->addChild(uiSprites[i], 50);
    }

    progIconSprite = Sprite::create("SnowWalkerProgressIcon.png");
    progIconSprite->setAnchorPoint(Point(0.5f,0));
    progIconSprite->setPosition(Point(visibleSize.width/2 - 302, uiPositions[uiProgress].y));
    this->addChild(progIconSprite, 50);

    return true;
}

void UILayer::Update(float deltaTime) {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Rect rect = uiSprites[uiLifeGauge]->getTextureRect();
    float wide = uiSprites[uiLifeGauge]->getTexture()->getPixelsWide();
    rect.size.width = wide * lifeRate;
    uiSprites[uiLifeGauge]->setTextureRect(rect);

    Point iconPos(visibleSize.width/2 - 302 + 604 * distRate, uiPositions[uiProgress].y);
    progIconSprite->setPosition(iconPos);
}

void UILayer::onEnter() {
	Layer::onEnter();
}

void UILayer::onExit() {

}

void UILayer::SetRecordTime(float record, float recordTop) {
    char text[64];
    sprintf(text, "Record Time %0.2f High %02.2f", record, recordTop);
    scoreLabel->setString(text);    
}

button::Type UILayer::CheckTouch(touch::Type touch, Point pos) {
    button::Type result = button::None;

    if(touch == touch::Began) {
        for(int i = 0; i < button::Num; ++i) {
            Point vec = pos - buttonDatas[i].pos;

            float dist = vec.getLength();
            if(dist < buttonDatas[i].scale) {
                result = buttonDatas[i].type;
                break;
            }
        }
    }
    return result;
}