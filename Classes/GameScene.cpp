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
//using namespace cocos2d::extension;

const float GameScene::kGameOverGemSpeed = 0.1;
const float GameScene::kGameOverGemAcceleration = 0.005;

#define RAND_0_TO_1 ((rand() % RAND_MAX) / RAND_MAX)

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

		CCSprite* background = CCSprite::create("background.png");
		background->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / background->getContentSize().width);
		background->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height / background->getContentSize().height);
		background->setAnchorPoint(ccp(0,0));
		this->addChild(background,tBackgroundLayer);
		
		CCSprite* header = CCSprite::create("header.png");
		header->setAnchorPoint(ccp(0,1));
		header->setPosition(ccp(0,CCEGLView::sharedOpenGLView()->getFrameSize().height));
		header->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / header->getContentSize().width);
		header->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.17 / header->getContentSize().height);
		this->addChild(header,tBackgroundLayer + 1);

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
	mBoard = new tGemType[kNumTotalGems];
	for (char i = 0; i < kNumTotalGems; i++)
	{
		mBoard[i] = -1;
	}
	mBoardSprites = new CCSprite*[kNumTotalGems];

	mNumGemsInColumn = new char[kBoardWidth];
	mTimeSinceAddInColumn = new unsigned char[kBoardWidth];

	for (char x = 0; x < kBoardWidth; x++)
	{
		mNumGemsInColumn[x] = 0;
		mTimeSinceAddInColumn[x] = 0;
	}

	// Setup falling pieces
	mFallingGems = new vector<tGemInfo>[kBoardWidth];
	for (char x = 0; x < kBoardWidth; x++)
	{
		mFallingGems[x] = vector<tGemInfo>(0);
	}

	CCSprite* timerSprt = CCSprite::create("timer.png");
	ccBlendFunc bf;
	bf.src = GL_SRC_ALPHA;
	bf.dst = GL_ONE;
	timerSprt->setBlendFunc(bf);
	mTimer = CCProgressTimer::create(timerSprt);
	mTimer->setPercentage(100);
    mTimer->setType(kCCProgressTimerTypeBar);
    mTimer->setMidpoint(ccp(0, 0.5));
    mTimer->setBarChangeRate(ccp(1, 0));
	mTimer->setAnchorPoint(ccp(0.5,0));
	mTimer->setScaleX(CCEGLView::sharedOpenGLView()->getFrameSize().width / 
		mTimer->getContentSize().width * 0.93);
	mTimer->setScaleY(CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.05 / 
		mTimer->getContentSize().height);
	mTimer->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width / 2,
		CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.922 - mTimer->getContentSize().height
		));
    mGameLayer->addChild(mTimer);

	mScoreLabel = CCLabelBMFont::create("0", "scorefont.fnt");
	mScoreLabel->setAnchorPoint(ccp(1,0));
	mScoreLabel->setPosition(ccp(CCEGLView::sharedOpenGLView()->getFrameSize().width * 0.99,
								 CCEGLView::sharedOpenGLView()->getFrameSize().height * 0.89));
	addChild(mScoreLabel);

	mLastMoveTime = clock();
	mStartTime = mLastMoveTime + kIntroTime;
    mNumConsecutiveGems = 0;
    mIsPowerPlay = false;
    mEndTimerStarted = false;
    mScore = 0;
	mLastScore = 0;
	mBoardChangedSinceEvaluation = true;
	mPossibleMove = -1;
	mIsDisplayingHint = false;
}

void GameScene::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
//	CCTouch* tch = (CCTouch*)(pTouches->anyObject());
//	CCPoint pt = tch->getLocation();
//	processClick(pt);
}

