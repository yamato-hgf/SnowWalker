#include "MainScene.h"
#include "UILayer.h"
#include "SnowLayer.h"
#include "StageManager.h"
#include "Utility.h"

USING_NS_CC;

float plHeight = 380;
float lifeMax = 200;
float life1st = 100;
float dranker = 145;
float lifeDownWalk = 10;
float lifeDownDash = 20;
float lifeGain = 15;
float lifeGainDrink = 15;
float plJumpAccel = 15;
float plFallSpeed = 32;

float snowManAppSecMin = 90.0f;
float snowManAppSecMax = 120.0f;

float iceAppSecMin = 30.0f;
float iceAppSecMax = 40.0f;

float distGoal = 3000;
float scrlSpd = 240;

MainScene* MainScene::instance = NULL;

namespace {
    struct LevelData {
        enum Type {
            Snow,
            Num
        } type;
        float appearSec;
        float repeatSec;
        Point vector;  // 方向
    } const levelDatas[] = {
/*        
        { LevelData::Snow, 5, 0.001f, Point(-880, -960) },
        { LevelData::Snow, 8, 0.2f, Point(-640, -960) },
        { LevelData::Snow, 10, 0.5f, Point(0, -960) },
        { LevelData::Snow, 15, 2.1f, Point(-320, -960) },
        { LevelData::Snow, 18, 0.1f, Point(-320, -960) },
*/
        { LevelData::Num }
    };

    float dmgBySnowSpd = 320;
    float recoverProgTime = 1;
    float recoverBaseValue = 30;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    instance = this;

    srand((unsigned int)time(NULL));
    
    recordTop = 0;
    record = 0;

    levelIndex = 0;

    debugMenu = NULL;
    debugSnowFallSpeed = -960;
    debugSnowMoveSpeed = 0;
    debugSnowRepeatSec = 0.01f;

    life = life1st;

    touchType = touch::None;

    recoverLife = 0;
    recoveredLife = 0;
    recoverSecCount = 0;

    scrlRate = 1;

    snowManAppSecCount = 0;
    snowManAppSec = snowManAppSecMin + (snowManAppSecMax - snowManAppSecMin) * utRand(1.f);

    iceAppSecCount = 0;
    iceAppSec = iceAppSecMin + (iceAppSecMax - iceAppSecMin) * utRand(1.f);


    // ステージの生成
    stageManager = new StageManager();

    // プレイヤーの生成
    playerSprite = Sprite::create("SnowWalkerPlayer.png");
    playerSprite->setAnchorPoint(Point(0.5f,0));
    playerSprite->setPosition(Point(320, plHeight));
    this->addChild(playerSprite, layer::player);

    // 降雪の生成
    snowLayer = SnowLayer::create();
    this->addChild(snowLayer, layer::snow);
    snowLayer->Setting(Point(-640 / 3, -1136 / 2), 0.01f);

    // UIの生成
    uiLayer = UILayer::create();
    this->addChild(uiLayer, layer::snow);

    schedule(schedule_selector(MainScene::Update));

    auto listener = EventListenerTouchAllAtOnce::create();
//    listener->setSwallowTouches(true);

    listener->onTouchesBegan = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouches(touches, event, touch::Began);
    };

    listener->onTouchesMoved = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouches(touches, event, touch::Moved);
    };

    listener->onTouchesEnded = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouches(touches, event, touch::Ended);
    };

    listener->onTouchesCancelled = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouches(touches, event, touch::Cancelled);
    };

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    // 描画用ノードの作成
    debugDrawNode = DrawNode::create();
    this->addChild(debugDrawNode, layer::debug);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    // position the label on the center of the screen
    debugInfo = LabelTTF::create("debug", "Arial", 24);
    debugInfo->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8 * 7 ));
    debugInfo->setColor(Color3B::RED);

    // add the label as a child to this layer
    this->addChild(debugInfo, layer::debug);

    BeginState(StateTitle);

    return true;
}

void MainScene::onEnter() {
    Scene::onEnter();
}
 
void MainScene::onExit() {
}   

void MainScene::onTouches(const Touches& touches, Event* event, touch::Type type) {
    touchType = type;

    for(int touchIdx = 0; touchIdx < touches.size(); ++touchIdx) {
        Touch* touch = touches[touchIdx];
        buttonType = uiLayer->CheckTouch(touchType, touch->getLocation());
        if(buttonType != button::None)
            break;
    }
}

void MainScene::Update(float deltaTime) {
    debugDrawNode->clear();

    switch(buttonType) {
    case button::Dash:
        break;
    case button::Jump:
        if(plPosY == 0) {
            jumpSpeed = plJumpAccel;
        }
        break;
    case button::Drink:
        recoverLife += recoverBaseValue - recoveredLife;
        recoverSecTotal = recoverProgTime;
//        recoverSecTotal += recoverProgTime - recoverSecCount;
        recoveredLife = 0;
        recoverSecCount = 0;
        break;
            
        default:
            break;
    }

    switch(state) {
    case StateTitle:
        UpdateTitle(deltaTime);
        break;

    case StateGame:
        UpdateGame(deltaTime);
        break;

    case StateFailed:
        UpdateFailed(deltaTime);
        break;

    case StateDebug:
        UpdateDebug(deltaTime);
        default:
            break;
    }

    touchType = touch::None;
    buttonType = button::None;

    uiLayer->Update(deltaTime);
    snowLayer->Update(deltaTime);
    stageManager->Update(deltaTime);
}

