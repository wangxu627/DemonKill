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

#include "GameScene.h"

using namespace cocos2d;
using namespace std;

static int g_id = 0;

GameScene::~GameScene()
{

}

CCScene* GameScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameScene *layer = GameScene::create();
        CC_BREAK_IF(! layer);

		scene->addChild(layer);
        // add layer as a child to scene
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

		CCSprite* background = CCSprite::create("background800.png");
		background->setAnchorPoint(ccp(0,0));
		this->addChild(background,tBackgroundLayer);

		mShimmerLayer = CCLayer::create();
		this->addChild(mShimmerLayer,tShimmerLayer);

		mGameLayer = CCLayer::create();
		this->addChild(mGameLayer,tGameLayer);

		mParticleLayer = CCLayer::create();
		this->addChild(mParticleLayer,tParticleLayer);

		mEffectsLayer = CCLayer::create();
		this->addChild(mEffectsLayer,tEffectsLayer);

		mHintLayer = CCLayer::create();
		this->addChild(mHintLayer,tHintLayer);


		setupBoard();

		mIsGameOver = false;
		mIsDisplayingHint = false;

		setTouchEnabled(true);

		schedule(schedule_selector(GameScene::OnUpdate));

        bRet = true;
    } while (0);

    return bRet;
}

void GameScene::setupBoard()
{
	mBoard = new char[kNumTotalGems];
	for (int i = 0; i < kNumTotalGems; i++)
	{
		mBoard[i] = -1;
	}
	mBoardSprites = new CCSprite*[kNumTotalGems];

	mNumGemsInColumn = new char[kBoardWidth];
	mTimeSinceAddInColumn = new char[kBoardWidth];

	for (int x = 0; x < kBoardWidth; x++)
	{
		mNumGemsInColumn[x] = 0;
		mTimeSinceAddInColumn[x] = 0;
	}

	// Setup falling pieces
	mFallingGems = new vector<tGemInfo>[kBoardWidth];
	for (int x = 0; x < kBoardWidth; x++)
	{
		mFallingGems[x] = vector<tGemInfo>(0);
	}

	mBoardChangedSinceEvaluation = true;
	mPossibleMove = -1;
}

void GameScene::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCTouch* tch = (CCTouch*)(pTouches->anyObject());
	CCPoint pt = tch->getLocation();
	processClick(pt);

}

void GameScene::OnUpdate( float dt )
{
	if (!mIsGameOver)
	{
		removeMarkedGems();

		for(int x = 0;x < kBoardWidth;x++)
		{
			if(mNumGemsInColumn[x] + mFallingGems[x].size() < kBoardHeight &&
				mTimeSinceAddInColumn[x] >= kTimeBetweenGemAdds)
			{
				// A gem should be added to this colum!
				tGemType gemType = rand() % kBoardTypePup0;
				char gemName[32];
				sprintf(gemName,"crystals/%d.png",gemType);
				CCSprite* gemSprite = CCSprite::createWithSpriteFrameName(gemName);
				gemSprite->setAnchorPoint(ccp(0,0));

				tGemInfo info(gemType,gemSprite,kBoardHeight,0,g_id++);
				mFallingGems[x].push_back(info);

				mGameLayer->addChild(gemSprite);

				mTimeSinceAddInColumn[x] = 0;
			}
			mTimeSinceAddInColumn[x]++;
		}

		bool gemLanded = false;
		for(int x = 0;x < kBoardWidth;x++)
		{
			vector<tGemInfo>& rColumn = mFallingGems[x];
			//int numFallingGems = mFallingGems[x].size();

			//for(int i = mFallingGems[x].size() - 1;i >= 0;i--)
			int k = rColumn.size() - 1;
			for(int i = rColumn.size() - 1;i >=0 ;i--)
			{
				tGemInfo* gem = &rColumn[i];

				gem->_ySpeed += 0.06;
				gem->_ySpeed *= 0.99;
				gem->_yPos -= gem->_ySpeed;

				if(gem->_yPos <= mNumGemsInColumn[x])
				{
					if(!gemLanded)
					{
						gemLanded = true;
					}

					int y = mNumGemsInColumn[x];
					if(mBoard[x + y * kBoardWidth] != -1)
					{
						CCLOG("ERROR");
					}

					mBoard[x + y * kBoardWidth] = gem->_gemType;
					mBoardSprites[x + y * kBoardWidth] = gem->_gemSprite;

					// Update fixed position
					gem->_gemSprite->setPosition(ccp(x * kGemSize, y * kGemSize));
					CCLOG("%4d ---> %5d : (%5d,%5d)",gem->_id,i, x * kGemSize,y * kGemSize);
					mNumGemsInColumn[x]++;

					rColumn.erase(rColumn.begin() + i);

					mBoardChangedSinceEvaluation = true;

				}
				else
				{
					gem->_gemSprite->setPosition(ccp(x * kGemSize, gem->_yPos * kGemSize));

				//	CCLOG("%f ----> %f",gem->_yPos,gem->_yPos * kGemSize);
				}	
			}
		}

		// Check if there are possible moves and no gems falling
		bool isFallingGems = false;
		for (int x = 0; x < kBoardWidth; x++)
		{
			if (mNumGemsInColumn[x] != kBoardHeight)
			{
				isFallingGems = true;
				break;
			}
		}

		if (!isFallingGems)
		{
			int possibleMove = findMove();
			if (possibleMove == -1)
			{
				// Create a possible move
				createRandomMove();
			}
		}
	}
}

int GameScene::findMove()
{

	return 0;
}

void GameScene::createRandomMove()
{

}

void GameScene::processClick( const cocos2d::CCPoint& pt )
{

}

void GameScene::removeMarkedGems()
{

}