void GameScene::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
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
			char output[64];
			sprintf(output,
					"THE IN COLUMN is %d and %d and %d\n",
					mNumGemsInColumn[x],
					mFallingGems[x].size(),
					mTimeSinceAddInColumn[x]);
			::OutputDebugStringA(output);
			//CCLOG("ENTER OUTER ==> %d",mNumGemsInColumn[x]);
			//CCLOG("ENTER OUTER2 ==> %d",mFallingGems[x].size());

			if(mNumGemsInColumn[x] + mFallingGems[x].size() < kBoardHeight &&
				mTimeSinceAddInColumn[x] >= kTimeBetweenGemAdds)
			{
				//CCLOG("ENTER INNER");
				//::OutputDebugStringA("+++++++++++++++++++++++++++ENTER INNER\n");

				// A gem should be added to this colum!
				tGemType gemType = rand() % (kGemTypeCount - 1);
				char gemName[32];
				sprintf(gemName,"crystals/%d.png",gemType);
				//sprintf(gemName,"crystals/1.png");
				CCSprite* gemSprite = CCSprite::createWithSpriteFrameName(gemName);
				gemSprite->setAnchorPoint(ccp(0,0));
				gemSprite->setScale(0.75);

				tGemInfo info(gemType,gemSprite,kBoardHeight,0);
				mFallingGems[x].push_back(info);

				mGameLayer->addChild(gemSprite);

				mTimeSinceAddInColumn[x] = 0;
			}
			if(++mTimeSinceAddInColumn[x] == 255)
			{
				mTimeSinceAddInColumn[x] = kTimeBetweenGemAdds;
			}
		}

		// Move falling gems
		bool gemLanded = false;
		for(int x = 0;x < kBoardWidth;x++)
		{
			vector<tGemInfo>& rColumn = mFallingGems[x];
			int numFallingGems = mFallingGems[x].size();

			//for(int i = numFallingGems - 1;i >= 0;i--)
			for(int i = 0;i < mFallingGems[x].size();i++)
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
						CCLog("Warning!");
					}

					mBoard[x + y * kBoardWidth] = gem->_gemType;
					mBoardSprites[x + y * kBoardWidth] = gem->_gemSprite;
					gem->_gemSprite->setPosition(ccp(x * kGemSize, y * kGemSize));
					mNumGemsInColumn[x]++;

					rColumn.erase(rColumn.begin() + i);

					mBoardChangedSinceEvaluation = true;
				}
				else
				{
					gem->_gemSprite->setPosition(ccp(x * kGemSize,gem->_yPos * kGemSize));
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

		// Update timer
		clock_t currentTime = clock();
		float elapsedTime = (currentTime - mStartTime) / (float)kTotalGameTime;
		float timeLeft = (1.0 - elapsedTime) * 100.0;
		if (timeLeft < 0)
		{
			timeLeft = 0;
		}
		if (timeLeft > 99.9)
		{
			timeLeft = 99.9;
		}
		mTimer->setPercentage(timeLeft);

		// Update consecutive moves / powerplay
		if (currentTime - mLastMoveTime > kMaxTimeBetweenConsecutiveMoves)
		{
			mNumConsecutiveGems = 0;
		}
		updatePowerPlay();
		
		// Update sparkles
		updateSparkle();

			// Check if timer sound should be played
		if (timeLeft < 6.6 && !mEndTimerStarted)
		{
		//	gAudioEngine.playEffect("sounds/timer.wav");
			mEndTimerStarted = true;
		}

		// Check for game over
		if (timeLeft <= 0)
		{
			createGameOver();
			//this->rootNode.animationManager.runAnimationsForSequenceNamed("Outro");
			//CCAnimationManager
			mIsGameOver = true;
			//gAudioEngine.stopAllEffects();
			//cc.log("stopAllEffects not working!");
			//gAudioEngine.playEffect("sounds/endgame.wav");
			mLastScore = mScore;
		}
		else if (currentTime - mLastMoveTime > kDelayBeforeHint && !mIsDisplayingHint)
		{
			displayHint();
		}
	}
	else
	{
		// It's game over
		updateGameOver();
	}
}

