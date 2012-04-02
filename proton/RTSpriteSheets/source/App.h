/*
 *  App.h
 *  Created by Aki Koskinen on 2/4/12.
 *  For license info, check the license.txt file that should have come with this.
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
	virtual void Update();

	string GetVersionString();
	float GetVersion();
	int GetBuild();
	void OnExitApp(VariantList *pVarList);

private:
	bool m_bDidPostInit;
};
 

extern App g_App;

App * GetApp();
const char * GetAppName();
const char * GetBundleName();
const char * GetBundlePrefix();

#endif // App_h_