void MainScene::UpdateTitle(float deltaTime) {
    if(debugMenu != NULL)
        return;

    if(touchType == touch::Began)
        BeginState(StateGame);
}

void MainScene::UpdateGame(float deltaTime) {
    Point plPos(160,plHeight+plPosY);
    Point plColliPos(plPos + Point(0, 16));

    distProg += scrlSpd * scrlRate * deltaTime * (1/64.f);

    stageManager->SetScrollSpeed(scrlSpd * scrlRate);

    debugDrawNode->drawDot(plColliPos, 32, Color4F(0,1,0,0.5f));
    playerSprite->setPosition(plPos);

    bool hit = false;

    // 雪だるまの衝突判定
    std::vector<Sprite*>::iterator itr = snowManSprites.begin();
    while(itr != snowManSprites.end()) {
        Sprite* sprite = (*itr);
        Point pos = sprite->getPosition();
        pos.x -= scrlSpd * scrlRate * deltaTime;
        sprite->setPosition(pos);
     
        Point colliPosBottom = pos + Point(0,16);
        Point colliPosTop = colliPosBottom + Point(0,64);

        // 球とシリンダーの判定
        if(fabsf(plPos.x - pos.x) < 64) {   // xが範囲内か？
            if((plColliPos.y < colliPosTop.y && 
                plColliPos.y > colliPosBottom.y) // yが内側か？
            || plColliPos.getDistance(colliPosBottom) < 64
            || plColliPos.getDistance(colliPosTop) < 64)    // 先端に触れたか？
            {
                hit = true;
            }
        }

        /* 線の描画 */
        debugDrawNode->drawSegment(colliPosTop, colliPosBottom, 32, Color4F(1,0,0,0.25f));
        
        if(pos.x < -480.f) {
            removeChild(sprite);
            itr = snowManSprites.erase(itr);
        }
        else ++itr;
    }

    // 雪だるまの出現管理
    snowManAppSecCount += deltaTime;
    if(snowManAppSecCount > snowManAppSec) {
        float rate = 1 - (record / (60 * 60 * 3));

        snowManAppSecCount = 0;        
        snowManAppSec = (snowManAppSecMin + (snowManAppSecMax - snowManAppSecMin) * utRand(1.f)) * rate;

        Sprite *sprite = Sprite::create("SnowWalkerSnowManM.png");
        sprite->retain();
        sprite->setAnchorPoint(Point(0.5f, 0));
        sprite->setPosition(Point(640 + 320, plHeight));
        addChild(sprite, layer::object);
        snowManSprites.push_back(sprite);
    }

    // 氷の衝突判定
    itr = iceSprites.begin();
    while(itr != iceSprites.end()) {
        Sprite* sprite = (*itr);
        Point pos = sprite->getPosition();
        pos.x -= scrlSpd * scrlRate * deltaTime;
        sprite->setPosition(pos);
        sprite->setScale(0.5f);
     
        if(fabsf(plPos.x - pos.x) < 128 && plPosY == 0) {   // xが範囲内で着地中か？
            if(buttonType != button::Slow)
                hit = true;
        }

        if(pos.x < -480.f) {
            removeChild(sprite);
            itr = iceSprites.erase(itr);
        }
        else ++itr;
    }

   // 氷の出現管理    
    iceAppSecCount += deltaTime;
    if(iceAppSecCount > iceAppSec) {
        float rate = 1 - (record / (60 * 60 * 3));

        iceAppSecCount = 0;        
        iceAppSec = (iceAppSecMin + (iceAppSecMax - iceAppSecMin) * utRand(1.f)) * rate;

        Sprite *sprite = Sprite::create("SnowWalkerIce.png");
        sprite->retain();
        sprite->setPosition(Point(640 + 320, plHeight));
        addChild(sprite, layer::object);
        iceSprites.push_back(sprite);
    }

    plPosY += jumpSpeed;
    if(plPosY > 0) {
        jumpSpeed -= plFallSpeed * deltaTime;
    }
    else {
        plPosY = 0;
        jumpSpeed = 0;
    }

    float snowSpeed = snowLayer->GetMoveSpeed();
    float dmgRate = snowSpeed * (1 / dmgBySnowSpd);

    switch(buttonType) {
    case button::Slow:
        scrlRate = 0.5f;
        life -= lifeDownWalk * dmgRate * deltaTime;
        break; 
    default:
        scrlRate += (1.f - scrlRate) * deltaTime;
        life -= lifeDownWalk * dmgRate * deltaTime;
        break;
    }
    if(life > dranker) {
        life += lifeGain * deltaTime;
    }

    if(recoverLife > 0) {        
        recoverSecCount += deltaTime;
        int recoverLifePrev = recoveredLife;
        float rate = 1;

        if(recoverSecCount < recoverSecTotal) {
            rate = recoverSecCount / recoverSecTotal;
            rate = sinf(3.14f / 2 * rate);
            recoveredLife = recoverLife * rate;
            life += recoveredLife - recoverLifePrev;
        }
        else {
            life += recoverLife - recoverLifePrev;
            recoverLife = 0;
            recoveredLife = 0;
            recoverSecCount = 0;
            recoverSecTotal = 0;
        }
    }

    char debugText[128];
    sprintf(debugText, "recLife %d recedLife %d \n recSecCnt %.2f recSecTtl %.2f", 
            recoverLife, recoveredLife, recoverSecCount, recoverSecTotal);
    debugInfo->setString(debugText);

    if(life < 0 || life > lifeMax || hit) {
        stageManager->SetScrollSpeed(0);
        BeginState(StateFailed);
    }

    const LevelData& levelData = levelDatas[levelIndex];
    if(levelData.type != LevelData::Num && record > levelData.appearSec) {
        snowLayer->Setting(levelData.vector, levelData.repeatSec);
        levelIndex++;
    }

    record += deltaTime;
    uiLayer->SetLifeRate(life / lifeMax);
    uiLayer->SetDistRate(distProg / distGoal);
    uiLayer->SetRecordTime(record, recordTop);
}