int GameScene::findMove()
{
	if (!mBoardChangedSinceEvaluation)
	{
		return mPossibleMove;
	}

	// Iterate through all places on the board
	for (int y = 0; y < kBoardHeight; y++)
	{
		for (int x = 0; x < kBoardWidth; x++)
		{
			int idx = x + y * kBoardWidth;
			tGemType gemType = mBoard[idx];

			// Make sure that it is a gem
			if (gemType < 0 || gemType >= kBoardTypePup0)
			{
				continue;
			}

			// Check surrounding tiles
			int numSimilar = 0;

			if (getGemType(x-1, y) == gemType)
			{
				numSimilar++;
			}
			if (getGemType(x+1, y) == gemType)
			{
				numSimilar++;
			}
			if (getGemType(x, y-1) == gemType)
			{
				numSimilar++;
			}
			if (getGemType(x, y+1) == gemType)
			{
				numSimilar++;
			}

			if (numSimilar >= 2)
			{
				mPossibleMove = idx;
				return idx;
			}
		}
	}
	mBoardChangedSinceEvaluation = false;
	mPossibleMove = -1;
	return -1;
}

void GameScene::processClick( const CCPoint& pt )
{
	CCPoint tmp = this->mGameLayer->getPosition();
	CCPoint loc = ccp(pt.x - this->mGameLayer->getPosition().x, pt.y - this->mGameLayer->getPosition().y);

	int x = (int)(loc.x / kGemSize);
	int y = (int)(loc.y / kGemSize);

	if (!mIsGameOver)
	{
		mHintLayer->removeAllChildrenWithCleanup(true);
		mIsDisplayingHint = false;

		if (activatePowerUp(x,y) || removeConnectedGems(x,y))
		{
			// Player did a valid move
			//var sound = gNumConsecutiveGems;
			//if (sound > 4) sound = 4;
			//gAudioEngine.playEffect("sounds/gem-"+sound+".wav");

			mNumConsecutiveGems++;
		}
		else
		{
			mNumConsecutiveGems = 0;
		}

		mLastMoveTime = clock();
		//gLastMoveTime = Date.now();
		
	}
}

void GameScene::removeMarkedGems()
{
	// Iterate through the board
	for (int x = 0; x < kBoardWidth; x++)
	{
		for (int y = 0; y < kBoardHeight; y++)
		{
			int i = x + y * kBoardWidth;

			if (mBoard[i] < -1)
			{
				// Increase the count for negative crystal types
				mBoard[i]++;
				if (mBoard[i] == -1)
				{
					mNumGemsInColumn[x]--;
					mBoardChangedSinceEvaluation = true;

					// Transform any gem above this to a falling gem
					for (int yAbove = y+1; yAbove < kBoardHeight; yAbove++)
					{
						int idxAbove = x + yAbove*kBoardWidth;

						if (mBoard[idxAbove] < -1)
						{
							mNumGemsInColumn[x]--;
							mBoard[idxAbove] = -1;
						}
						if (mBoard[idxAbove] == -1) continue;

						// The gem is not connected, make it into a falling gem
						tGemType gemType = mBoard[idxAbove];
						CCSprite* gemSprite = mBoardSprites[idxAbove];

						//tGemInfo gem = {gemType: gemType, sprite: gemSprite, yPos: yAbove, ySpeed: 0};
						tGemInfo gem(gemType,gemSprite,yAbove,0);

						mFallingGems[x].push_back(gem);

						// Remove from board
						mBoard[idxAbove] = -1;
						mBoardSprites[idxAbove] = NULL;

						mNumGemsInColumn[x]--;
					}
				}
			}
		}
	}
}

void GameScene::createRandomMove()
{
		// Find a random place in the lower part of the board
	int x = rand() % kBoardWidth;
	int y = rand() % kBoardHeight;

	// Make sure it is a gem that we found
	tGemType gemType = mBoard[x + y * kBoardWidth];
	if (gemType == -1 || gemType >= kBoardTypePup0)
	{
		return;
	}

	// Change the color of two surrounding gems
	setGemType(x+1, y, gemType);
	setGemType(x, y+1, gemType);

	mBoardChangedSinceEvaluation = true;
}

GameScene::tGemType GameScene::getGemType(int x, int y)
{
	if (x < 0 || x >= kBoardWidth)
	{
		return -1;
	}
	if (y < 0 || y >= kBoardHeight)
	{
		return -1;
	}

	return mBoard[x + y * kBoardWidth];
}

