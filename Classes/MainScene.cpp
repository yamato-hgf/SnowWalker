#include "MainScene.h"
#include "UILayer.h"
#include "SnowLayer.h"
#include "StageLayer.h"
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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    recordTop = 0;
    record = 0;

    life = life1st;

    touchType = touch::None;

    scrlRate = 1;

    snowManAppSecCount = 0;
    snowManAppSec = snowManAppSecMin + (snowManAppSecMax - snowManAppSecMin) * utRand(1.f);

    iceAppSecCount = 0;
    iceAppSec = iceAppSecMin + (iceAppSecMax - iceAppSecMin) * utRand(1.f);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    // ステージの生成
    stageLayer = StageLayer::create();
    this->addChild(stageLayer, 20);

    // プレイヤーの生成
    playerSprite = Sprite::create("SnowWalkerPlayer.png");
    playerSprite->setAnchorPoint(Point(0.5f,0));
    playerSprite->setPosition(Point(320, plHeight));
    this->addChild(playerSprite, 30);

    // 降雪の生成
    snowLayer = SnowLayer::create();
    this->addChild(snowLayer, 40);
    snowLayer->Setting(Point(-640 / 3, -1136 / 2), 0.01f);

    // UIの生成
    uiLayer = UILayer::create();
    this->addChild(uiLayer, 50);

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
    this->addChild(debugDrawNode, 100);

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
    snowLayer->Update(deltaTime);

    switch(buttonType) {
    case button::Dash:
        break;
    case button::Jump:
        if(plPosY == 0) {
            jumpSpeed = plJumpAccel;
        }
        break;
    case button::Drink:
        life += lifeGainDrink;
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
            
        default:
            break;
    }

    touchType = touch::None;
    buttonType = button::None;
}

void MainScene::UpdateTitle(float deltaTime) {
    if(touchType == touch::Began)
        BeginState(StateGame);
}

void MainScene::UpdateGame(float deltaTime) {
    Point plPos(160,plHeight+plPosY);
    Point plColliPos(plPos + Point(0, 16));

    distProg += scrlSpd * scrlRate * deltaTime * (1/64.f);

    stageLayer->SetScrollSpeed(scrlRate);
    stageLayer->Update(deltaTime);

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
        addChild(sprite);
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
        addChild(sprite);
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

    switch(buttonType) {
    case button::Slow:
        scrlRate = 0.5f;
        life -= lifeDownWalk * deltaTime;
        break; 
    default:
        scrlRate += (1.f - scrlRate) * deltaTime;
        life -= lifeDownWalk * deltaTime;
        break;
    }
    if(life > dranker) {
        life += lifeGain * deltaTime;
    }

    if(life < 0 || life > lifeMax || hit) {
        BeginState(StateTitle);
    }

    static float snowSec = 0;
    if(snowSec > 5) {
        snowLayer->Setting(Point(-640 / utRand(1, 8), -1136 / utRand(2, 16)), utRand(0.1f, 0.001f));
            snowSec = 0;
    }
    snowSec += deltaTime;

    record += deltaTime;
    uiLayer->SetLifeRate(life / lifeMax);
    uiLayer->SetDistRate(distProg / distGoal);
    uiLayer->SetRecordTime(record, recordTop);
    uiLayer->Update(deltaTime);
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
        default:
            break;
    }

    switch(state = state_) {
    case StateInit:
        break;
    case StateLogo:
        break;
    case StateTitle:
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
        default:
            break;
    }
}

void MainScene::SetScrlRate(float scrlRate_) { 
    scrlRate = scrlRate_; 
    life -= lifeDownDash;
}
