#include "MainScene.h"
#include "GameScene.h"
#include "RankScene.h"
#include <stdlib.h>

using namespace cocos2d;
using namespace std;


CCScene* MainScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainScene *layer = MainScene::create();
        CC_BREAK_IF(! layer);

		scene->addChild(layer);
        // add layer as a child to scene
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

		mBackLayer = CCLayer::create();
		this->addChild(mBackLayer,lBackGround);

		mFallingLayer = CCLayer::create();
		this->addChild(mFallingLayer,lFalling);

		mMenuLayer = CCLayer::create();
		this->addChild(mMenuLayer,lMenu);

		CCSprite* background = CCSprite::create("background.png");
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setAnchorPoint(ccp(0,0));
		this->mBackLayer->addChild(background);

		playItem = CCMenuItemImage::create("btn-play.png","btn-play-down.png");
		playItem->setScale(0);
		playItem->setTarget(this,menu_selector(MainScene::playCallback));
		playItem->setPositionY(CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.45);

		rankItem = CCMenuItemImage::create("btn-about.png","btn-about-down.png");
		rankItem->setScale(0);
		rankItem->setTarget(this,menu_selector(MainScene::rankCallback));
		rankItem->setPositionY(CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.286);

		aboutItem = CCMenuItemImage::create("btn-about.png","btn-about-down.png");
		aboutItem->setScale(0);
		aboutItem->setTarget(this,menu_selector(MainScene::aboutCallback));
		aboutItem->setPositionY(CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.15);

		menu = CCMenu::create(playItem,rankItem,aboutItem,NULL);
		menu->setPositionY(0);
		//menu->setScale(0);
		this->mMenuLayer->addChild(menu);

		logo = CCSprite::create("logo.png");
		logo->setScale(0.0);
		logo->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width * 0.5,
			CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.7));
		this->mMenuLayer->addChild(logo);
		/*mStarParticles = CCParticleSystem::create("particles/bg-stars.plist");
		mStarParticles->setPosition(ccp(400,300));
		this->addChild(mStarParticles);*/
		
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("crystals.plist");

		CCScaleTo* scale = CCScaleTo::create(0.2,0.65);
		CCScaleTo* scale2 = CCScaleTo::create(0.2,0.65);
		CCScaleTo* scale3 = CCScaleTo::create(0.2,0.65);
		CCScaleTo* scale4 = CCScaleTo::create(0.2,0.65);
		//scale->setTarget(logo);
		logo->runAction(scale);
		playItem->runAction(scale2);
		rankItem->runAction(scale3);
		aboutItem->runAction(scale4);
		//menu->runAction(scale2);

		schedule(schedule_selector(MainScene::OnUpdate));

		srand(time(NULL));

        bRet = true;
    } while (0);

    return bRet;
}

void MainScene::OnUpdate(float dt)
{
	if(rand() % 100 < 3)
	{
		char name[16];
		sprintf(name,"crystals/%d.png",rand() % 5);

		CCSprite* sprt = CCSprite::createWithSpriteFrameName(name);
		short x = rand() % (int)CCEGLView::sharedOpenGLView()->getFrameSize().width;
		short y = CCEGLView::sharedOpenGLView()->getFrameSize().height + sprt->getContentSize().height / 2;
		float scale = 0.2 + ((rand() % 100) / 100.0) * 0.8;
		float speed = 2 * scale * sprt->getContentSize().height / 40.0;

		sprt->setPosition(ccp(x,y));
		sprt->setScale(scale);

		tGem gem(sprt,speed);
		mGemContainer.push_back(gem);

		this->mFallingLayer->addChild(sprt);

		//mStarParticles->addParticle();
	}

	vector<tGem>::iterator iter = mGemContainer.begin();
	for(;iter != mGemContainer.end();)
	{
		iter->_sprt->setPosition(ccp(iter->_sprt->getPositionX(),
									 iter->_sprt->getPositionY() - iter->_speed));
		if(iter->_sprt->getPositionY() < -iter->_sprt->getContentSize().height / 2)
		{
			this->mFallingLayer->removeChild(iter->_sprt,true);
			iter = mGemContainer.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

//void MainScene::onExit()
//{
//	//CCScaleTo* scale = CCScaleTo::create(0.2,0.0);
//	/*CCScaleTo* scale2 = CCScaleTo::create(0.2,0.0);
//	CCScaleTo* scale3 = CCScaleTo::create(0.2,0.0);
//	CCScaleTo* scale4 = CCScaleTo::create(0.2,0.0);*/
//	//scale->setTarget(logo);
//	menu->runAction(scale);
//	//playItem->runAction(scale2);
//	//rankItem->runAction(scale3);
//	//aboutItem->runAction(scale4);
//}

void MainScene::playCallback(cocos2d::CCObject* sender)
{
	prepareDisappear(2);
}
void MainScene::aboutCallback(cocos2d::CCObject* sender)
{

}
void MainScene::rankCallback(cocos2d::CCObject* sender)
{
	prepareDisappear(1);
}

void MainScene::onDisappear()
{
	if(mType == 1)
	{
		CCScene* rankScene = RankScene::scene();
		CCDirector::sharedDirector()->replaceScene(rankScene);
	}
	else if(mType == 2)
	{
		CCScene* gameScene = GameScene::scene();
		CCDirector::sharedDirector()->replaceScene(gameScene);
	}
}

void MainScene::prepareDisappear(int aType)
{
	CCScaleTo* scale = CCScaleTo::create(0.2,0.0);
	CCScaleTo* scale2 = CCScaleTo::create(0.2,0.0);
	CCScaleTo* scale3 = CCScaleTo::create(0.2,0.0);
	CCScaleTo* scale4 = CCScaleTo::create(0.2,0.0);
	CCCallFunc* callback = CCCallFunc::create(this,callfunc_selector(MainScene::onDisappear));
	logo->runAction(scale);
	playItem->runAction(scale2);
	rankItem->runAction(scale3);
	aboutItem->runAction(CCSequence::create(scale4,callback,NULL));

	mType = aType;
}