void GameScene::setGemType(int x,int y,tGemType newType)
{
	// Check bounds
	if (x < 0 || x >= kBoardWidth)
	{
		return;
	}
	if (y < 0 || y >= kBoardHeight)
	{
		return;
	}

	// Get the type of the gem
	int idx = x + y * kBoardWidth;
	tGemType gemType = mBoard[idx];

	// Make sure that it is a gem
	if (gemType < 0 || gemType >= kBoardTypePup0)
	{
		return;
	}

	mBoard[idx] = newType;

	// Remove old gem and insert a new one
	mGameLayer->removeChild(mBoardSprites[idx], true);

	char gemName[32];
	sprintf(gemName,"crystals/%d.png",gemType);
	CCSprite* gemSprite = CCSprite::createWithSpriteFrameName(gemName);
	gemSprite->setPosition(ccp(x * kGemSize, y * kGemSize));
	gemSprite->setAnchorPoint(ccp(0,0));

	mGameLayer->addChild(gemSprite);
	mBoardSprites[idx] = gemSprite;

	mBoardChangedSinceEvaluation = true;
}

void GameScene::updatePowerPlay()
{
	bool powerPlay = (mNumConsecutiveGems >= 5);
	if (powerPlay == mIsPowerPlay)
	{
		return;
	}

	if (powerPlay)
	{
		// Start power-play
		mPowerPlayParticles = CCParticleSystemQuad::create("particles/power-play.plist");
		mPowerPlayParticles->setAutoRemoveOnFinish(true);
		mParticleLayer->addChild(mPowerPlayParticles);

		CCSize contentSize = mGameLayer->getContentSize();
		mPowerPlayLayer = CCLayerColor::create(ccc4(85, 0, 70, 0), 
						contentSize.width, contentSize.height);

		CCSequence* action = CCSequence::createWithTwoActions(CCFadeIn::create(0.25), CCFadeOut::create(0.25));
		mPowerPlayLayer->runAction(CCRepeatForever::create(action));
		//mPowerPlayLayer->setBlendFunc(SRC_ALPHA, gl.ONE);
		ccBlendFunc bf;
		bf.src = GL_SRC_ALPHA;
		bf.dst = GL_ONE;
		mPowerPlayLayer->setBlendFunc(bf);
		mEffectsLayer->addChild(mPowerPlayLayer);
	}
	else
	{
		// Stop power-play
		if (mPowerPlayParticles)
		{
			mPowerPlayParticles->stopSystem();
			mPowerPlayParticles = NULL;
			/*
			if( 'opengl' in sys.capabilities ) {
				gPowerPlayLayer.stopAllActions();
				gPowerPlayLayer.runAction(cc.Sequence.create(cc.FadeOut.create(0.5), cc.CallFunc.create(onRemoveFromParent, this)));
			}
			*/
		}
		
	}

	mIsPowerPlay = powerPlay;
}

