#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    connectToAppWarp();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    tapCount = 0;
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

       // add the label as a child to this layer
    this->addChild(label, 1);
    
    countLabel = (LabelTTF*)LabelTTF::create("0", "Arial", 30);
    countLabel->setPosition(Point(100, origin.y + visibleSize.height - countLabel->getContentSize().height));

    this->addChild(countLabel, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);
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

void HelloWorld::connectToAppWarp()
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
void HelloWorld::menuCloseCallback(Object* pSender)
{
    disconnect();
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onChatReceived(AppWarp::chat chatevent)
{
    printf("onChatReceived..");
    if(chatevent.sender != userName)
    {
        std::size_t loc = chatevent.chat.find('!');
        std::string str1 = chatevent.chat.substr(0,loc);
        std::string str2 = chatevent.chat.substr(loc+1);
        float x = (float)std::atof (str1.c_str());
        float y = (float)std::atof(str2.c_str());
        HelloWorld::createStar(Point(x, y));
    }
}
void HelloWorld::disconnect()
{
    AppWarp::Client::getInstance()->leaveRoom(ROOM_ID);
    AppWarp::Client::getInstance()->disconnect();
}


void HelloWorld::onDisconnectDone(int res)
{}

void HelloWorld::onConnectDone(int res)
{
    if (res==AppWarp::ResultCode::success)
    {        printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->joinRoom(ROOM_ID);
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

void HelloWorld::update(float time)
{
    
}


void HelloWorld::onJoinRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        printf("\nonJoinRoomDone .. SUCCESS\n");
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        warpClientRef->subscribeRoom(ROOM_ID);
        
        HelloWorld::createStar(Point(100, 100));
    }
    else
        printf("\nonJoinRoomDone .. FAILED\n");
}

void HelloWorld::onSubscribeRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        printf("\nonSubscribeRoomDone .. SUCCESS\n");
        AppWarp::Client *warpClientRef;
        warpClientRef = AppWarp::Client::getInstance();
        std::map<std::string, std::string> properties;
        properties["1"] = "Owner";
        properties["2"] = "Manager";
        properties["3"] = "Team Lead";
        
        //  properties.insert(std::pair<std::string, std::string>("1","Owner"));
        
        std::vector<std::string> removeArray;
        warpClientRef->updateRoomProperties(ROOM_ID,properties, removeArray);
    }
    else
        printf("\nonSubscribeRoomDone .. FAILED\n");
}
void HelloWorld::sendData(float x, float y)
{
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    
    std::stringstream str;
    str <<x << "!" <<y ;
    warpClientRef->sendChat(str.str());
}

void HelloWorld::onUpdatePropertyDone(AppWarp::liveroom revent)
{
    if (revent.result==0)
    {
        printf("onUpdatePropertyDone....Success");
        std::map<std::string, std::string> properties = revent.properties;
        std::map<std::string,std::string>::iterator it;
        for(it = properties.begin(); it != properties.end(); ++it)
        {
            //cJSON_AddStringToObject(propJSON, it->first.c_str(),it->second.c_str());
            printf("key= %s...value= %s",it->first.c_str(),it->second.c_str());
        }
        
        
    }
    else
    {
        printf("onUpdatePropertyDone....Failed");
    }
}

bool HelloWorld::touchBegan(Touch* touch, Event* event){
    HelloWorld::createStar(touch->getLocation());
    Point loc = touch->getLocation();
    sendData(loc.x, loc.y);
    tapCount ++;
    countLabel->setString(std::to_string(tapCount));
    std::cout << tapCount;
    return true;
}
void HelloWorld::createStar(Point p){
    auto starSprite = Sprite::create("star.png");
    starSprite->setPosition(p);
    
    this->addChild(starSprite, 0);
}



