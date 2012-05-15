//  ***************************************************************
//  IAPManager - Creation date: 10/04/2011
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2011 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//                  Aki Koskinen (aki@secondlion.fi)
//  ***************************************************************

#ifndef IAPManager_h__
#define IAPManager_h__

#include <set>
#include <string>

/**
 * A class for doing in-app purchases (IAP).
 *
 * This manager can be used to make an IAP for a single item at a time. It can
 * also be used to check if some item has already been purchased.
 *
 * An IAP process is started for an item with \c BuyItem(). Later on the
 * \c IAPManager tells how the purchase ended. The end result can be received
 * by listening to the \c m_sig_item_purchase_result signal or using the
 * various state methods of the class.
 */
class IAPManager
{
public:
	IAPManager();
	virtual ~IAPManager();

	/**
	 * The result of a purchase process.
	 */
	enum eReturnState
	{
		RETURN_STATE_NONE,                //!< No result available. This is used e.g. when no purchases have been initiated yet.
		RETURN_STATE_FAILED,              //!< The purchase process has failed.
		RETURN_STATE_PURCHASED,           //!< The purchase process has finished successfully.
		RETURN_STATE_ALREADY_PURCHASED    //!< The item has been purchased already previously. Applicable to managed items.
	};

	/**
	 * Explanation reason for a failed purchase.
	 */
	enum eFailureReason
	{
		FAILURE_REASON_NONE,              //!< Either there was no failure or the reason is not known.
		FAILURE_REASON_USER_CANCELED      //!< The user canceled the purchase.
	};

	bool Init();
	void Update();

	/**
	 * Use this to pass messages for the IAPManager to handle.
	 * \note This needs to be called in order for the system to work at all!
	 */
	void OnMessage( Message &m );

	/**
	 * Gets the state of the last finished purchase process.
	 * If a purchase process is in progress or hasn't been initiated yet
	 * then the state is \link IAPManager::RETURN_STATE_NONE <tt>RETURN_STATE_NONE</tt>\endlink.
	 */
	eReturnState GetReturnState() const {return m_returnState;}

	/**
	 * Gets the detailed reason for a failed purchase.
	 * The value returned by this method is only interesting when \c GetReturnState()
	 * returns \link IAPManager::RETURN_STATE_FAILED <tt>RETURN_STATE_FAILED</tt>\endlink.
	 * If a purchase has not failed then this method returs \link IAPManager::FAILURE_REASON_NONE
	 * <tt>FAILURE_REASON_NONE</tt>\endlink.
	 */
	eFailureReason GetFailureReason() const { return m_failureReason; }

	/**
	 * Starts a purchasing process for the given \a itemName.
	 */
	void BuyItem(const std::string &itemName);
	bool IsItemPurchased(const std::string &item) const;

	/**
	 * Resets the state of the IAPManager.
	 */
	void Reset();

	/**
	 * The extra data is platform dependent details about the purchase.
	 * It can be for example the order# or receipt.
	 */
	std::string GetExtraData() const {return m_extraData;}

	/**
	 * Returns the last item name BuyItem() was called with.
	 */
	std::string GetItemID() const {return m_lastItemID;}

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
	 * - 0: the return code of type \link IAPManager::eReturnState <tt>eReturnState</tt>\endlink
	 *      as a uint. See \c GetReturnState().
	 * - 1: a string that gives more information about the result. This might depend on the
	 *      platform. See \c GetExtraData().
	 * - 2: the item id in question (as a string). See \c GetItemID().
	 * - 3: if parameter 0 is \link IAPManager::RETURN_STATE_FAILED <tt>RETURN_STATE_FAILED</tt>\endlink
	 *      then this parameter contains the detailed failure reason if it's known. The type of this
	 *      parameter is \link IAPManager::eFailureReason <tt>eFailureReason</tt>\endlink as a uint.
	 *      See \c GetFailureReason().
	 */
	boost::signal<void (VariantList*)> m_sig_item_purchase_result; 
	
protected:
	enum eState
	{
		STATE_NONE,
		STATE_WAITING
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
	eFailureReason m_failureReason;
	unsigned int m_timer;

	std::set<std::string> m_items;
	std::string m_itemToBuy;
	std::string m_extraData;
	bool m_bWaitingForReply;
	std::string m_lastItemID;

	void sendPurchaseMessage();

	/**
	 * Changes the state of the purchasing process and sends the m_sig_item_purchase_result signal.
	 */
	void endPurchaseProcessWithResult(eReturnState returnState);
};

#endif // IAPManager_h__