bool GameScene::activatePowerUp(int x, int y)
{
	// Check for bounds
	if (x < 0 || x >= kBoardWidth) return false;
	if (y < 0 || y >= kBoardHeight) return false;

	bool removedGems = false;

	int idx = x + y * kBoardWidth;
	if (mBoard[idx] == kBoardTypePup0)
	{
		// Activate bomb
		//gAudioEngine.playEffect("sounds/powerup.wav");

		removedGems = true;

		addScore(2000);

		mBoard[idx] = -kNumRemovalFrames;
		mGameLayer->removeChild(mBoardSprites[idx], true);
		mBoardSprites[idx] = NULL;

		// Remove a horizontal line
		int idxRemove;
		for (int xRemove = 0; xRemove < kBoardWidth; xRemove++)
		{
			idxRemove = xRemove + y * kBoardWidth;
			if (mBoard[idxRemove] >= 0 && mBoard[idxRemove] < 5)
			{
				mBoard[idxRemove] = -kNumRemovalFrames;
				mGameLayer->removeChild(mBoardSprites[idxRemove], true);
				mBoardSprites[idxRemove] = NULL;
			}
		}

		// Remove a vertical line
		for (int yRemove = 0; yRemove < kBoardHeight; yRemove++)
		{
			idxRemove = x + yRemove * kBoardWidth;
			if (mBoard[idxRemove] >= 0 && mBoard[idxRemove] < 5)
			{
				mBoard[idxRemove] = -kNumRemovalFrames;
				mGameLayer->removeChild(mBoardSprites[idxRemove], true);
				mBoardSprites[idxRemove] = NULL;
			}
		}

		// Add particle effects
		CCParticleSystem* hp = CCParticleSystemQuad::create("particles/taken-hrow.plist");
		hp->setPosition(ccp(kBoardWidth / 2 * kGemSize + kGemSize / 2,
							y * kGemSize + kGemSize / 2));
		hp->setAutoRemoveOnFinish(true);
		mParticleLayer->addChild(hp);

		CCParticleSystem* vp = CCParticleSystemQuad::create("particles/taken-vrow.plist");
		vp->setPosition(ccp(x * kGemSize + kGemSize / 2,
			                kBoardHeight / 2 * kGemSize + kGemSize / 2));
		vp->setAutoRemoveOnFinish(true);
		mParticleLayer->addChild(vp);
		

		// Add explo anim
		CCPoint center = ccp(x * kGemSize+kGemSize / 2, y * kGemSize+kGemSize / 2);

		// Horizontal
		CCSprite* sprtH0 = CCSprite::createWithSpriteFrameName("crystals/bomb-explo.png");
		//sprtH0->setBlendFunc(gl.SRC_ALPHA, gl.ONE);
		ccBlendFunc bf;
		bf.src = GL_SRC_ALPHA;
		bf.dst = GL_ONE;
		sprtH0->setBlendFunc(bf);
		sprtH0->setScaleX(5);
		sprtH0->setPosition(center);
		sprtH0->runAction(CCScaleTo::create(0.5, 30, 1));
		sprtH0->runAction(CCSequence::create(CCFadeOut::create(0.5), 
											 CCCallFuncN::create(this,callfuncN_selector(GameScene::onRemoveFromParent)),
											 NULL));
		mEffectsLayer->addChild(sprtH0);

		// Vertical
		CCSprite* sprtV0 = CCSprite::createWithSpriteFrameName("crystals/bomb-explo.png");
		//sprtV0.setBlendFunc(gl.SRC_ALPHA, gl.ONE);
		sprtV0->setBlendFunc(bf);
		sprtV0->setPosition(center);
		sprtV0->setScaleY(5);
		sprtV0->runAction(CCScaleTo::create(0.5, 1, 30));
		sprtV0->runAction(CCSequence::create(CCFadeOut::create(0.5), 
											 CCCallFuncN::create(this,callfuncN_selector(GameScene::onRemoveFromParent)),
											 NULL));
		mEffectsLayer->addChild(sprtV0);

		// Horizontal
		CCSprite* sprtH1 = CCSprite::createWithSpriteFrameName("crystals/bomb-explo-inner.png");
		//sprtH1.setBlendFunc(gl.SRC_ALPHA, gl.ONE);
	
		sprtH1->setBlendFunc(bf);
		sprtH1->setPosition(center);
		sprtH1->setScaleX(0.5);
		sprtH1->runAction(CCScaleTo::create(0.5, 8, 1));
		sprtH1->runAction(CCSequence::create(CCFadeOut::create(0.5), 
											 CCCallFuncN::create(this,callfuncN_selector(GameScene::onRemoveFromParent)),
											 NULL));
		mEffectsLayer->addChild(sprtH1);

		// Vertical
		CCSprite* sprtV1 = CCSprite::createWithSpriteFrameName("crystals/bomb-explo-inner.png");
		sprtV1->setRotation(90);
		//sprtV1->setBlendFunc(gl.SRC_ALPHA, gl.ONE);
	
		sprtV1->setBlendFunc(bf);
		sprtV1->setPosition(center);
		sprtV1->setScaleY(0.5);
		sprtV1->runAction(CCScaleTo::create(0.5, 8, 1));
		sprtV1->runAction(CCSequence::create(CCFadeOut::create(0.5), 
											 CCCallFuncN::create(this,callfuncN_selector(GameScene::onRemoveFromParent)),
											 NULL));
		mEffectsLayer->addChild(sprtV1);
	}

	return removedGems;
}

