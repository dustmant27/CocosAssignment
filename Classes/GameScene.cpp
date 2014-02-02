#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    tapCount = 0;
    /////////////////////////////
    // 3. add your codes below...

    
    countLabel = (LabelTTF*)LabelTTF::create("0", "Arial", 30);
    countLabel->setPosition(Point(100, origin.y + visibleSize.height - countLabel->getContentSize().height));

    this->addChild(countLabel, 1);

    // add "HelloWorld" splash screen"
    statusSprite = (Sprite*)Sprite::create("Waiting.png");
    // position the sprite on the center of the screen
    statusSprite->setPosition(Point(visibleSize.width/2, visibleSize.height/2 ));

    // add the sprite as a child to this layer
    this->addChild(statusSprite, 0);
    
     touchListener = (EventListenerTouchOneByOne*)EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::touchBegan, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);
    isSecondPlayerConnected = false;
    connectToAppWarp();
    
    return true;
}

std::string genRandom()
{
    std::string charStr;
    srand (time(NULL));
    
    for (int i = 0; i < 10; ++i) {
        charStr += (char)(65+(rand() % (26)));
    }
    
    return charStr;
}

void GameScene::connectToAppWarp()
{
    isConnected = false;
    AppWarp::Client *warpClientRef;
        isFirstLaunch = !isFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->setRecoveryAllowance(60);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        userName = genRandom();
        warpClientRef->connect(userName);
}
void GameScene::menuCloseCallback(Object* pSender)
{
    disconnect();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::onChatReceived(AppWarp::chat chatevent)
{
    printf("onChatReceived..");
    if(chatevent.sender != userName)
    {
        std::size_t loc = chatevent.chat.find('!');
        std::string str1 = chatevent.chat.substr(0,loc);
        if(str1=="init" && isSecondPlayerConnected == false){
            isSecondPlayerConnected = true;
            GameScene::sendStringData("init!");
            statusSprite->setTexture(CCTextureCache::sharedTextureCache()
                                     ->addImage("Connected.png"));
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Connected.wav");
        }
        else if (str1=="l"){
            GameScene::disconnect();
            
            Director::getInstance()->pushScene(PlayerLoseScene::createScene());
            Director::getInstance()->pause();
        }
        else{
            std::string str2 = chatevent.chat.substr(loc+1);
            
            
            float x = (float)std::atof (str1.c_str());
            float y = (float)std::atof(str2.c_str());
            GameScene::createStar(Point(x, y));

        }
       
          }
}
void GameScene::disconnect()
{
    AppWarp::Client::getInstance()->leaveRoom(ROOM_ID);
    AppWarp::Client::getInstance()->disconnect();
}


void GameScene::onDisconnectDone(int res)
{
    printf("Disconnected..");
}

void GameScene::onConnectDone(int res)
{
    if (res==AppWarp::ResultCode::success)
    {        printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->joinRoom(ROOM_ID);
        GameScene::sendStringData("init!");
        
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {  }
    else if (res==AppWarp::ResultCode::bad_request)
    { }
    else
    { }
}

void GameScene::update(float time)
{
    
}


void GameScene::onJoinRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        printf("\nonJoinRoomDone .. SUCCESS\n");
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->subscribeRoom(ROOM_ID);
        
        
        Point origin = Director::getInstance()->getVisibleOrigin();
        Size visibleSize = Director::getInstance()->getVisibleSize();
        auto sprite = Sprite::create("ConnectedDisplay.png");
        
        sprite->setPosition(Point(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height - 20));
        
        this->addChild(sprite, 1);
    }
    else
        printf("\nonJoinRoomDone .. FAILED\n");
}


void GameScene::sendData(float x, float y)
{
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    
    std::stringstream str;
    str <<x << "!" <<y ;
    warpClientRef->sendChat(str.str());
}

void GameScene::sendStringData(std::string incoming)
{
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    
    warpClientRef->sendChat(incoming);
}

bool GameScene::touchBegan(Touch* touch, Event* event){
    if( isSecondPlayerConnected){
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("TapSound.wav");
    Point loc = touch->getLocation();
    sendData(loc.x, loc.y);
    tapCount ++;
        createStar(loc);
    countLabel->setString(std::to_string(tapCount));
    std::cout << tapCount;
    if(tapCount == 50){
        
        GameScene::sendStringData("l!");
        
         Director::getInstance()->pushScene(PlayerWinScene::createScene());
        getEventDispatcher()->removeEventListener(touchListener);
    }
        
    }
        return true;
}
void GameScene::createStar(Point p){
    auto starSprite = Sprite::create("tap.png");
    starSprite->setPosition(p);
    
    this->addChild(starSprite, 0);
    FadeOut *fadeout = FadeOut::create(1.0f);
    starSprite->runAction(fadeout);
}



