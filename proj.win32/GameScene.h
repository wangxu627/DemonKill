/* ===================================================== */
//	
//    ®ç®Ä®Ä®Ä®Ä®Ä®é               ®ç®Ä®Ä®Ä®Ä®Ä®Ä®é
// ®ç®è             ®ê®é         ®ç®è             ®ê®é 
// ®è                 ®ê®é     ®ç®è                 ®Ä 
// ®Ü      ®ç®Ä®é       ®ê®é®ç®è       ®ç®Ä®é       ®Ä
// ®é     ®ç®è  ®ê®é               ®ç®é   ®ê®é   ®ç 
// ®ê®Ä®Ä®è   ®ç®è                   ®ê®é   ®ê®Ä®è
//          ®Ä   °Ò               °Ò    ®Ä 
//          ®Ä   °®       ®{       °®   ®Ä 
//           ®ê®é       ®^®m®a       ®ç®è 
//             ®ê®Ä®|®z®z   ®z®z®|®Ä®è 
//               ®ç®è       ®ê®é  
//               ®Ä           ®Ä  
//             ®ç®è®ä       ®Ü®ê®é
//           ®ä®z®é®ç®|®|®|®é®ç®z®Ü
//            
//                 Author : Wangxu
//            Create Date :				
//            Description : 
//
/* ===================================================== */	

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <vector>

class GameScene : public cocos2d::CCLayer,public cocos2d::CCTouchDelegate
{
	typedef unsigned char tGemType;enum
	{
		kBoardTypeGem0 = 0,
		kBoardTypeGem1,
		kBoardTypeGem2,
		kBoardTypeGem3,
		kBoardTypeGem4,
		kBoardTypePup0,

		kGemTypeCount,
	};

	struct tGemInfo
	{
		tGemType _gemType;
		cocos2d::CCSprite* _gemSprite;
		float _yPos;
		float _ySpeed;
		int _id;

		tGemInfo(){}
		tGemInfo(tGemType gemType,cocos2d::CCSprite* gemSprite,
				float yPos,float ySpeed,int id)
				: _gemType(gemType)
				, _gemSprite(gemSprite)
				,_yPos(yPos)
				,_ySpeed(ySpeed)
				,_id(id)
		{}
	};

	typedef unsigned char tLayer;enum
	{
		tBackgroundLayer = -2,
		tShimmerLayer = -1,
		tGameLayer = 0,
		tParticleLayer = 1,
		tEffectsLayer = 2,
		tHintLayer = 3,
	};

public:
	virtual ~GameScene();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(GameScene);

private:
	void playCallback(cocos2d::CCObject* sender);
	void aboutCallback(cocos2d::CCObject* sender);
	void OnUpdate(float dt);
	void setupBoard();
	void processClick(const cocos2d::CCPoint& pt);
	void removeMarkedGems();
	int findMove();
	void createRandomMove();

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	//virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

private:
	//std::vector<tGem> mGemContainer;
	//cocos2d::CCLayer* mFallingLayer;
	//cocos2d::CCLayer* mBackLayer;
	//cocos2d::CCLayer* mMenuLayer;
	//cocos2d::CCParticleSystem* mStarParticles;

	char* mBoard;
	char* mNumGemsInColumn;
	char* mTimeSinceAddInColumn;
	std::vector<tGemInfo>* mFallingGems;
	bool mBoardChangedSinceEvaluation;
	char mPossibleMove;
	cocos2d::CCSprite** mBoardSprites;
	bool mIsGameOver;
	bool mIsDisplayingHint;

	cocos2d::CCLayer* mShimmerLayer;
	cocos2d::CCLayer* mParticleLayer;
	cocos2d::CCLayer* mGameLayer;
	cocos2d::CCLayer* mHintLayer;
	cocos2d::CCLayer* mEffectsLayer;


	static const int kTimeBetweenGemAdds = 8;
	static const int kMaxTimeBetweenConsecutiveMoves = 1000;
	static const int kBoardWidth = 8;
	static const int kBoardHeight = 10;
	static const int kNumTotalGems = kBoardWidth * kBoardHeight;
	static const int kGemSize = 60;

};

#endif
