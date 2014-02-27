
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class SnowLayer;

typedef std::vector<cocos2d::Touch*> Touches;

class MainScene : public cocos2d::Scene
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

    static MainScene* const Get() { return instance; }

    void SetScrlRate(float scrlRate_);

    void onTouchesBegan(const Touches& touches, cocos2d::Event* event);
    void onTouchesMoved(const Touches& touches, cocos2d::Event* event);
    void onTouchesEnded(const Touches& touches, cocos2d::Event* event);
    void onTouchesCancelled(const Touches& touches, cocos2d::Event* event);

private:
    static MainScene* instance;

    enum State {
        StateInit,
        StateLogo,
        StateTitle,
        StateGame,
        StateFailed,
        StateResult,
        StateNum
    };
    State state;
    State const GetState() { return state; }


    enum {
        bgSky,
        bgCloudBack,
        bgCloudFront,
        bgCity,
        bgRiver,
        bgBank,
        bgField,
        bgUILifeFrame,
        bgUILifeGauge,
        bgUILifeWarning,
        bgUIProgress,
        bgUIUnder,
        bgNum
    };

    cocos2d::LabelTTF* scoreLabel;

    cocos2d::Sprite* playerSprite;
    cocos2d::Sprite* progIconSprite;
    cocos2d::Sprite* bgSprites[bgNum];
    std::vector<cocos2d::Sprite*> snowManSprites;
    std::vector<cocos2d::Sprite*> iceSprites;
    SnowLayer* snowLayer;

    float record;
    float recordTop;

    float scrlRate;
    float snowManAppSecCount;
    float snowManAppSec;

    float iceAppSecCount;
    float iceAppSec;

    float distProg;

    float life;
    int holdButton;
    float plPosY;
    float jumpSpeed;


private:    // debug
    cocos2d::DrawNode* debugDrawNode;
 
private:    // methods
    void onEnter();
    void onExit();

    void Update(float deltaTime);
    void UpdateTitle(float deltaTime);
    void UpdateGame(float deltaTime);

    void BeginState(State state_);
};

#endif // __MAIN_SCENE_H__
