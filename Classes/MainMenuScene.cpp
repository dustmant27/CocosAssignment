#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
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
                                           CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
    
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
    
//    auto label = LabelTTF::create("Hello World", "Arial", 24);
//    
//    // position the label on the center of the screen
//    label->setPosition(Point(origin.x + visibleSize.width/2,
//                             origin.y + visibleSize.height - label->getContentSize().height));
//    
//    // add the label as a child to this layer
//    this->addChild(label, 1);
    
    // add "MainMenuScene" splash screen"
    auto titleSprite = Sprite::create("Title.png");
    
    // position the sprite on the center of the screen
    titleSprite->setPosition(Point(origin.x + visibleSize.width/2,
                              origin.y + visibleSize.height - titleSprite->getContentSize().height));
    
    // add the sprite as a child to this layer
    this->addChild(titleSprite, 0);
    
    
    
    // add "MainMenuScene" splash screen"
    auto instructionSprite = Sprite::create("Instructions.png");
    
    // position the sprite on the center of the screen
    instructionSprite->setPosition(Point(origin.x + visibleSize.width/2 ,
                                   origin.y + visibleSize.height/2 ));
    
    // add the sprite as a child to this layer
    this->addChild(instructionSprite, 2);
    
    
    
    auto gameButton = MenuItemImage::create("PlayButton.png", "PlayButton.png",
                                            CC_CALLBACK_1(MainMenuScene::gameButtonCallback, this));
    
    gameButton->setPosition(Point(origin.x + visibleSize.width/2 ,
                                  origin.y + 100 ));
    
    
    auto gameButtonMenu = Menu::create(gameButton, NULL);
    gameButtonMenu->setPosition(Point::ZERO);
    
    this->addChild(gameButtonMenu, 1);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("LevelMusic.mp3");
    return true;
}

void MainMenuScene::gameButtonCallback(Object* pSender)
{
    
    Director::getInstance()->pushScene(GameScene::createScene());
}

void MainMenuScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
