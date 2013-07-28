#include "RankScene.h"
#include "Score.h"

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

		CCSprite* background = CCSprite::create("background.png");
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setAnchorPoint(ccp(0,0));
		this->addChild(background);


		float yPos= 0.88;

		float startX = CCEGLView::sharedOpenGLView()->getFrameSize().width / 6;

		CCSprite* golden = CCSprite::create("throne.png",CCRect(202,405,200,180));
		golden->setAnchorPoint(ccp(0,0));
		golden->setScale(0.3);
		golden->setPosition(ccp(startX + CCEGLView::sharedOpenGLView()->getFrameSize().width,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * yPos));
		titles[0] = golden;


		yPos -= 0.07;
		CCSprite* silver = CCSprite::create("throne.png",CCRect(0,405,200,180));
		silver->setAnchorPoint(ccp(0,0));
		silver->setScale(0.3);
		silver->setPosition(ccp(startX + CCEGLView::sharedOpenGLView()->getFrameSize().width,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * yPos));
		titles[1] = silver;


		yPos -= 0.07;
		CCSprite* brozen = CCSprite::create("throne.png",CCRect(414,405,200,180));
		brozen->setAnchorPoint(ccp(0,0));
		brozen->setScale(0.3);
		brozen->setPosition(ccp(startX + CCEGLView::sharedOpenGLView()->getFrameSize().width,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * yPos));
		titles[2] = brozen;


		this->addChild(golden);
		this->addChild(silver);
		this->addChild(brozen);

		yPos -= 0.07;
		for(int i = 4;i <= 10;i++)
		{
			char buffer[8];
			sprintf(buffer,"%d.",i);
			CCLabelTTF* label = CCLabelTTF::create(buffer,"80 Decibels",50);
			label->setAnchorPoint(ccp(0,0));
			label->setPosition(ccp(startX + CCEGLView::sharedOpenGLView()->getFrameSize().width,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * yPos));
			yPos -= 0.07;
			this->addChild(label);
			titles[i - 1] = label;

		}

		int scores[10];
		Score::getInstance()->getScores(scores);

		yPos= 0.88;
		for(int i = 0;i < Score::SCORE_LEN;i++)
		{
			char buffer[16];
			if(scores[i] == -1)
			{
				sprintf(buffer,"No Rec");
			}
			else
			{
				sprintf(buffer,"%d",scores[i]);
			}
			CCLabelTTF* label = CCLabelTTF::create(buffer,"80 Decibels",50);
			label->setAnchorPoint(ccp(0,0));
			label->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width / 3.2 +
								   CCEGLView::sharedOpenGLView()->getFrameSize().width,
						        CCEGLView::sharedOpenGLView()->getFrameSize().height * yPos));
			yPos -= 0.07;
			this->addChild(label);

			items[i] = label;
		}

		for(int i = 0;i < Score::SCORE_LEN;i++)
		{
			CCMoveBy* moveBy = CCMoveBy::create(0.5,ccp(
								-CCEGLView::sharedOpenGLView()->getFrameSize().width,0));
			CCMoveBy* sleep = CCMoveBy::create(0.1 * i,ccp(0,0));
			CCMoveBy* moveBy2 = CCMoveBy::create(0.5,ccp(
								-CCEGLView::sharedOpenGLView()->getFrameSize().width,0));
			CCMoveBy* sleep2 = CCMoveBy::create(0.1 * i,ccp(0,0));

			titles[i]->runAction(CCSequence::create(sleep,moveBy,NULL));
			items[i]->runAction(CCSequence::create(sleep2,moveBy2,NULL));

		}

        bRet = true;
    } while (0);

    return bRet;
}