bool GameScene::removeConnectedGems(int x, int y)
{
	// Check for bounds
	if (x < 0 || x >= kBoardWidth) return false;
	if (y < 0 || y >= kBoardHeight) return false;

	vector<tGemType> connected = findConnectedGems(x,y);
	bool removedGems = false;

	if (connected.size() >= 3)
	{
		mBoardChangedSinceEvaluation = true;
		removedGems = true;

		addScore(100 * connected.size());

		int idxPup = -1;
		int pupX;
		int pupY;
		if (connected.size() >= 6)
		{
			// Add power-up
			idxPup = connected[rand() % connected.size()];
			pupX = idxPup % kBoardWidth;
			pupY = (int)(idxPup / kBoardWidth);
		}

		for (int i = 0; i < connected.size(); i++)
		{
			tGemType idx = connected[i];
			int gemX = idx % kBoardWidth;
			int gemY = (int)(idx / kBoardWidth);

			mBoard[idx] = -kNumRemovalFrames;
			mGameLayer->removeChild(mBoardSprites[idx], true);
			mBoardSprites[idx] = NULL;

			// Add particle effect
			CCParticleSystem* particle = CCParticleSystemQuad::create("particles/taken-gem.plist");
			particle->setPosition(gemX * kGemSize + kGemSize / 2,
				                  gemY * kGemSize + kGemSize / 2);
			particle->setAutoRemoveOnFinish(true);
			mParticleLayer->addChild(particle);
		
			// Add power-up
			if (idx == idxPup)
			{
				mBoard[idx] = kBoardTypePup0;

				CCSprite* sprt = CCSprite::createWithSpriteFrameName("crystals/bomb.png");
				sprt->setScale(0.75);
				sprt->setPosition(ccp(gemX * kGemSize, gemY * kGemSize));
				sprt->setAnchorPoint(ccp(0,0));
				sprt->setOpacity(0);
				sprt->runAction(CCFadeIn::create(0.4));

				CCSprite* sprtGlow = CCSprite::createWithSpriteFrameName("crystals/bomb-hi.png");
				sprtGlow->setAnchorPoint(ccp(0,0));
				sprtGlow->setOpacity(0);
				sprtGlow->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCFadeIn::create(0.4),CCFadeOut::create(0.4))));
				sprt->addChild(sprtGlow);

				mBoardSprites[idx] = sprt;
				mGameLayer->addChild(sprt);
			}
			else if (idxPup != -1)
			{
				// Animate effect for power-up
				CCSprite* sprtLight = CCSprite::createWithSpriteFrameName("crystals/bomb-light.png");
				sprtLight->setPosition(ccp(gemX * kGemSize + kGemSize / 2, gemY * kGemSize + kGemSize / 2));
				sprtLight->setScale(0.75);
				//sprtLight.setBlendFunc(gl.SRC_ALPHA, gl.ONE);
				ccBlendFunc bf;
				bf.src = GL_SRC_ALPHA;
				bf.dst = GL_ONE;
				sprtLight->setBlendFunc(bf);
				mEffectsLayer->addChild(sprtLight);

				CCMoveTo* movAction = CCMoveTo::create(0.2, ccp(pupX * kGemSize + kGemSize / 2, pupY * kGemSize + kGemSize / 2));
				CCSequence* seqAction = CCSequence::createWithTwoActions(movAction, 
											CCCallFuncN::create(this,callfuncN_selector(GameScene::onRemoveFromParent)));

				sprtLight->runAction(seqAction);
			}
		}
	}
	else
	{
	//	gAudioEngine.playEffect("sounds/miss.wav");
	}

	mLastMoveTime = clock();
	//gLastMoveTime = Date.now();

	return removedGems;
}

vector<GameScene::tGemType> GameScene::findConnectedGems(int x, int y)
{
	vector<tGemType> connected;
	if (mBoard[x + y * kBoardWidth] <= -1) return connected;

	findConnectedGems_(x, y, connected, mBoard[x + y * kBoardWidth]);

	return connected;
}

