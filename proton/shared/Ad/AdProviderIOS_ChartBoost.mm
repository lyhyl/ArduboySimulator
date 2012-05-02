#include "PlatformPrecomp.h"
#import "iOS/ChartBoost/ChartBoost.h"

void ChartBoostIOS_StartSession(string appID, string appSignature)
{
	ChartBoost *cb = [ChartBoost sharedChartBoost];
	NSString * NSappId = [NSString stringWithCString: appID.c_str() encoding: [NSString defaultCStringEncoding]];
    NSString * NSappSig = [NSString stringWithCString: appSignature.c_str() encoding: [NSString defaultCStringEncoding]];
	cb.appId = NSappId;
	cb.appSignature = NSappSig;
	[cb startSession];
    LogMsg("Initted ChartBoost.");
}

void ChartBoostIOS_ShowInterstitial(string location)
{
  
    ChartBoost *cb = [ChartBoost sharedChartBoost];
    LogMsg("Asking ChartBoost to show interstitial...");

	if (location.empty())
	{
		[cb showInterstitial];
	} else
	{
		[cb showInterstitial: [NSString stringWithCString: location.c_str() encoding: [NSString defaultCStringEncoding]]];
	}
}

void ChartBoostIOS_CacheShowInterstitial(string location)
{
  
    ChartBoost *cb = [ChartBoost sharedChartBoost];
    LogMsg("Asking ChartBoost to cache interstitial...");

	if (location.empty())
	{
		[cb cacheInterstitial];
	} else
	{
		[cb cacheInterstitial: [NSString stringWithCString: location.c_str() encoding: [NSString defaultCStringEncoding]]];
	}
}


void ChartBoostIOS_ShowMoreApps()
{
	ChartBoost *cb = [ChartBoost sharedChartBoost];
    LogMsg("Asking ChartBoost to show more apps...");
	[cb showMoreApps];
}

void ChartBoostIOS_CacheShowMoreApps()
{
	ChartBoost *cb = [ChartBoost sharedChartBoost];
    LogMsg("Asking ChartBoost to cache more apps...");
	[cb cacheMoreApps];
}