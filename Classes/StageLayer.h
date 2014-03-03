//
//  StageLayer.h
//  SnowWalker
//
//  Created by 小川 穣 on 2014/02/25.
//
//

#ifndef __SnowWalker__StageLayer__
#define __SnowWalker__StageLayer__

#include "cocos2d.h"

class StageLayer : public cocos2d::Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(StageLayer);

    void Update(float deltaTime);
 
    void onEnter();
    void onExit();

    void SetScrollSpeed(float scrlRate_) {
    	scrlRate = scrlRate_;
    };

private:
    enum {
        bgSky,
        bgCloudBack,
        bgCloudFront,
        bgCity,
        bgRiver,
        bgBank,
        bgField,
        bgNum
    };

    cocos2d::Sprite* bgSprites[bgNum];

    float scrlRate;
};

#endif /* defined(__SnowWalker__StageLayer__) */