void GameScene::findConnectedGems_(int x, int y, vector<tGemType>& arr, tGemType gemType)
{
	// Check for bounds
	if (x < 0 || x >= kBoardWidth) return;
	if (y < 0 || y >= kBoardHeight) return;

	int idx = x + y*kBoardWidth;

	// Make sure that the gems type match
	if (mBoard[idx] != gemType) return;


	// Check if idx is already visited
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i] == idx) return;
	}

	// Add idx to array
	arr.push_back(idx);

	// Visit neighbours
	findConnectedGems_(x+1, y, arr, gemType);
	findConnectedGems_(x-1, y, arr, gemType);
	findConnectedGems_(x, y+1, arr, gemType);
	findConnectedGems_(x, y-1, arr, gemType);
}


void GameScene::onRemoveFromParent(CCNode* node)
{
	node->getParent()->removeChild(node, true);
}

void GameScene::addScore(int score)
{
	if (mIsPowerPlay)
	{
		score *= 3;
	}
	mScore += score;
	char buf[16];
	sprintf(buf,"%d",mScore);
	mScoreLabel->setString(buf);
}

void GameScene::updateGameOver()
{
	vector<tGemInfoOver>::iterator iter = mGameOverGems.begin();
	while(iter != mGameOverGems.end())
	{
		iter->_xPos += iter->_xSpeed;
		iter->_yPos += iter->_ySpeed;
		iter->_ySpeed -= kGameOverGemAcceleration;

		iter->_gemSprite->setPosition(ccp(iter->_xPos * kGemSize, iter->_yPos * kGemSize));
		++iter;
	}
}

void GameScene::createGameOver()
{
	mGameOverGems.resize(0);

	for (int x = 0; x < kBoardWidth; x++)
	{
		vector<tGemInfo>& rColumn = mFallingGems[x];
		
		for (int i = 0; i < rColumn.size(); i++)
		{
			//tGemInfo gem = rColumn[i];
			tGemInfo* gem = &rColumn[i];

			float ySpeed = ((rand() % RAND_MAX) / (float)RAND_MAX * 2 - 1) * kGameOverGemSpeed;
			float xSpeed = ((rand() % RAND_MAX) / (float)RAND_MAX * 2 - 1) * kGameOverGemSpeed;

			tGemInfoOver gameOverGem(gem->_gemSprite, x, gem->_yPos, ySpeed, xSpeed);
			mGameOverGems.push_back(gameOverGem);
		}

		for (int y = 0; y < kBoardHeight; y++)
		{
			int i1 = x + y * kBoardWidth;
			if (mBoardSprites[i1])
			{
				float ySpeed1 = ((rand() % RAND_MAX) / (float)RAND_MAX * 2 - 1) * kGameOverGemSpeed;
				float xSpeed1 = ((rand() % RAND_MAX) / (float)RAND_MAX) * kGameOverGemSpeed;

				tGemInfoOver gameOverGem1(mBoardSprites[i1], x, y, ySpeed1, xSpeed1);
				mGameOverGems.push_back(gameOverGem1);
			}
		}
	}

	//mHintLayer->removeAllChildren(true);
	mHintLayer->removeAllChildrenWithCleanup(true);

	removeShimmer();
}

