#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "appwarp.h"
#include "PlayerLoseScene.h"
#include "PlayerWinScene.h"
#include "Box2D/Box2D.h"

#define APPWARP_APP_KEY     "9b1afff73182dae55131f1647f4bbb027d3662e10fa7c5f7b704ee0c33c6e562"
#define APPWARP_SECRET_KEY  "f64f4516a711e40b57a5afb48f9dff4a42eb893e51e251f2f4f6d117146c844e"
#define GAME_NAME           "CocosMultiplayer"
#define ROOM_ID             "1958064958"
#define ROOM_NAME           "BigRoom"
#define ROOM_OWNER          "Dustin"
#define MAX_PLAYER          10

using namespace cocos2d;
class GameScene : public cocos2d::Layer, public AppWarp::ConnectionRequestListener,public AppWarp::RoomRequestListener,public AppWarp::NotificationListener,public AppWarp::ZoneRequestListener

{
    
public:
    
    
    bool isConnected;
    int tapCount;
    std::string userName;
    EventListenerTouchOneByOne *touchListener;
    LabelTTF *countLabel;
    Sprite *statusSprite;
    bool isFirstLaunch;
    bool isSecondPlayerConnected;
        virtual void update(float time);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    void onConnectDone(int res);
    void onDisconnectDone(int res);
    void onChatReceived(AppWarp::chat chatevent);
    void onJoinRoomDone(AppWarp::room revent);
    void createStar(Point p);
    void connectToAppWarp();
    void sendData(float x, float y);
    void sendStringData(std::string winLose);
    bool touchBegan(Touch* touch, Event* event);
    
    void disconnect();
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __HELLOWORLD_SCENE_H__
