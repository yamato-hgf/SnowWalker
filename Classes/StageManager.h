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

class StageManager
{
public:
    StageManager();

    void Initialize();

    void Update(float deltaTime);
 
    void SetScrollSpeed(float scrlSpd_) {
    	scrlSpd = scrlSpd_;
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
        bgBuilding,
        bgDoorLight,
        bgNum
    };

    cocos2d::Sprite* bgSprites[bgNum];

    float scrlSpd;
};

#endif /* defined(__SnowWalker__StageLayer__) */
