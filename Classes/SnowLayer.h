//
//  SnowLayer.h
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#ifndef __SnowWalker__SnowLayer__
#define __SnowWalker__SnowLayer__

#include "cocos2d.h"

class SnowLayer : public cocos2d::Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(SnowLayer);

    void Initialize();
    void Update(float deltaTime);
    void Setting(cocos2d::Point speed, float appearInterval);
    void Move(cocos2d::Point vector);
    float GetMoveSpeed() { return speedSetting.getLength(); }

private:
	struct SnowController {
		SnowController(cocos2d::Sprite* sprite_, cocos2d::Point speed_)
		: sprite(sprite_), speed(speed_), tremor(0) {}

		cocos2d::Sprite* sprite;
		cocos2d::Point speed;
		float tremor;
	};

	std::vector<SnowController*> snowCtrls;

	cocos2d::Point speedSetting;
	float appearIntervalSetting;

	float appearSecCount;

public: // debug
	int GetSnowFrakes() { return snowCtrls.size(); }
};

#endif /* defined(__SnowWalker__SnowLayer__) */
