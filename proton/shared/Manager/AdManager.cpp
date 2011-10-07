#include "PlatformPrecomp.h"
#include "AdManager.h"

AdManager::AdManager()
{
	m_bTapjoyAdReady = false;
	m_updateTimer = 0;
	m_bSendTapjoyAdRequestASAP = false;
	m_bSendTapjoyFeaturedAppRequestASAP = false;
	m_bShowTapjoyFeaturedAppASAP = false;
	m_errorCount= 0;

}

AdManager::~AdManager()
{
}

void AdManager::SetTapjoyFeatureAppVisible( bool bVisible )
{
	if (bVisible)
	{
	
		if (m_bTapjoyFeaturedAppReady)
		{
			LogMsg("Showing featured app!");
			OSMessage o;
			o.m_type = OSMessage::MESSAGE_TAPJOY_SHOW_FEATURED_APP;
			o.m_x = 1; //show ad
			GetBaseApp()->AddOSMessage(o);
	
		} else
		{
			//not quite ready yet
			m_bShowTapjoyFeaturedAppASAP = true;
		}
	} else
	{
		//don't show it, if we're waiting..
		m_bSendTapjoyFeaturedAppRequestASAP = false;
	}
}

void AdManager::SetTapjoyAdVisible(bool bVisible)
{
	if (bVisible)
	{
		if (m_bTapjoyAdReady)
		{
			OSMessage o;
			o.m_type = OSMessage::MESSAGE_TAPJOY_SHOW_AD;
			o.m_x = 1; //show ad
			GetBaseApp()->AddOSMessage(o);
		} else
		{
			//do it after it becomes ready
			m_bShowTapjoyAdASAP = true;
		}

	} else
	{
		m_bShowTapjoyAdASAP = false;
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_TAPJOY_SHOW_AD;
		o.m_x = 0; //stop showing ad
		GetBaseApp()->AddOSMessage(o);

	}
}

void AdManager::CacheTapjoyAd()
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_TAPJOY_GET_AD;
	GetBaseApp()->AddOSMessage(o);
	//m_bTapjoyAdReady = false; //It might be dangerous to try to show it while we're loading it.. unsure.
	m_bSendTapjoyAdRequestASAP = false;
}

void AdManager::CacheTapjoyFeaturedApp()
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_TAPJOY_GET_FEATURED_APP;
	GetBaseApp()->AddOSMessage(o);
	m_bSendTapjoyFeaturedAppRequestASAP = false;
}

void AdManager::OnMessage( Message &m )
{
	switch (m.GetType())
	{
		case MESSAGE_TYPE_TAPJOY_FEATURED_APP_READY:

			if (m.GetParm1() == 1)
			{
				//LogMsg("Featured app is ready");
				m_bTapjoyFeaturedAppReady = true;
				
				if (m_bShowTapjoyFeaturedAppASAP)
				{
					//LogMsg("Showing featured now");

					SetTapjoyFeatureAppVisible(true);
				}
			} else
			{
				m_errorCount++;
				//error
				LogMsg("Error getting featured app: %s", m.GetStringParm().c_str());
				
				if (m.GetStringParm().find("exceeded display") != string::npos)
				{
					//LogMsg("Displayed all front page ads we can get, not showing any more.");
					SetTapjoyFeatureAppVisible(false);
				} else
				{
					m_bSendTapjoyFeaturedAppRequestASAP = true;
					m_bTapjoyFeaturedAppReady = false;

				}
			}
		
		break;

	case MESSAGE_TYPE_TAPJOY_AD_READY:
		if (m.GetParm1() == 1)
		{
#ifdef _DEBUG
			LogMsg("Tapjoy ad is ready");
#endif
			m_bTapjoyAdReady = true;

			if (m_bShowTapjoyAdASAP)
			{
				SetTapjoyAdVisible(true);
			}
		} else
		{
			//LogMsg("trying Tapjoy get ad, it had an error");
			m_bSendTapjoyAdRequestASAP = true;
			m_bTapjoyAdReady = false;
			m_errorCount++;

		}
		break;
	}
}

void AdManager::Update()
{

	if (m_errorCount > 15) return; //something is seriously wrong, let's not risk hammering the tapjoy service forever

	if (m_updateTimer >= GetTick(TIMER_SYSTEM)) return;
	m_updateTimer = GetTick(TIMER_SYSTEM)+ 1000;

	if (m_bSendTapjoyAdRequestASAP)
	{
		CacheTapjoyAd();
	}

	if (m_bSendTapjoyFeaturedAppRequestASAP)
	{
		CacheTapjoyFeaturedApp();
	}

}

void AdManager::Init()
{
	//get going on caching these for later
	CacheTapjoyAd();
	CacheTapjoyFeaturedApp();
}

