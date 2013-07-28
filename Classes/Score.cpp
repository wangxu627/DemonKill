#include "Score.h"
#include "FileUtils.h"
#include "CPPUtils.h"
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

Score* Score::mInstance = NULL;

Score::Score()
{
	FileUtils* fu = new FileUtils("data.dat","r+");
	int i = 0;
	if(fu->state() == 0)
	{
		char buffer[64] = {0};
		int n = fu->read(buffer,64);
		fu->close();

		vector<string> arr;
		CPPUtils::split(buffer,';',arr);

		for(;i < arr.size();i++)
		{
			_scores[i] = atoi(arr[i].c_str());
		}
	}
	for(;i < SCORE_LEN;i++)
	{
		_scores[i] = -1;
	}
	delete fu;

#if _DEBUG
	for(int i = 0;i < SCORE_LEN;i++)
	{
		char buf[32];
		sprintf(buf,"SCORE %d ==> %d\n",i,_scores[i]);
		::OutputDebugStringA(buf);
	}
#endif
}

Score::~Score()
{
	saveToFile();
}

void Score::updateScore(int aScore)
{
	int idx = 0;
	for(;idx < SCORE_LEN;idx++)
	{
		if(aScore > _scores[idx])
		{
			break;
		}
	}

	for(int i = SCORE_LEN - 2;i >= idx;i--)
	{
		_scores[i + 1] = _scores[i];
	}

	if(idx != SCORE_LEN)
	{
		_scores[idx] = aScore;
	}

	saveToFile();
}

int Score::getHighestScore()
{
	return _scores[0];
}

void Score::getScores(int aScores[])
{
	memcpy(aScores,_scores,sizeof(int) * SCORE_LEN);
}

void Score::saveToFile()
{
	string out;
	vector<string> arr;
	for(int i = 0;i < SCORE_LEN;i++)
	{
		char buf[8];
		itoa(_scores[i],buf,10);
		arr.push_back(buf);
	}
	CPPUtils::combine(arr,';',out);
	FileUtils* fu = new FileUtils("data.dat","w");
	fu->write((void*)out.c_str(),strlen(out.c_str()));
	fu->close();
	delete fu;
}

Score* Score::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new Score();
	}
	return mInstance;
}
