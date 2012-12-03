/*
 *  App.h
 *  Created by Seth Robinson
 *  For license info, check the license.txt file that should have come with this.
 *
 
 This example helps me test in app billing and the integration of various ad providers.
 */

#pragma once

#include "BaseApp.h"
#include "Manager/IAPManager.h"
#include "Manager/AdManager.h"

#ifndef DISABLE_CHARTBOOST
#define RT_CHARTBOOST_ENABLED
#endif

#define RT_FLURRY_ENABLED


#define RT_TAPJOY_ENABLED //for ios, this must be declared for the whole project, not just here

class App: public BaseApp
{
public:
	
	App();
	virtual ~App();
	
	virtual bool Init();
	virtual void Kill();
	virtual void Draw();
	virtual void OnScreenSizeChange();
	virtual void OnEnterBackground();
	virtual void OnEnterForeground();
	virtual void Update();
	virtual void OnMessage(Message &m);

	void OnExitApp(VariantList *pVarList);
	AdManager * GetAdManager() {return &m_adManager;}
	
	//we'll wire these to connect to some signals we care about
	void OnAccel(VariantList *pVList);
	void OnArcadeInput(VariantList *pVList);

	IAPManager m_IAPManager;
	AdManager m_adManager;

private:

	bool m_bDidPostInit;
	Surface m_surf; //for testing
};


extern App g_App;

App * GetApp();
const char * GetAppName();
const char * GetBundlePrefix();
const char * GetBundleName();
