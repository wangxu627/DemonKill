#include "RankScene.h"

using namespace cocos2d;

CCScene* RankScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
		RankScene *layer = RankScene::create();
        CC_BREAK_IF(! layer);

		scene->addChild(layer);
        // add layer as a child to scene
    } while (0);

    // return the scene
    return scene;
}

bool RankScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

		golden = CCSprite::create("throne.jpg",CCRect(0,400,200,200));
		golden->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width / 3,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.7));
		silver = CCSprite::create("throne.jpg",CCRect(200,400,200,200));
		silver->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width / 3,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.6));
		brozen = CCSprite::create("throne.jpg",CCRect(400,400,200,200));
		brozen->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width / 3,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.5));

		this->addChild(golden);
		this->addChild(silver);
		this->addChild(brozen);

        bRet = true;
    } while (0);

    return bRet;
}
