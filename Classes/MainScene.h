
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "Define.h"

class UILayer;
class SnowLayer;
class StageLayer;

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

    void onTouches(const Touches& touches, cocos2d::Event* event, touch::Type type);

private:    // methods
    enum State {
        StateInit,
        StateLogo,
        StateTitle,
        StateGame,
        StateFailed,
        StateResult,
        StateNum
    };
    
    void onEnter();
    void onExit();

    void Update(float deltaTime);
    void UpdateTitle(float deltaTime);
    void UpdateGame(float deltaTime);

    void BeginState(State state_);
    State const GetState() { return state; }

private:
    static MainScene* instance;

    State state;
    button::Type buttonType;
    touch::Type touchType;

    cocos2d::Sprite* playerSprite;
    std::vector<cocos2d::Sprite*> snowManSprites;
    std::vector<cocos2d::Sprite*> iceSprites;
    UILayer* uiLayer;
    SnowLayer* snowLayer;
    StageLayer* stageLayer;

    float record;
    float recordTop;

    float scrlRate;
    float snowManAppSecCount;
    float snowManAppSec;

    float iceAppSecCount;
    float iceAppSec;

    float distProg;

    float life;
    float plPosY;
    float jumpSpeed;

private:    // debug
    cocos2d::DrawNode* debugDrawNode;
};

#endif // __MAIN_SCENE_H__
