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

namespace {
	float tremorSpeed = 1;
	float outsideDist = 32;
}

// on "init" you need to initialize your instance
bool SnowLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Initialize();
    return true;
}

void SnowLayer::Initialize() {
    appearSecCount = 0.f;
    appearIntervalSetting = 0;
    speedSetting = Point(640 / 16, 1136 / 8);	
}

void SnowLayer::Update(float deltaTime) {
	std::vector<SnowController*>::iterator snowItr = snowCtrls.begin();
	while(snowItr != snowCtrls.end()) {
		SnowController* ctrl = (*snowItr);
		Point pos = ctrl->sprite->getPosition();
		pos += ctrl->speed * deltaTime;
		pos.x += sinf(ctrl->tremor) * tremorSpeed;
		ctrl->tremor += deltaTime;
        ctrl->sprite->setPosition(pos);

        Size visibleSize = Director::getInstance()->getVisibleSize();
		if(((ctrl->speed.x < 0 && pos.x < -outsideDist) ||
			(ctrl->speed.x > 0 && pos.x > visibleSize.width + outsideDist)) || pos.y < 0)
		{
			ctrl->sprite->getParent()->removeChild(ctrl->sprite);
			snowItr = snowCtrls.erase(snowItr);
		}
		else {
			++snowItr;
		}
	}

	while(appearIntervalSetting > 0 && appearSecCount > appearIntervalSetting) {
		appearSecCount -= appearIntervalSetting;

		Size visibleSize = Director::getInstance()->getVisibleSize();
        float range = utRand(0.1f,1.0f);
        Point speed = speedSetting * range;
        float offset = speed.x * (visibleSize.height/ speed.y);
        float width = visibleSize.width + fabsf(offset);
		Point pos(utRand(fminf(0, offset), offset + width)
				, visibleSize.height + outsideDist);

		Sprite* sprite = Sprite::create("SnowWalkerSnows.png");
		sprite->retain();
        sprite->setPosition(pos);
		sprite->setTextureRect(Rect(24 * utRand(3), 24 * utRand(2), 24, 24));
        this->addChild(sprite);
        
        sprite->setScale(range);
        SnowController* snowCtrl = new SnowController(sprite, speed);
        snowCtrls.push_back(snowCtrl);
	}
	appearSecCount += deltaTime;
}

void SnowLayer::Setting(cocos2d::Point speed, float appearInterval) {
	speedSetting = speed;
	appearIntervalSetting = appearInterval;
}
