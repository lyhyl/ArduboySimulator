/*
 *  App.h
 *  Created by Aki Koskinen on 2/4/12.
 *  For license info, check the license.txt file that should have come with this.
 *
 */

#ifndef App_h_
#define App_h_

#include "BaseApp.h"

class App: public BaseApp
{
public:
	
	App();
	virtual ~App();
	
	virtual bool Init();
	virtual void Kill();
	virtual void Draw();
	virtual void OnScreenSizeChange();
	virtual void Update();
	virtual void OnEnterBackground();

	string GetVersionString();
	float GetVersion();
	int GetBuild();
//	void GetServerInfo(string &server, uint32 &port);
//	VariantDB * GetShared() {return &m_varDB;}
//	Variant * GetVar(const string &keyName );
//	Variant * GetVarWithDefault(const string &varName, const Variant &var) {return m_varDB.GetVarWithDefault(varName, var);}
//	int GetSpecial();
	void OnExitApp(VariantList *pVarList);

private:

//	void SaveOurStuff();

	bool m_bDidPostInit;
	//VariantDB m_varDB;
	//int m_special;
};
 

extern App g_App;

App * GetApp();
const char * GetAppName();
const char * GetBundleName();
const char * GetBundlePrefix();

#endif // App_h_