void GameScene::setupShimmer()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gamescene/shimmer.plist");

	for (int i = 0; i < 2; i++)
	{
		char name[64];
		sprintf(name,"gamescene/shimmer/bg-shimmer-%d.png",i);
		CCSprite* sprt = CCSprite::createWithSpriteFrameName(name);

		CCActionInterval* seqRot = NULL;
		CCActionInterval* seqMov = NULL;
		CCActionInterval* seqSca = NULL;
		
		float x;
		float y;
		float rot;

		for (int j = 0; j < 10; j++)
		{
			float time = RAND_0_TO_1 * 10 + 5;
			x = kBoardWidth * kGemSize/2;
			y = RAND_0_TO_1 * kBoardHeight*kGemSize;
			rot = RAND_0_TO_1 * 180 - 90;
			float scale = RAND_0_TO_1 * 3 + 3;

			CCEaseInOut* actionRot = CCEaseInOut::create(CCRotateTo::create(time, rot), 2);
			CCEaseInOut* actionMov = CCEaseInOut::create(CCMoveTo::create(time, ccp(x,y)), 2);
			CCScaleTo* actionSca = CCScaleTo::create(time, scale);

			if (!seqRot)
			{
				seqRot = actionRot;
				seqMov = actionMov;
				seqSca = actionSca;
			}
			else
			{
				seqRot = CCSequence::createWithTwoActions(seqRot, actionRot);
				seqMov = CCSequence::createWithTwoActions(seqMov, actionMov);
				seqSca = CCSequence::createWithTwoActions(seqSca, actionSca);
			}
		}

		x = kBoardWidth * kGemSize/2;
		y = RAND_0_TO_1 * kBoardHeight * kGemSize;
		rot = RAND_0_TO_1 * 180 - 90;

		sprt->setPosition(ccp(x,y));
		sprt->setRotation(rot);

		sprt->setPosition(ccp(kBoardWidth * kGemSize / 2, kBoardHeight * kGemSize / 2));
		ccBlendFunc bf;
		bf.src = GL_SRC_ALPHA;
		bf.dst = GL_ONE;
		sprt->setBlendFunc(bf);
		sprt->setScale(3);

		mShimmerLayer->addChild(sprt);
		sprt->setOpacity(0);
		sprt->runAction(CCRepeatForever::create(seqRot));
		sprt->runAction(CCRepeatForever::create(seqMov));
		sprt->runAction(CCRepeatForever::create(seqSca));

		sprt->runAction(CCFadeIn::create(2));
	}
}


void GameScene::removeShimmer()
{
	//CCArray* children = mShimmerLayer->getChildren();
	for (int i = 0; i < mShimmerLayer->getChildrenCount(); i++)
	{
		((CCNode*)(mShimmerLayer->getChildren()->objectAtIndex(i)))->runAction(CCFadeOut::create(1));
	}
	
}

void GameScene::displayHint()
{
	mIsDisplayingHint = true;

	int idx = findMove();
	int x = idx % kBoardWidth;
	int y = idx / kBoardWidth;

	vector<GameScene::tGemType> connected = findConnectedGems(x,y);

	for (int i = 0; i < connected.size(); i++)
	{
		idx = connected[i];
		x = idx % kBoardWidth;
		y = idx / kBoardWidth;

		CCFadeIn* actionFadeIn = CCFadeIn::create(0.5);
		CCFadeOut* actionFadeOut = CCFadeOut::create(0.5);
		CCSequence* actionSeq = CCSequence::createWithTwoActions(actionFadeIn, actionFadeOut);
		CCRepeatForever* action = CCRepeatForever::create(actionSeq);

		CCSprite* hintSprite = CCSprite::createWithSpriteFrameName("crystals/hint.png");
		hintSprite->setOpacity(0);
		hintSprite->setPosition(ccp(x * kGemSize, y * kGemSize));
		hintSprite->setScale(0.75);
		hintSprite->setAnchorPoint(ccp(0, 0));
		mHintLayer->addChild(hintSprite);
		hintSprite->runAction(action);
	}
}

void GameScene::updateSparkle()
{
	//if (RAND_0_TO_1 > 0.000005) return;
	
	if(rand() % 100 > 5) return;
	int idx = rand()  % kNumTotalGems;
	CCSprite* gemSprite = mBoardSprites[idx];
	if (mBoard[idx] < 0 || mBoard[idx] >= kGemTypeCount - 1) return;
	if (!gemSprite) return;

	if(gemSprite->getChildrenCount() > 0) return;
	//if(gemSprite->getChildren()->count() > 0) return;

	CCSprite* sprite = CCSprite::createWithSpriteFrameName("crystals/sparkle.png");
	sprite->runAction(CCRepeatForever::create(CCRotateBy::create(3, 360)));

	sprite->setOpacity(0);

	sprite->runAction(CCSequence::create(
		CCFadeIn::create(0.5),
		CCFadeOut::create(2),
		CCCallFuncN::create(this,callfuncN_selector(GameScene::onRemoveFromParent)),
		NULL));

	sprite->setPosition(ccp(kGemSize * (2 / 6),  kGemSize * (4 / 6)));

	gemSprite->addChild(sprite);
}