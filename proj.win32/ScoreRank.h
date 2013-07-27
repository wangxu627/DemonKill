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

#include "cocos2d.h"

class ScoreRank : public cocos2d::CCNode
{
public:
	static ScoreRank* create(const char* aBGName,const char* aFontName);
	ScoreRank(const char* aBGName,const char* aFontName);
	void setScore(int aScore);
	void setTotal(int aTotal);
	
private:
	cocos2d::CCSprite* mBG;
	cocos2d::CCLabelTTF* mLBScoreTitle;
	cocos2d::CCLabelTTF* mLBScore;
	cocos2d::CCLabelTTF* mLBTotalTitle;
	cocos2d::CCLabelTTF* mLBTotal;
	cocos2d::CCLabelTTF* mLBNewRecord;

};





