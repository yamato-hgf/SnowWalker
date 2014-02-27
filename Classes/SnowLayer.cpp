//
//  SnowLayer.cpp
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#include "SnowLayer.h"
#include "Utility.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool SnowLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    appearSecCount = 0.f;
    appearIntervalSetting = 0;
    speedSetting = Point(640 / 16, 1136 / 8);
    return true;
}

void SnowLayer::Update(float deltaTime) {
	std::vector<SnowController*>::iterator snowItr = snowCtrls.begin();
	while(snowItr != snowCtrls.end()) {
		Point pos = (*snowItr)->sprite->getPosition();
		pos += (*snowItr)->speed * deltaTime;
        (*snowItr)->sprite->setPosition(pos);

		if(pos.y < 0) {
			(*snowItr)->sprite->getParent()->removeChild((*snowItr)->sprite);
			snowItr = snowCtrls.erase(snowItr);
		}
		else {
			++snowItr;
		}
	}

	if(appearIntervalSetting > 0 && appearSecCount > appearIntervalSetting) {
		appearSecCount = 0;

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point pos(visibleSize.width * 2 * CCRANDOM_0_1(), visibleSize.height);
		Sprite* sprite = Sprite::create("SnowWalkerSnows.png");
		sprite->retain();
        sprite->setPosition(pos);
		sprite->setTextureRect(Rect(24 * utRand(3), 24 * utRand(2), 24, 24));
        this->addChild(sprite);
        
        float range = utRand(0.1f,1.0f);
        sprite->setScale(range);
        SnowController* snowCtrl = new SnowController(sprite, speedSetting * range);
        snowCtrls.push_back(snowCtrl);
	}
	appearSecCount += deltaTime;
}

void SnowLayer::Setting(cocos2d::Point speed, float appearInterval) {
	speedSetting = speed;
	appearIntervalSetting = appearInterval;
}
