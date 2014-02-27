#include "MainScene.h"
#include "SnowLayer.h"
#include "Utility.h"

USING_NS_CC;

const char* bgFiles[] = {
    "SnowWalkerSky.png",    // bgSky
    "SnowWalkerCloudBack.png",    // bgCloudBack
    "SnowWalkerCloudFront.png",    // bgCloudFront
    "SnowWalkerCity.png",    // bgCity
    "SnowWalkerRiver.png",    // bgRiver
    "SnowWalkerBank.png",    // bgBank
    "SnowWalkerField.png",    // bgField
    "SnowWalkerLifeFrame.png", // bgUILifeFrame
    "SnowWalkerLifeGauge.png", // bgUILifeGauge
    "SnowWalkerLifeWarning.png", // bgUILifeWarning
    "SnowWalkerProgress.png", // bgUIProgress
    "SnowWalkerUIUnder.png"    // bgUIUnder    
};

enum Buttons {
    ButtonSlow,
    ButtonJump,
    ButtonDash,
    ButtonDrink,
    ButtonNum
};

Point buttonPositions[] = {
    Point(64, 160),
    Point(135, 256),
    Point(196, 160),
    Point(400, 200)
};

const Point bgUVs[] = {
    Point(0.5f, 1),    // bgSky
    Point(0.5f, 1),    // bgCloudBack
    Point(0.5f, 1),    // bgCloudFront
    Point(0.5f, 0.5f),    // bgCity
    Point(0.5f, 0.5f),    // bgRiver
    Point(0.5f, 0.5f),    // bgBank
    Point(0.5f, 0),    // bgField
    Point(0.5f, 0.5f), // bgUILifeFrame
    Point(0.0f, 0.5f), // bgUILifeGauge
    Point(0.5f, 0.5f), // bgUILifeWarning
    Point(0.5f, 0.5f), // bgUIProgress
    Point(0.5f, 0)    // bgUIUnder
};

const Point bgPositions[] = {
    Point(0, 1136),    // bgSky
    Point(0, 1136),    // bgCloudBack
    Point(0, 1136),    // bgCloudFront
    Point(0, 920),    // bgCity
    Point(0, 600),    // bgRiver
    Point(0, 760),    // bgBank
    Point(0, 80),    // bgField
    Point(0, 1024), // bgUILifeFrame
    Point(-309, 1024), // bgUILifeGauge
    Point(128, 1034), // bgUILifeWarning
    Point(0, 960), // bgUIProgress
    Point(0, 0)    // bgUIUnder    
};

const float bgScrlSpds[] = {
    4,    // bgSky
    32,    // bgCloudBack
    48,    // bgCloudFront
    64,    // bgCity
    160,    // bgRiver
    120,    // bgBank
    240,    // bgField
    0,  // bgUILifeFrame
    0,  // bgUILifeGauge
    0,  // bgUILifeWarning
    0,  // bgUIProgress
    0    // bgUIUnder    
};

const float bgDepth[] = {
    0,    // bgSky
    0,    // bgCloudBack
    0,    // bgCloudFront
    0,    // bgCity
    0,    // bgRiver
    0,    // bgBank
    0,    // bgField
    50,  // bgUILifeFrame
    50,  // bgUILifeGauge
    50,  // bgUILifeWarning
    50,  // bgUIProgress
    50    // bgUIUnder    
};

float plHeight = 480;
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

inline float Rand0to1() {
    return (rand() % 10000) / 10000.f;
}

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

    scrlRate = 1;
    holdButton = ButtonNum;

    snowManAppSecCount = 0;
    snowManAppSec = snowManAppSecMin + (snowManAppSecMax - snowManAppSecMin) * Rand0to1();

    iceAppSecCount = 0;
    iceAppSec = iceAppSecMin + (iceAppSecMax - iceAppSecMin) * Rand0to1();

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

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    scoreLabel = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    scoreLabel->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - scoreLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(scoreLabel, 1);
/*
    // add "MainScene" splash screen"
    auto sprite = Sprite::create("MainScene.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
*/
    for(int i = 0; i < bgNum; ++i) {
        Point pos = bgPositions[i];
        pos.x += visibleSize.width/2 + origin.x;
        bgSprites[i] = Sprite::create(bgFiles[i]);
        bgSprites[i]->setPosition(pos);
        bgSprites[i]->setAnchorPoint(bgUVs[i]);
        auto sprite = Sprite::create(bgFiles[i]);
        sprite->setAnchorPoint(Point(0, 0));
        sprite->setPosition(Point(640, 0));
        bgSprites[i]->addChild(sprite, 0);
        this->addChild(bgSprites[i], bgDepth[i]);
    }

    snowLayer = SnowLayer::create();
    this->addChild(snowLayer, 25);
    snowLayer->Setting(Point(-640 / 3, -1136 / 2), 0.01f);

    progIconSprite = Sprite::create("SnowWalkerProgressIcon.png");
    progIconSprite->setAnchorPoint(Point(0.5f,0));
    progIconSprite->setPosition(Point(visibleSize.width/2 - 302, bgPositions[bgUIProgress].y));
    this->addChild(progIconSprite, bgDepth[bgUIProgress]);

    playerSprite = Sprite::create("SnowWalkerPlayer.png");
    playerSprite->setAnchorPoint(Point(0.5f,0));
    playerSprite->setPosition(Point(320, plHeight));
    this->addChild(playerSprite, 5);

    schedule(schedule_selector(MainScene::Update));

    auto listener = EventListenerTouchAllAtOnce::create();
