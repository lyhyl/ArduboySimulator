//  ***************************************************************
//  AdManager - Creation date: 09/30/2011
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2011 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

/*

Seth's class to control Tapjoy ads on Android.  Needs special versions of the Android .java files to work...

*/

#ifndef AdManager_h__
#define AdManager_h__

class AdManager
{
public:
	AdManager();
	virtual ~AdManager();

	void SetTapjoyAdVisible(bool bVisible);
	void SetTapjoyFeatureAppVisible(bool bVisible);

	void Init();	//call this at start of app, will cache ads to get them ready
	void Update(); //call this from your App::Update
	void OnMessage(Message &m); //call this from your App::OnMessage(Message &m);
	void CacheTapjoyAd();
	void CacheTapjoyFeaturedApp();
	bool IsTapJoyAdReady() {return m_bTapjoyAdReady;}

protected:

	bool m_bTapjoyAdReady;
	bool m_bTapjoyFeaturedAppReady;

	bool m_bShowTapjoyAdASAP;
	bool m_bShowTapjoyFeaturedAppASAP;

	unsigned int m_updateTimer;
	bool m_bSendTapjoyAdRequestASAP;
	bool m_bSendTapjoyFeaturedAppRequestASAP;

	int m_errorCount;

private:
};

#endif // AdManager_h__