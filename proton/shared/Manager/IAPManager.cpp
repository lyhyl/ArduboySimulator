#include "PlatformPrecomp.h"
#include "IAPManager.h"

#ifdef _DEBUG
	#define SHOW_DEBUG_IAP_MESSAGES
#endif

IAPManager::IAPManager()
{
	m_state = STATE_NONE;
	m_returnState = RETURN_STATE_NONE;
	m_failureReason = FAILURE_REASON_NONE;
	m_timer = 0;
	m_bWaitingForReply = false;
}

IAPManager::~IAPManager()
{
}

bool IAPManager::IsItemPurchased(const string &item) const
{
	return m_items.find(item) != m_items.end();
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
#endif
		
		return;
	}

	switch (m.GetType())
	{
	case MESSAGE_TYPE_IAP_RESULT:
	{
		m_extraData = m.GetStringParm();
	
#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Got IAP response: %d - Extra: %s", (int)m.GetParm1(), m_extraData.c_str());
#endif

		ResponseCode result = ResponseCode((int)m.GetParm1());
		switch (result)
		{
		case RESULT_OK:
		case RESULT_OK_ALREADY_PURCHASED:
			if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
			{
				//Android sends a MESSAGE_TYPE_IAP_ITEM_STATE instead, I can't remember why.. but for
				//other platforms we just do this:
				//we successfully bought it

				endPurchaseProcessWithResult(result == RESULT_OK_ALREADY_PURCHASED ? RETURN_STATE_ALREADY_PURCHASED : RETURN_STATE_PURCHASED);
			}
			break;

		case RESULT_USER_CANCELED:
			m_failureReason = FAILURE_REASON_USER_CANCELED;
		default:
			endPurchaseProcessWithResult(RETURN_STATE_FAILED);
			break;
		}

		m_itemToBuy.clear();
		break;
	}

	case MESSAGE_TYPE_IAP_ITEM_STATE:
	{
	#ifdef SHOW_DEBUG_IAP_MESSAGES
		LogMsg("Got Item State response: %d", m.GetParm1());
	#endif
		
		switch (ItemStateCode(m.GetParm1()))
		{
		case END_OF_LIST:
		{
			if (!m_itemToBuy.empty())
			{
				//now buy the item

				//but wait, do we already own it?
				if (IsItemPurchased(m_itemToBuy))
				{
					//just fake a yes to the purchase request now
					endPurchaseProcessWithResult(RETURN_STATE_ALREADY_PURCHASED);
					m_itemToBuy.clear();
					break;
				}

#ifdef SHOW_DEBUG_IAP_MESSAGES
				LogMsg("Well, we don't already own it.  Sending buy request for %s", m_itemToBuy.c_str());
#endif
				sendPurchaseMessage();
			}

			break;
		}

		case PURCHASED:
		{
#ifdef SHOW_DEBUG_IAP_MESSAGES
			LogMsg("Has purchased %s", m.GetStringParm().c_str());
#endif

			bool added = m_items.insert(m.GetStringParm()).second;
#ifdef SHOW_DEBUG_IAP_MESSAGES
			if (added)
			{
				LogMsg("Added %s, now has %d purchased items", m.GetStringParm().c_str(), m_items.size());
			}
#endif

			if (m_itemToBuy.empty())
			{
				//done buying a real item
				endPurchaseProcessWithResult(RETURN_STATE_PURCHASED);
			} else
			{
				//we're actually just scanning purchased items, not finished
			}

			break;
		}

		case CANCELED:
		case REFUNDED:
			endPurchaseProcessWithResult(RETURN_STATE_FAILED);
			break;
		}
		break;
	}

	default:
		break;
	}
}

bool IAPManager::Init()
{
	return true;
}

void IAPManager::Update()
{
#if defined PLATFORM_WINDOWS || defined PLATFORM_LINUX
	if (m_bWaitingForReply)
	{
		//don't support billing on this platform (probably testing in desktop), fake it after 3 seconds for testing purposes

		if (m_timer+3000 < GetTick(TIMER_SYSTEM))
		{
			//fake a successful sale message back to us, so we'll process the order even while emulating on desktop
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
				//DEBUG - To test successful or faked purchases under desktop emulation
				GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_OK,0.0f,0, "Faked order for testing on desktop");

				//or, for already purchased
				//GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_OK_ALREADY_PURCHASED,0.0f,0, "Faked order for testing on desktop");


				//or, to test for a failed transaction
				//GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_USER_CANCELED,0.0f,0, "Faked order for testing on desktop");
			}
		}
	}
#endif
}

void IAPManager::BuyItem(const string &itemName)
{
	Reset();

	m_lastItemID = itemName;
	m_itemToBuy = itemName;

#ifdef SHOW_DEBUG_IAP_MESSAGES
	LogMsg("Planning to buy %s", itemName.c_str());
#endif

	
	if (GetEmulatedPlatformID() == PLATFORM_ID_ANDROID)
	{
		//The Android in-app-billing way

		//we'll query all purchased items first, because if we try to buy something already purchased, it will give an
		//annoying error to the user and not send us any response.
		//We should probably modify this to only scan once in a while but it seems very quick, so  meh.

		m_items.clear();
		m_state = STATE_WAITING;

		OSMessage o;
		o.m_type = OSMessage::MESSAGE_IAP_GET_PURCHASED_LIST;
		GetBaseApp()->AddOSMessage(o);
	} else
	{
		//issue buy command the normal way, it's not like Android where you need to get a list of stuff first
		sendPurchaseMessage();
	}
}

void IAPManager::Reset()
{
	m_state = STATE_NONE;
	m_returnState = RETURN_STATE_NONE;
	m_failureReason = FAILURE_REASON_NONE;
	m_extraData.clear();
	m_timer = 0;
}

void IAPManager::sendPurchaseMessage()
{
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_IAP_PURCHASE;
	o.m_string = m_itemToBuy;
	GetBaseApp()->AddOSMessage(o);

	m_itemToBuy.clear();
	m_timer = GetTick(TIMER_SYSTEM);
	m_bWaitingForReply = true;
	m_state = STATE_WAITING;
}

void IAPManager::endPurchaseProcessWithResult(eReturnState returnState)
{
	m_state = STATE_NONE;
	m_returnState = returnState;
	VariantList vList(uint32(m_returnState), m_extraData, m_lastItemID, uint32(m_failureReason));
	m_sig_item_purchase_result(&vList);
}
