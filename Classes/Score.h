#ifndef __SCORE_H__
#define __SCORE_H__

class Score
{
public:
	Score();
	~Score();
	void updateScore(int aScore);
	int getHighestScore();
	void getScores(int aScores[]);
	void saveToFile();
	static Score* getInstance();
	
	static const int SCORE_LEN = 10;

private:
	int _scores[SCORE_LEN];
	static Score* mInstance;
};



#endif