void MainScene::UpdateFailed(float deltaTime) {
    if(touchType == touch::Began) {
        stageManager->Initialize();
        BeginState(StateTitle);
    }
}

void MainScene::UpdateDebug(float deltaTime) {
    char debugText[128];
    sprintf(debugText, "snow spd.x %.2f y %.2f \n repeat %.2f num %d", 
            debugSnowMoveSpeed, debugSnowFallSpeed, 
            debugSnowRepeatSec, snowLayer->GetSnowFrakes());
    debugInfo->setString(debugText);
}

void MainScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainScene::BeginState(State state_) {
    switch(state) {
    case StateInit:
        break;
    case StateLogo:
        break;
    case StateTitle:
        break;
    case StateGame:
        snowLayer->Initialize();
        for(std::vector<Sprite*>::iterator itr = snowManSprites.begin();
            itr != snowManSprites.end(); itr = snowManSprites.erase(itr))
        {
            removeChild(*itr);
        }
        for(std::vector<Sprite*>::iterator itr = iceSprites.begin();
            itr != iceSprites.end(); itr = iceSprites.erase(itr))
        {
            removeChild(*itr);
        }
        break;
    case StateFailed:
        break;
    case StateResult:
        break;
    case StateDebug:
        removeChild(debugMenu, true);
        break;
    default:
        break;
    }

    ValueVector items;
    switch(state = state_) {
    case StateInit:
        break;
    case StateLogo:
        break;
    case StateTitle:
        levelIndex = 0;
        recoverLife = 0;
        recoveredLife = 0;
        recoverSecCount = 0;
        recoverSecTotal = 0;

        CreateDebugMenu(
            MenuItemFont::create("Start", [](Object* sender) { Get()->BeginState(StateGame); }),
            MenuItemFont::create("Debug", [](Object* sender) { Get()->BeginState(StateDebug); }),
            NULL
        );

        life = life1st;
        if(record > recordTop)
            recordTop = record;
        record = 0;
        break;
    case StateGame:
        break;
    case StateFailed:
        break;
    case StateResult:
        break;
    case StateDebug:
        CreateDebugMenu(
            MenuItemFont::create("snow spd.x--", [](Object* sender) { Get()->debugSnowMoveSpeed -= 32; Get()->DebugFallSnow(); }),
            MenuItemFont::create("snow spd.x++", [](Object* sender) { Get()->debugSnowMoveSpeed += 32; Get()->DebugFallSnow(); }),
            MenuItemFont::create("snow spd.y--", [](Object* sender) { Get()->debugSnowFallSpeed -= 32; Get()->DebugFallSnow(); }),
            MenuItemFont::create("snow spd.y++", [](Object* sender) { Get()->debugSnowFallSpeed += 32; Get()->DebugFallSnow(); }),
            MenuItemFont::create("snow repeat--", [](Object* sender) { Get()->debugSnowRepeatSec += 0.001f; Get()->DebugFallSnow(); }),
            MenuItemFont::create("snow repeat++", [](Object* sender) { Get()->debugSnowRepeatSec -= 0.001f; Get()->DebugFallSnow(); }),
            NULL
        );
        break;
    default:
        break;
    }
}

void MainScene::CreateDebugMenu(MenuItem* item, ...) {
    va_list args;
    va_start(args,item);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    Point pos(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);

    if(debugMenu != NULL)
        this->removeChild(debugMenu, true);
    debugMenu = Menu::createWithItems(item, args);
    debugMenu->alignItemsVerticallyWithPadding(32);
    debugMenu->setPosition(pos);
    debugMenu->setColor(Color3B::RED);
    this->addChild(debugMenu, layer::debug);
    
    va_end(args);    
}

void MainScene::DebugFallSnow() {
    snowLayer->Setting(Point(debugSnowMoveSpeed, debugSnowFallSpeed), debugSnowRepeatSec);    
}