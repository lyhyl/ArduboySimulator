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
	
	enum eReturnState
	{
		RETURN_STATE_NONE,
		RETURN_STATE_WAITING,
		RETURN_STATE_ERROR,
		RETURN_STATE_SUCCESS

	};
	
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
	string GetPointsString();
	void SetUsingTapPoints(bool bNew);
	void OpenTapjoyOfferWall();
	Variant m_tapPointVariant; //can hook onto this to get updated when it changes if needed, using sigslots
	string GetLastErrorString();
	void ClearError();
	eReturnState GetReturnState();
	bool IsReadyForTransaction(); //only need to check this to give/remove tap points
	void ModifyTapPoints(int mod);
	void GetTapPointsFromServer();

	void OnRender(); //currently only used to render a fake rect on windows to show where the ad would be.  (ads only display in Android)

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


private:
};

#endif // AdManager_h__