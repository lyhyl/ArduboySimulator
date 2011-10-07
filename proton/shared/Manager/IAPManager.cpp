#include "PlatformPrecomp.h"
#include "IAPManager.h"

//#define SHOW_DEBUG_IAP_MESSAGES

IAPManager::IAPManager()
{
	m_state = STATE_NONE;
	m_returnState = RETURN_STATE_NONE;
	m_timer = 0;
}

IAPManager::~IAPManager()
{
}

bool IAPManager::IsItemPurchased( const string item)
{
	for (uint32 i=0; i < m_items.size(); i++)
	{
		if (m_items[i] == item) return true;
	}

	return false;
}

void IAPManager::OnMessage( Message &m )
{

	if (m_state == STATE_NONE) return;


	if (m.GetType() == MESSAGE_TYPE_IAP_RESULT)
	{
#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Got IAP response: %d", m.GetParm1());
#endif

		if (m.GetParm1() != RESULT_OK)
		{
			m_state = STATE_NONE;
			m_returnState = RETURN_STATE_FAILED;
			m_itemToBuy.clear();
		}
	}

	if (m.GetType() == MESSAGE_TYPE_IAP_ITEM_STATE)
	{
	#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Got Item State response: %d", m.GetParm1());
	#endif
		
		if (m.GetParm1() == END_OF_LIST)
		{
			if (!m_itemToBuy.empty())
			{
				    //now buy the item

					//but wait, do we already own it?
				    if (IsItemPurchased(m_itemToBuy))
					{
						//just fake a yes to the purchase request now
						m_state = STATE_NONE;
						m_returnState = RETURN_STATE_ALREADY_PURCHASED;
						m_itemToBuy.clear();
						return;
					}
	
					#ifdef SHOW_DEBUG_IAP_MESSAGES
						LogMsg("Well, we don't already own it.  Sending buy request for %s", m_itemToBuy.c_str());
					#endif

					OSMessage o;
					o.m_type = OSMessage::MESSAGE_IAP_PURCHASE;
					o.m_string = m_itemToBuy;
					m_itemToBuy.clear();
					GetBaseApp()->AddOSMessage(o);
					m_timer = GetTick(TIMER_SYSTEM);

			}

			return;
		}
		if (m.GetParm1() == PURCHASED)
		{
		#ifdef SHOW_DEBUG_IAP_MESSAGES
			LogMsg("Has purchased %s", m.GetStringParm().c_str());
		#endif
			bool bNeedToAd = true;
			for (uint32 i=0; i < m_items.size(); i++)
			{
				if (m_items[i] ==m.GetStringParm() )
				{
					bNeedToAd = false;
				}
			}

			if (bNeedToAd)
			{
				m_items.push_back(m.GetStringParm());
				#ifdef SHOW_DEBUG_IAP_MESSAGES
					LogMsg("Added %s, now has %d purchased items", m.GetStringParm().c_str(), m_items.size());
				#endif
			}

			if (m_itemToBuy.empty())
			{
				//done buying a real item
				m_state = STATE_NONE;
				m_returnState = RETURN_STATE_PURCHASED;
			} else
			{
				//we're actually just scanning purchased items, not finished
			}
		}

		if (m.GetParm1() == CANCELED || m.GetParm1() == REFUNDED)
		{
			m_state = STATE_NONE;
			m_returnState = RETURN_STATE_FAILED;
		}
	}
}

bool IAPManager::Init()
{
	return true;
}

void IAPManager::Update()
{

	if (GetPlatformID() != PLATFORM_ID_ANDROID)
	{
		//don't support billing on this platform, fake it after 3 seconds.
		if (m_timer+3000 < GetTick(TIMER_SYSTEM))
		{
			//fake a sale
			Message m(MESSAGE_CLASS_GUI, TIMER_SYSTEM, MESSAGE_TYPE_IAP_ITEM_STATE);
			m.SetParm1(PURCHASED);
			OnMessage(m);
		}
	}
}

void IAPManager::BuyItem( string itemName )
{
	m_state = STATE_WAITING;
	m_returnState = RETURN_STATE_NONE;
	
	#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Planning to buy %s", itemName.c_str());
	#endif

	if (GetPlatformID() == PLATFORM_ID_ANDROID)
	{
		//we'll query all purchased items first, because if we try to buy something already purchased, it will give an
		//annoying error to the user and not send us any response.

		//We should probably modify this to only scan once in a while but it seems very quick, so  meh.

		//two ways to do it.. let's just try to buy directly:
	
		/*
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_IAP_PURCHASE;
		o.m_string = itemName;
		m_itemToBuy.clear();
		GetBaseApp()->AddOSMessage(o);
		m_timer = GetTick(TIMER_SYSTEM);
		*/

		

		m_items.clear();
		m_itemToBuy = itemName;


		OSMessage o;
		o.m_type = OSMessage::MESSAGE_IAP_GET_PURCHASED_LIST;
		GetBaseApp()->AddOSMessage(o);



	} else
	{
		//skip that, we're testing
		
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_IAP_PURCHASE;
		o.m_string = itemName;
		m_itemToBuy.clear();
		GetBaseApp()->AddOSMessage(o);
		m_timer = GetTick(TIMER_SYSTEM);
	}
	
}

