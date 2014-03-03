//
//  UILayer.h
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#ifndef __SnowWalker__UILayer__
#define __SnowWalker__UILayer__

#include "cocos2d.h"
#include "Define.h"

class UILayer : public cocos2d::Layer
{
public:
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(UILayer);

    void Update(float deltaTime);
 
    void onEnter();
    void onExit();

    void SetLifeRate(float rate){ lifeRate = rate; }
    void SetDistRate(float rate){ distRate = rate; }
     void SetRecordTime(float record, float recordTop);

    button::Type CheckTouch(touch::Type touch, cocos2d::Point pos);

private:
    enum {
        uiLifeFrame,
        uiLifeGauge,
        uiLifeWarning,
        uiProgress,
        uiUnder,
        uiNum
    };

    cocos2d::LabelTTF* scoreLabel;
    cocos2d::Sprite* progIconSprite;
    cocos2d::Sprite* uiSprites[uiNum];

    float lifeRate;
    float distRate;
};

#endif /* defined(__SnowWalker__UILayer__) */
