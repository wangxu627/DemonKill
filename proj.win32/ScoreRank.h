/* ===================================================== */
//	
//    ��������������               ����������������
// ����             ����         ����             ���� 
// ��                 ����     ����                 �� 
// ��      ������       ��������       ������       ��
// ��     ����  ����               ����   ����   �� 
// ��������   ����                   ����   ������
//          ��   ��               ��    �� 
//          ��   ��       �{       ��   �� 
//           ����       �^�m�a       ���� 
//             �����|�z�z   �z�z�|���� 
//               ����       ����  
//               ��           ��  
//             ������       ������
//           ���z�����|�|�|�����z��
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





