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

Also, there is an RTAdTest sample app for this, but it isn't going to be public until I work out some hacky ugliness on the
java side..

But here is how it would be used:

1.  Add #include "Manager/AdManager.h" to your App.h
2.  Add AdManager m_adManager; as a member variable in your App class
3.  Add m_adManager.Init(); so it gets called once at startup somewhere
4.  Add m_adManager.Update(); to your App::Update() function
5.  Add m_adManager.OnRender(); the the BOTTOM of your App::Draw() function, this is to show fake ads when testing from Windows
6.  Override BaseApp::OnMessage in your App class and add m_adManager.OnMessage(m);
	Be sure to also call the base version, example:
	void App::OnMessage( Message &m )
	{
		m_adManager.OnMessage(m);
		BaseApp::OnMessage(m);
	}

*/

#ifndef AdManager_h__
#define AdManager_h__

class AdManager
{
public:
	
	enum eReturnState
	{
		RETURN_STATE_NONE,
		RETURN_STATE_WAITING,
		RETURN_STATE_ERROR,
		RETURN_STATE_SUCCESS

	};
	
	AdManager();
	virtual ~AdManager();

	//general

	void Init();	//call this at start of app, will cache ads to get them ready
	void Update(); //call this from your App::Update
	void OnMessage(Message &m); //call this from your App::OnMessage(Message &m);
	Variant m_tapPointVariant; //can hook onto this to get updated when it changes if needed, using sigslots
	string GetLastErrorString();
	void ClearError();
	eReturnState GetReturnState();
	bool IsReadyForTransaction(); //only need to check this to give/remove tap points
	void OnRender(); //currently only used to render a fake rect on windows to show where the ad would be.  (ads only display in Android)

	//Tapjoy specific, only supported on Android currently, and only with special java files that aren't public yet

	void SetTapjoyAdVisible(bool bVisible); //will load an ad if not cached, so it might not display right away
	void SetTapjoyFeatureAppVisible(bool bVisible); //will load an ad if not cached, so it might not display right away
	void CacheTapjoyAd();
	void CacheTapjoyFeaturedApp();
	bool IsTapJoyAdReady() {return m_bTapjoyAdReady;}
	string GetPointsString();
	void OpenTapjoyOfferWall();
	void SetUsingTapPoints(bool bNew);
	void ModifyTapPoints(int mod);
	void GetTapPointsFromServer();
	void SetupBanner(CL_Vec2f vBannerSize, eAlignment alignment = ALIGNMENT_DOWN_CENTER); //alignment is ignored from now, always bottom centered
	
	boost::signal<void (VariantList*)> m_sig_tappoints_awarded; //called when awarded tap points

protected:

	eReturnState m_returnState;
	bool m_bTapjoyAdReady;
	bool m_bTapjoyFeaturedAppReady;

	bool m_bShowTapjoyAdASAP;
	bool m_bShowTapjoyFeaturedAppASAP;

	unsigned int m_updateTimer;
	bool m_bSendTapjoyAdRequestASAP;
	bool m_bSendTapjoyFeaturedAppRequestASAP;

	int m_errorCount;
	int32 m_tapPoints; //currency used with tapjoy
	bool m_bUsingTapPoints;
	string m_tapCurrency;
	string m_lastError;
	bool m_bShowingAd;
	eAlignment m_desiredBannerAlignment;
	CL_Vec2f m_vBannerSize;

private:
};

#endif // AdManager_h__