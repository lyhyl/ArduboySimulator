#include "PlatformPrecomp.h"
#include "AdProviderChartBoost.h"

#ifdef PLATFORM_IOS
#include "AdProviderIOS_ChartBoost.h"
#endif

AdProviderChartBoost::AdProviderChartBoost()
{
	m_adProviderType = AD_PROVIDER_CHARTBOOST;
}

AdProviderChartBoost::~AdProviderChartBoost()
{
}

bool AdProviderChartBoost::OnAddToManager( AdManager *pAdManager )
{
	if (m_appID.empty())
	{
		LogError("You must call AdProviderChartBoost::SetupInfo with the info you get from ChartBoost before you add this to the manager!");
		return false;
	}

#ifdef PLATFORM_IOS
	ChartBoostIOS_StartSession(m_appID, m_appSignature);
#else
	LogMsg("(can't use ChartBoost on this platform)");
#endif
	return true;
}

void AdProviderChartBoost::Update()
{
}

void AdProviderChartBoost::SetupInfo( const string appID, const string appSignature )
{
	m_appID = appID;
	m_appSignature = appSignature;
	
}

void AdProviderChartBoost::ShowInterstitial( std::string location /*= ""*/, std::string parm2 /*= ""*/, std::string parm3 /*= ""*/ )
{
#ifdef PLATFORM_IOS
	ChartBoostIOS_ShowInterstitial(location);
#else
	LogMsg("(can't use ChartBoost on this platform)");
#endif

}

void AdProviderChartBoost::CacheShowInterstitial( std::string location /*= ""*/, std::string parm2 /*= ""*/, std::string parm3 /*= ""*/ )
{
#ifdef PLATFORM_IOS
	ChartBoostIOS_CacheShowInterstitial(location);
#else
	LogMsg("(can't use ChartBoost on this platform)");
#endif

}

void AdProviderChartBoost::ShowMoreApps( std::string location /*= ""*/, std::string parm2 /*= ""*/, std::string parm3 /*= ""*/ )
{
#ifdef PLATFORM_IOS
    ChartBoostIOS_ShowMoreApps();
#else
	LogMsg("(can't use ChartBoost on this platform)");
#endif

}

void AdProviderChartBoost::CacheShowMoreApps( std::string location /*= ""*/, std::string parm2 /*= ""*/, std::string parm3 /*= ""*/ )
{
#ifdef PLATFORM_IOS
	ChartBoostIOS_CacheShowMoreApps();
#else
	LogMsg("(can't use ChartBoost on this platform)");
#endif

}

bool AdProviderChartBoost::OnMessage( Message &m )
{

	return false; //we didn't handle it, so keep processing
}