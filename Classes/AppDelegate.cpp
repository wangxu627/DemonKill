#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "MainScene.h"
#include "FileUtils.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainScene::scene();

    // run
    pDirector->runWithScene(pScene);

	//
	FileUtils* fu = new FileUtils("data.dat","r+");
	char buffer[64];
	int n = fu->read(buffer,64);
	if(n == 0)
	{
		for(int i = 0;i < 10;i++)
		{
			_scores[i] = 0;
		}
	}
	else
	{
		int i = 0;
		char* p = buffer;
		char* q = NULL;
		do 
		{
			q = strchr(p,';');
			if(q != NULL)
			{
				char s[16] = {0};
				memcpy(s,p,q - p);
				_scores[i++] = atoi(s);
			}
			p = q + 1;
		} while (q);

		for(;i < 10;i++)
		{
			_scores[i] = 0;
		}
	}
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
}
