#include "PlatformPrecomp.h"
#include "IAPManager.h"

#ifdef _DEBUG
	#define SHOW_DEBUG_IAP_MESSAGES
#endif

IAPManager::IAPManager()
{
	m_state = STATE_NONE;
	m_returnState = RETURN_STATE_NONE;
	m_timer = 0;
	m_bWaitingForReply = false;
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

	if (m_state == STATE_NONE) 
	{
#ifdef SHOW_DEBUG_IAP_MESSAGES
	
		if (m.GetType() == MESSAGE_TYPE_IAP_RESULT || m.GetType() == MESSAGE_TYPE_IAP_ITEM_STATE)
		{
			LogMsg("Ignoring IAP response, wrong state: %d - Extra: %s", (int)m.GetParm1(), m_extraData.c_str());
		}
#endif		return;
	}


	if (m.GetType() == MESSAGE_TYPE_IAP_RESULT)
	{
		m_extraData = m.GetStringParm();
	
#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Got IAP response: %d - Extra: %s", (int)m.GetParm1(), m_extraData.c_str());
#endif
        int result = (int)m.GetParm1();
        
		if (result != RESULT_OK && result != RESULT_OK_ALREADY_PURCHASED)
		{
			m_state = STATE_NONE;
			m_returnState = RETURN_STATE_FAILED;

			VariantList vList(uint32(m_returnState), m_extraData, m_itemToBuy);
			m_sig_item_purchase_result(&vList);
			m_itemToBuy.clear();
		} else
		{

			if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
			{
				//Android sends a MESSAGE_TYPE_IAP_ITEM_STATE instead, I can't remember why.. but for
				//other platforms we just do this:
				//we successfully bought it

				m_state = STATE_NONE;
				m_returnState = RETURN_STATE_PURCHASED;
				
                if (result == RESULT_OK_ALREADY_PURCHASED)
                {
                    //more accurate
                    m_returnState = RETURN_STATE_ALREADY_PURCHASED;
                }
                
        		VariantList vList(uint32(m_returnState), m_extraData, m_itemToBuy);
				m_sig_item_purchase_result(&vList);
				m_itemToBuy.clear();
			}
		}
	}

	if (m.GetType() == MESSAGE_TYPE_IAP_ITEM_STATE)
	{
	#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Got Item State response: %d", m.GetParm1());
	#endif
		
		if ((int)m.GetParm1() == END_OF_LIST)
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
						VariantList vList(uint32(m_returnState), m_extraData, m_itemToBuy);
						m_sig_item_purchase_result(&vList);
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
		if ((int)m.GetParm1() == PURCHASED)
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
				VariantList vList(uint32(m_returnState), m_extraData, m_itemToBuy);
				m_sig_item_purchase_result(&vList);
			} else
			{
				//we're actually just scanning purchased items, not finished
			}
		}

		if (m.GetParm1() == CANCELED || m.GetParm1() == REFUNDED)
		{
			m_state = STATE_NONE;
			m_returnState = RETURN_STATE_FAILED;
			VariantList vList(uint32(m_returnState), m_extraData, m_itemToBuy);
			m_sig_item_purchase_result(&vList);
		}
	}
}

bool IAPManager::Init()
{
	return true;
}

void IAPManager::Update()
{

#if !defined RT_WEBOS && !defined PLATFORM_IOS
	if (m_bWaitingForReply && GetPlatformID() != PLATFORM_ID_ANDROID && GetPlatformID() != PLATFORM_ID_WEBOS)
	{
		//don't support billing on this platform (probably testing in Windows), fake it after 3 seconds for testing purposes

		if (m_timer+3000 < GetTick(TIMER_SYSTEM))
		{
			//fake a successful sale message back to us, so we'll process the order even while emulating on win/whatever
			m_bWaitingForReply = false;
			
			if (GetEmulatedPlatformID() == PLATFORM_ID_ANDROID)
			{
				//I'm not sure why Android sends this message instead of just MESSAGE_TYPE_IAP_RESULT.. but some client games
				//rely on this behavior so I don't want to mess with it.. something to do with how Android actually checks the
				//status of all IAP when you try to buy something, to see if it's already been bought...
				Message m(MESSAGE_CLASS_GUI, TIMER_SYSTEM, MESSAGE_TYPE_IAP_ITEM_STATE);
				m.SetParm1(PURCHASED); //to fake purchased
				//m.SetParm1(CANCELED); //to fake canceled
				OnMessage(m);
			} else
			{
				//WebOS & iOS
				//DEBUG - To test successful or faked purchases under Win emulation
				//GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_OK,0.0f,0, "Faked order!");

				//or, for already purchased
				GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_OK_ALREADY_PURCHASED,0.0f,0, "Faked order!");


				//or, to test for a failed transaction
				//GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_USER_CANCELED,0.0f,0, "Faked order!");

			}

			
		}
	}

#endif
}

void IAPManager::BuyItem( string itemName )
{
	Reset();

	m_lastItemID = itemName;

	#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Planning to buy %s", itemName.c_str());
	#endif

	
	if (GetPlatformID() == PLATFORM_ID_ANDROID)
	{
		//The Android in-app-billing way

		//we'll query all purchased items first, because if we try to buy something already purchased, it will give an
		//annoying error to the user and not send us any response.
		//We should probably modify this to only scan once in a while but it seems very quick, so  meh.

		m_items.clear();
		m_itemToBuy = itemName;
	
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_IAP_GET_PURCHASED_LIST;
		GetBaseApp()->AddOSMessage(o);
	} else
	{
		//issue buy command the normal way, it's not like Android where you need to get a list of stuff first
		
		OSMessage o;
		o.m_type = OSMessage::MESSAGE_IAP_PURCHASE;
		o.m_string = itemName;
	
		m_itemToBuy.clear();
		GetBaseApp()->AddOSMessage(o);
		m_timer = GetTick(TIMER_SYSTEM);
		m_bWaitingForReply = true;
		m_state = STATE_WAITING;
	}
}

void IAPManager::Reset()
{
	m_state = STATE_NONE;
	m_returnState = RETURN_STATE_NONE;
	m_extraData.clear();
	m_timer = 0;
}