//    listener->setSwallowTouches(true);

    listener->onTouchesBegan = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouchesBegan(touches, event);
    };

    listener->onTouchesMoved = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouchesMoved(touches, event);
    };

    listener->onTouchesEnded = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouchesEnded(touches, event);
    };

    listener->onTouchesCancelled = [](const Touches& touches, Event* event) {
        MainScene::Get()->onTouchesCancelled(touches, event);
    };

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
//    EventDispatcher::getInstance()->addEventListenerWithSceneGraphPriority(listener, playerSprite);


    // 画面サイズの取得
    Size size = Director::getInstance()->getWinSize();
    
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

void MainScene::onTouchesBegan(const Touches& touches, Event* event) {
    for(int touchIdx = 0; touchIdx < touches.size(); ++touchIdx) {
        Touch* touch = touches[touchIdx];
        Point location = touch->getLocation();
    //    log("location %f %f", location.x, location.y);
        for(int i = 0; i < ButtonNum; ++i) {
            Point vec = location - buttonPositions[i];
            float dist = vec.getLength();
            if(dist < 64) {
                if(holdButton != i) {
                    holdButton = i;
                    switch(holdButton) {
                    case ButtonDash:
                        break;
                    case ButtonJump:
                        if(plPosY == 0) {
                            jumpSpeed = plJumpAccel;
                        }
                        break;
                    case ButtonDrink:
                        life += lifeGainDrink;
                        break;
                    }
                }
                break;
            }
        }
    }
}

void MainScene::onTouchesMoved(const Touches& touches, Event* event) {

}

void MainScene::onTouchesEnded(const Touches& touches, Event* event) {

}

void MainScene::onTouchesCancelled(const Touches& touches, Event* event) {

}

void MainScene::Update(float deltaTime) {
    debugDrawNode->clear();
    snowLayer->Update(deltaTime);

    switch(state) {
    case StateTitle:
        UpdateTitle(deltaTime);
        break;

    case StateGame:
        UpdateGame(deltaTime);
        break;
    }
}

void MainScene::UpdateTitle(float deltaTime) {

}

void MainScene::UpdateGame(float deltaTime) {
    Size visibleSize = Director::getInstance()->getVisibleSize();

    for(int i = 0; i < bgNum; ++i) {
        Point pos = bgSprites[i]->getPosition();
        pos.x -= bgScrlSpds[i] * scrlRate * deltaTime;
        if(pos.x < -320)
            pos.x += 640;
        pos.y = bgPositions[i].y;
        bgSprites[i]->setPosition(pos);
    }

    Point plPos(160,plHeight+plPosY);
    Point plColliPos(plPos + Point(0, 16));

    distProg += bgScrlSpds[bgField] * scrlRate * deltaTime * (1/64.f);

    debugDrawNode->drawDot(plColliPos, 32, Color4F(0,1,0,0.5f));
    playerSprite->setPosition(plPos);

    Point iconPos(visibleSize.width/2 - 302 + 604 * (distProg / distGoal), bgPositions[bgUIProgress].y);
    progIconSprite->setPosition(iconPos);

    bool hit = false;
    std::vector<Sprite*>::iterator itr = snowManSprites.begin();
    while(itr != snowManSprites.end()) {
        Sprite* sprite = (*itr);
        Point pos = sprite->getPosition();
        pos.x -= bgScrlSpds[bgField] * scrlRate * deltaTime;
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
        snowManAppSec = (snowManAppSecMin + (snowManAppSecMax - snowManAppSecMin) * Rand0to1()) * rate;        

        Sprite *sprite = Sprite::create("SnowWalkerSnowManM.png");
        sprite->retain();
        sprite->setAnchorPoint(Point(0.5f, 0));
        sprite->setPosition(Point(640 + 320, plHeight));
        addChild(sprite);
        snowManSprites.push_back(sprite);
    }

    itr = iceSprites.begin();
    while(itr != iceSprites.end()) {
        Sprite* sprite = (*itr);
        Point pos = sprite->getPosition();
        pos.x -= bgScrlSpds[bgField] * scrlRate * deltaTime;
        sprite->setPosition(pos);
        sprite->setScale(0.5f);
     
        if(fabsf(plPos.x - pos.x) < 128 && plPosY == 0) {   // xが範囲内で着地中か？
            if(holdButton != ButtonSlow)
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
        iceAppSec = (iceAppSecMin + (iceAppSecMax - iceAppSecMin) * Rand0to1()) * rate;        

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

    switch(holdButton) {
    case ButtonSlow:
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

    float rate = life / lifeMax;
    Rect rect = bgSprites[bgUILifeGauge]->getTextureRect();
    float wide = bgSprites[bgUILifeGauge]->getTexture()->getPixelsWide();
    rect.size.width = wide * rate;
    bgSprites[bgUILifeGauge]->setTextureRect(rect);

    static float snowSec = 0;
    if(snowSec > 5) {
        snowLayer->Setting(Point(-640 / utRand(1, 8), -1136 / utRand(2, 16)), utRand(0.1f, 0.001f));
            snowSec = 0;
    }
    snowSec += deltaTime;

    record += deltaTime;
    char text[64];
    sprintf(text, "Record Time %0.2f High %02.2f", record, recordTop);
    scoreLabel->setString(text);
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
    }
}

void MainScene::SetScrlRate(float scrlRate_) { 
    scrlRate = scrlRate_; 
    life -= lifeDownDash;
}
