#ifndef __RANKSCENE_H__
#define __RANKSCENE_H__

#include "cocos2d.h"
#include "Score.h"

class RankScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(RankScene);

private:
	
private:
	//cocos2d::CCSprite* golden;
	//cocos2d::CCSprite* silver;
	//cocos2d::CCSprite* brozen;

	cocos2d::CCSprite* titles[Score::SCORE_LEN];
	cocos2d::CCSprite* items[Score::SCORE_LEN];


	int mType;
};

#endif