//  ***************************************************************
//  IAPManager - Creation date: 10/04/2011
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2011 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef IAPManager_h__
#define IAPManager_h__

class IAPManager
{
public:
	IAPManager();
	virtual ~IAPManager();


	enum eReturnState
	{
		RETURN_STATE_NONE,
		RETURN_STATE_FAILED,
		RETURN_STATE_PURCHASED,
		RETURN_STATE_ALREADY_PURCHASED, //applicable to managed items
		RETURN_STATE_FINISHED
	};

	bool Init();
	void Update();
	void OnMessage( Message &m );
	eReturnState GetReturnState() {return m_returnState;}

	void BuyItem(string itemName);
	bool IsItemPurchased( const string item);
	void Reset(); //call this after a purchase to reset things
	string GetExtraData() {return m_extraData;} //the order# or receipt, after a successful purchase
	string GetItemID() {return m_lastItemID;} //last itemID that was processed

	enum ResponseCode
	{
		//Don't change the order, will screw up Android stuff
		RESULT_OK,
		RESULT_USER_CANCELED,
		RESULT_SERVICE_UNAVAILABLE,
		RESULT_BILLING_UNAVAILABLE,
		RESULT_ITEM_UNAVAILABLE,
		RESULT_DEVELOPER_ERROR,
		RESULT_ERROR,
        RESULT_OK_ALREADY_PURCHASED

	};

	/**
	 * A signal for reporting changes in the purchasing process.
	 *
	 * The parameter variant list contains the following items:
	 * - 0: the return code of type \c eReturnState as a uint.
	 * - 1: a string that gives more information about the result. This might depend on the platform.
	 * - 2: is the item id in question (as a string).
	 */
	boost::signal<void (VariantList*)> m_sig_item_purchase_result; 
	
protected:
	
	enum eState
	{
		STATE_NONE,
		STATE_WAITING,
	};

	enum ItemStateCode
	{
		//Don't change the order, will screw up Android stuff
		END_OF_LIST = -1,
		PURCHASED,
		CANCELED,
		REFUNDED
	};
	
	eState m_state;
	eReturnState m_returnState;
	unsigned int m_timer;

	vector<string> m_items;
	string m_itemToBuy;
	string m_extraData;
	bool m_bWaitingForReply;
	string m_lastItemID;
};

#endif // IAPManager_h__