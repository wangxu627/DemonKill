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
	mLBScoreTitle	= CCLabelTTF::create("Scores",aFontName,32);
	mLBScore		= CCLabelTTF::create("0",aFontName,32);
	mLBTotalTitle	= CCLabelTTF::create("Total",aFontName,32);
	mLBTotal		= CCLabelTTF::create("0",aFontName,32);

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
