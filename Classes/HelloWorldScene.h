#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "appwarp.h"


#define APPWARP_APP_KEY     "9b1afff73182dae55131f1647f4bbb027d3662e10fa7c5f7b704ee0c33c6e562"
#define APPWARP_SECRET_KEY  "f64f4516a711e40b57a5afb48f9dff4a42eb893e51e251f2f4f6d117146c844e"
#define GAME_NAME           "CocosMultiplayer"
#define ROOM_ID             "1627120740"
#define ROOM_NAME           "TestRoom"
#define ROOM_OWNER          "Dustin"
#define MAX_PLAYER          2

using namespace cocos2d;
class HelloWorld : public cocos2d::Layer, public AppWarp::ConnectionRequestListener,public AppWarp::RoomRequestListener,public AppWarp::NotificationListener,public AppWarp::ZoneRequestListener

{
    
public:
    
    
    bool isConnected;
    std::string userName;
    bool isFirstLaunch;
        virtual void update(float time);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    void onConnectDone(int res);
    void onDisconnectDone(int res);
    void onJoinRoomDone(AppWarp::room revent);
    void onSubscribeRoomDone(AppWarp::room revent);
    void onUpdatePropertyDone(AppWarp::liveroom revent);
    void createStar(Point p);
    void connectToAppWarp();
    void sendData(float x, float y, float duration);

    bool touchBegan(Touch* touch, Event* event);
    
    void disconnect();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
