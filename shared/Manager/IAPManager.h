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

	enum ResponseCode
	{
		//Don't change the order, will screw up Android stuff
		RESULT_OK,
		RESULT_USER_CANCELED,
		RESULT_SERVICE_UNAVAILABLE,
		RESULT_BILLING_UNAVAILABLE,
		RESULT_ITEM_UNAVAILABLE,
		RESULT_DEVELOPER_ERROR,
		RESULT_ERROR

	};

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
};

#endif // IAPManager_h__