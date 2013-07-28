#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include <vector>

struct tGem
{
	cocos2d::CCSprite* _sprt;
	float _speed;

	tGem(cocos2d::CCSprite* sprt,float speed)
		: _sprt(sprt)
		, _speed(speed)
	{}
};

class MainScene : public cocos2d::CCLayer
{
	typedef unsigned char tLayer;enum
	{
		lBackGround = 0,
		lFalling,
		lMenu,
	};
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
	
    // implement the "static node()" method manually
    CREATE_FUNC(MainScene);

private:
	void OnUpdate(float dt);
	void playCallback(cocos2d::CCObject* sender);
	void aboutCallback(cocos2d::CCObject* sender);
	void rankCallback(cocos2d::CCObject* sender);
	void onDisappear();
	void prepareDisappear(int aType);
	
private:
	std::vector<tGem> mGemContainer;
	cocos2d::CCLayer* mFallingLayer;
	cocos2d::CCLayer* mBackLayer;
	cocos2d::CCLayer* mMenuLayer;
	cocos2d::CCParticleSystem* mStarParticles;
	cocos2d::CCMenu* menu;
	cocos2d::CCMenuItem* playItem;
	cocos2d::CCMenuItem* rankItem;
	cocos2d::CCMenuItem* aboutItem;
	cocos2d::CCSprite* logo;
	int mType;
};

#endif  // __HELLOWORLD_SCENE_H__