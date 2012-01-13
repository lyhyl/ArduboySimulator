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
	m_tapPoints = -1; //-1 means don't know
	m_bUsingTapPoints= false;
	m_tapPointVariant.Set(m_tapPoints);
	m_returnState = RETURN_STATE_NONE;
	m_bShowTapjoyAdASAP = false;
	m_bShowingAd = false;
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
	m_bShowingAd = bVisible;

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

void AdManager::OpenTapjoyOfferWall()
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_TAPJOY_SHOW_OFFERS;
	GetBaseApp()->AddOSMessage(o);
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


	
	if (m_returnState == RETURN_STATE_WAITING)
	{
		switch (m.GetType())
		{
		case MESSAGE_TYPE_TAPJOY_SPEND_TAP_POINTS_RETURN:
			m_returnState = RETURN_STATE_SUCCESS;
			ClearError();
			break;

		case MESSAGE_TYPE_TAPJOY_SPEND_TAP_POINTS_RETURN_ERROR:
			m_returnState = RETURN_STATE_ERROR;
			m_lastError = m.GetStringParm();

			break;
		}
	}

	switch (m.GetType())
	{

	
	case MESSAGE_TYPE_TAPJOY_EARNED_TAP_POINTS:
		{
			LogMsg("We just got %d tappoints!", (int)m.GetParm1());
			VariantList vList((int32)m.GetParm1());

			m_sig_tappoints_awarded(&vList); //called when awarded tap points
		}

		break;

	case MESSAGE_TYPE_TAPJOY_TAP_POINTS_RETURN:
		
		m_tapPoints = (int32)m.GetParm1();
		m_tapPointVariant.Set(m_tapPoints);
		m_tapCurrency = m.GetStringParm();
#ifdef _DEBUG
		LogMsg("Tap points set to %d %s", m_tapPoints, m_tapCurrency.c_str());
#endif
		break;

	case MESSAGE_TYPE_TAPJOY_MOVIE_AD_READY:
		if (m.GetParm1() == 1)
		{
			LogMsg("Video ad is ready!");
		} else
		{
			LogMsg("Video ad failed %d - %s", m.GetParm2(), m.GetStringParm().c_str());
		}

		break;
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

void AdManager::GetTapPointsFromServer()
{
	if (m_bUsingTapPoints)
	{
		LogMsg("Requesting latest info from Tapjoy");
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_TAPJOY_GET_TAP_POINTS;
		GetBaseApp()->AddOSMessage(o);
	}
}

void AdManager::Init()
{
	
//	GetTapPointsFromServer();
	//CacheTapjoyFeaturedApp();

}

std::string AdManager::GetPointsString()
{
	if (m_tapPoints == -1)
	{
		return "Offline";
	}

	return toString(m_tapPoints)+" "+m_tapCurrency;
}

void AdManager::SetUsingTapPoints( bool bNew )
{
	if (bNew == m_bUsingTapPoints) return;

	assert(bNew && "Why would you turn this off?");

	m_bUsingTapPoints = bNew;

}
string AdManager::GetLastErrorString()
{
	return m_lastError;
}

void AdManager::ClearError()
{
	m_lastError.clear();


}

AdManager::eReturnState AdManager::GetReturnState()
{
	return m_returnState;
}

bool AdManager::IsReadyForTransaction()
{
	return !m_tapCurrency.empty();
}

void AdManager::ModifyTapPoints( int mod )
{
	m_returnState = RETURN_STATE_WAITING;
	ClearError();

	if (mod < 0)
	{
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_TAPJOY_SPEND_TAP_POINTS;
		o.m_parm1 = abs(mod); //turn it positive
		GetBaseApp()->AddOSMessage(o);
	} else
	{
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_TAPJOY_AWARD_TAP_POINTS;
		o.m_parm1 = mod;
		GetBaseApp()->AddOSMessage(o);
	}
}

void AdManager::OnRender()
{

#ifdef WIN32
	if (m_bShowingAd)
	{
		
		CL_Vec2f vRatio = CL_Vec2f(1,1);
		if (GetFakePrimaryScreenSizeX() != 0)
		{
			vRatio.x = (GetScreenSizeXf()/float(GetOriginalScreenSizeX()));
			vRatio.y =(GetScreenSizeYf()/float(GetOriginalScreenSizeY()));
		}
		rtRect r(0,0,480*vRatio.x,80*vRatio.y);
		
		//move to bottom
		r.AdjustPosition(0, GetScreenSizeYf()-r.GetHeight());

		//center
		r.AdjustPosition( (GetScreenSizeXf()-r.GetWidth())/2, 0 );
		
		DrawRect(r, MAKE_RGBA(255,255,255,255));
		
	}
#endif

}