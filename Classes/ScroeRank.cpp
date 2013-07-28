/* ===================================================== */
//	
//    ¨¨€¨€¨€¨€¨€¨Ž               ¨¨€¨€¨€¨€¨€¨€¨Ž
// ¨¨             ¨¨Ž         ¨¨             ¨¨Ž 
// ¨                 ¨¨Ž     ¨¨                 ¨€ 
// ¨†      ¨¨€¨Ž       ¨¨Ž¨¨       ¨¨€¨Ž       ¨€
// ¨Ž     ¨¨  ¨¨Ž               ¨¨Ž   ¨¨Ž   ¨ 
// ¨¨€¨€¨   ¨¨                   ¨¨Ž   ¨¨€¨
//          ¨€   ¡ñ               ¡ñ    ¨€ 
//          ¨€   ¡¨       ¨{       ¡¨   ¨€ 
//           ¨¨Ž       ¨^¨m¨a       ¨¨ 
//             ¨¨€¨|¨z¨z   ¨z¨z¨|¨€¨ 
//               ¨¨       ¨¨Ž  
//               ¨€           ¨€  
//             ¨¨¨Š       ¨†¨¨Ž
//           ¨Š¨z¨Ž¨¨|¨|¨|¨Ž¨¨z¨†
//            
//                 Author : Wangxu
//            Create Date :				
//            Description : 
//
/* ===================================================== */	

#include "ScoreRank.h"

using namespace cocos2d;

ScoreRank* ScoreRank::create( const char* aBGName,const char* aFontName )
{
	ScoreRank* ret = new ScoreRank(aBGName,aFontName);
	ret->autorelease();
	return ret;
}

ScoreRank::ScoreRank( const char* aBGName,const char* aFontName )
{
	mBG				= CCSprite::create(aBGName);
	mBG->setAnchorPoint(ccp(0,0));
	float scaleRate = CCEGLView::sharedOpenGLView()->getFrameSize().width / 
								mBG->getContentSize().width;
	mBG->setScale(scaleRate);
	this->setContentSize(CCSize(mBG->getContentSize().width * scaleRate,
							    mBG->getContentSize().height * scaleRate));

	mLBScoreTitle	= CCLabelTTF::create("Scores",aFontName,FONT_SIZE);
	mLBScoreTitle->setPosition(ccp(this->getContentSize().width / 2,
								   this->getContentSize().height  * 0.85));

	mLBScore		= CCLabelTTF::create("0",aFontName,FONT_SIZE);
	mLBScore->setPosition(ccp(this->getContentSize().width / 2,
						      this->getContentSize().height  * 0.72));

	mLBTotalTitle	= CCLabelTTF::create("Total",aFontName,FONT_SIZE);
	mLBTotalTitle->setPosition(ccp(this->getContentSize().width / 2,
						           this->getContentSize().height  * 0.5));

	mLBTotal		= CCLabelTTF::create("0",aFontName,FONT_SIZE);
	mLBTotal->setPosition(ccp(this->getContentSize().width / 2,
						      this->getContentSize().height  * 0.37));

	this->addChild(mBG);
	this->addChild(mLBScoreTitle);
	this->addChild(mLBScore);
	this->addChild(mLBTotalTitle);
	this->addChild(mLBTotal);
}

void ScoreRank::setScore( int aScore )
{
	char buf[16];
	itoa(aScore,buf,10);
	mLBScore->setString(buf);
}

void ScoreRank::setTotal( int aTotal )
{
	char buf[16];
	itoa(aTotal,buf,10);
	mLBTotal->setString(buf);
}
