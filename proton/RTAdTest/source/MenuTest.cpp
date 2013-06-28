#include "PlatformPrecomp.h"
#include "MenuTest.h"
#include "MenuMain.h"

#include "Entity/EntityUtils.h"
#include "Manager/MessageManager.h"


unsigned int numSignals = 0;
IAPManager::eReturnState sigReturnState = IAPManager::RETURN_STATE_NONE;
string sigExtraData;
string sigItemId;
IAPManager::eFailureReason sigFailureReason = IAPManager::FAILURE_REASON_NONE;

void MenuTestPurchaseResult(VariantList *vList)
{
	++numSignals;
	sigReturnState = IAPManager::eReturnState(vList->Get(0).GetUINT32());
	sigExtraData = vList->Get(1).GetString();
	sigItemId = vList->Get(2).GetString();
	sigFailureReason = IAPManager::eFailureReason(vList->Get(3).GetUINT32());
}

#ifdef RT_TESTFW
#include "testfw/ProtonTester.h"
#include "testfw/ProtonTesterGUI.h"

ePlatformID originalPlatformID;

INIT_TEST()
{
	GetApp()->m_IAPManager.Reset();

	originalPlatformID = GetEmulatedPlatformID();
}

CLEAN_TEST()
{
	GetApp()->m_IAPManager.Reset();

	numSignals = 0;
	sigReturnState = IAPManager::RETURN_STATE_NONE;
	sigExtraData.clear();
	sigItemId.clear();
	sigFailureReason = IAPManager::FAILURE_REASON_NONE;

	GetBaseApp()->KillOSMessagesByType(OSMessage::MESSAGE_IAP_PURCHASE);
	GetBaseApp()->KillOSMessagesByType(OSMessage::MESSAGE_IAP_GET_PURCHASED_LIST);

	SetEmulatedPlatformID(originalPlatformID);
}

const string testItem("testItem");
const string extraData("extraData");

void sendIAPResultMessageToIAPManager(IAPManager::ResponseCode responseCode)
{
	Message msg(MESSAGE_CLASS_GUI, TIMER_SYSTEM, MESSAGE_TYPE_IAP_RESULT);
	msg.SetParm1((float)responseCode);
	msg.SetStringParm(extraData);
	GetApp()->m_IAPManager.OnMessage(msg);
}

enum ItemStateCode
{
	//Don't change the order, will screw up Android stuff
	END_OF_LIST = -1,
	PURCHASED,
	CANCELED,
	REFUNDED
};

void sendIAPItemStateMessageToIAPManager(ItemStateCode itemStateCode)
{
	Message msg(MESSAGE_CLASS_GUI, TIMER_SYSTEM, MESSAGE_TYPE_IAP_ITEM_STATE);
	msg.SetParm1((float)itemStateCode);
	if (itemStateCode == PURCHASED)
	{
		msg.SetStringParm(testItem);
	}
	GetApp()->m_IAPManager.OnMessage(msg);
}

bool isOSMessageInQueue(OSMessage::eMessageType expectedMessageType, bool expectItemIdAsMessageString)
{
	bool wantedMessageFound = false;

	deque<OSMessage> *msgs = GetBaseApp()->GetOSMessages();
	for (deque<OSMessage>::iterator it(msgs->begin()); it != msgs->end(); it++)
	{
		if ((*it).m_type == expectedMessageType && (!expectItemIdAsMessageString || (*it).m_string == testItem))
		{
			wantedMessageFound = true;
			break;
		}
	}

	return wantedMessageFound;
}

void execute_test_when_requesting_to_buy_an_item_then_IAPManager_adds_a_correct_OSMessage(ePlatformID platform, OSMessage::eMessageType expectedMessageType, bool expectItemIdAsMessageString)
{
	SetEmulatedPlatformID(platform);

	GetApp()->m_IAPManager.BuyItem(testItem);

	CheckTrue(isOSMessageInQueue(expectedMessageType, expectItemIdAsMessageString));
}

TEST(When_requesting_to_buy_an_item_on_ios_then_IAPManager_adds_a_correct_OSMessage)
{
	execute_test_when_requesting_to_buy_an_item_then_IAPManager_adds_a_correct_OSMessage(PLATFORM_ID_IOS, OSMessage::MESSAGE_IAP_PURCHASE, true);
}

TEST(When_requesting_to_buy_an_item_on_android_then_IAPManager_adds_a_correct_OSMessage)
{
	execute_test_when_requesting_to_buy_an_item_then_IAPManager_adds_a_correct_OSMessage(PLATFORM_ID_ANDROID, OSMessage::MESSAGE_IAP_GET_PURCHASED_LIST, false);
}

void checkSignal(IAPManager::eReturnState expectedReturnState, const std::string& expectedExtraData = string(), IAPManager::eFailureReason expectedFailureReason = IAPManager::FAILURE_REASON_NONE)
{
	CheckEq((unsigned int)1, numSignals);
	CheckEq(expectedReturnState, sigReturnState);
	CheckEq(expectedExtraData, sigExtraData);
	CheckEq(testItem, sigItemId);
	CheckEq(expectedFailureReason, sigFailureReason);
}

void checkQueryAPI(IAPManager::eReturnState expectedReturnState, const std::string& expectedExtraData = string(), IAPManager::eFailureReason expectedFailureReason = IAPManager::FAILURE_REASON_NONE)
{
	CheckEq(expectedReturnState, GetApp()->m_IAPManager.GetReturnState());
	CheckEq(expectedExtraData, GetApp()->m_IAPManager.GetExtraData());
	CheckEq(testItem, GetApp()->m_IAPManager.GetItemID());
	CheckEq(expectedFailureReason, GetApp()->m_IAPManager.GetFailureReason());
}

void execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters(IAPManager::ResponseCode iapResult, IAPManager::eReturnState expectedReturnState, IAPManager::eFailureReason expectedFailureReason = IAPManager::FAILURE_REASON_NONE, ePlatformID enforcePlatform = PLATFORM_ID_UNKNOWN)
{
	if (enforcePlatform != PLATFORM_ID_UNKNOWN)
	{
		SetEmulatedPlatformID(enforcePlatform);
	}

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPResultMessageToIAPManager(iapResult);

	checkSignal(expectedReturnState, extraData, expectedFailureReason);
}

void execute_test_when_IAPManager_receives_OSMessage_then_it_returns_correct_values_via_the_query_interface(IAPManager::ResponseCode iapResult, IAPManager::eReturnState expectedReturnState, IAPManager::eFailureReason expectedFailureReason = IAPManager::FAILURE_REASON_NONE, ePlatformID enforcePlatform = PLATFORM_ID_UNKNOWN)
{
	if (enforcePlatform != PLATFORM_ID_UNKNOWN)
	{
		SetEmulatedPlatformID(enforcePlatform);
	}

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPResultMessageToIAPManager(iapResult);

	checkQueryAPI(expectedReturnState, extraData, expectedFailureReason);
}

void execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::ResponseCode iapResult, IAPManager::eReturnState expectedReturnState, IAPManager::eFailureReason expectedFailureReason = IAPManager::FAILURE_REASON_NONE, ePlatformID enforcePlatform = PLATFORM_ID_UNKNOWN)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters(iapResult, expectedReturnState, expectedFailureReason, enforcePlatform);
	execute_test_when_IAPManager_receives_OSMessage_then_it_returns_correct_values_via_the_query_interface(iapResult, expectedReturnState, expectedFailureReason, enforcePlatform);
}

TEST(When_IAPManager_on_ios_receives_item_purchased_OSMessage_then_return_state_purchased_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_OK, IAPManager::RETURN_STATE_PURCHASED, IAPManager::FAILURE_REASON_NONE, PLATFORM_ID_IOS);
}

TEST(When_IAPManager_on_ios_receives_item_already_purchased_OSMessage_then_return_state_already_purchased_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_OK_ALREADY_PURCHASED, IAPManager::RETURN_STATE_ALREADY_PURCHASED, IAPManager::FAILURE_REASON_NONE, PLATFORM_ID_IOS);
}

TEST(When_IAPManager_receives_user_canceled_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_USER_CANCELED, IAPManager::RETURN_STATE_FAILED, IAPManager::FAILURE_REASON_USER_CANCELED);
}

TEST(When_IAPManager_receives_service_unavailable_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_SERVICE_UNAVAILABLE, IAPManager::RETURN_STATE_FAILED);
}

TEST(When_IAPManager_receives_billing_unavailable_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_BILLING_UNAVAILABLE, IAPManager::RETURN_STATE_FAILED);
}

TEST(When_IAPManager_receives_item_unavailable_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_ITEM_UNAVAILABLE, IAPManager::RETURN_STATE_FAILED);
}

TEST(When_IAPManager_receives_developer_error_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_DEVELOPER_ERROR, IAPManager::RETURN_STATE_FAILED);
}

TEST(When_IAPManager_receives_error_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_ERROR, IAPManager::RETURN_STATE_FAILED);
}


TEST(When_IAPManager_on_android_receives_purchased_OSMessage_then_item_is_reported_as_purchased)
{
	SetEmulatedPlatformID(PLATFORM_ID_ANDROID);

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPItemStateMessageToIAPManager(PURCHASED);

	CheckTrue(GetApp()->m_IAPManager.IsItemPurchased(testItem));
}

TEST(When_IAPManager_on_android_receives_end_of_list_OSMessage_then_IAPManager_adds_a_correct_OSMessage)
{
	SetEmulatedPlatformID(PLATFORM_ID_ANDROID);

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPItemStateMessageToIAPManager(END_OF_LIST);

	CheckTrue(isOSMessageInQueue(OSMessage::MESSAGE_IAP_PURCHASE, true));
}

TEST(When_IAPManager_on_android_receives_end_of_list_after_purchased_OSMessage_then_return_state_already_purchased_is_signaled_and_returned_via_the_query_interface)
{
	SetEmulatedPlatformID(PLATFORM_ID_ANDROID);

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPItemStateMessageToIAPManager(PURCHASED);
	sendIAPItemStateMessageToIAPManager(END_OF_LIST);

	checkSignal(IAPManager::RETURN_STATE_ALREADY_PURCHASED);
	checkQueryAPI(IAPManager::RETURN_STATE_ALREADY_PURCHASED);
}

TEST(When_IAPManager_on_android_receives_purchased_after_end_of_list_OSMessage_then_return_state_purchased_is_signaled_and_returned_via_the_query_interface)
{
	SetEmulatedPlatformID(PLATFORM_ID_ANDROID);

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPItemStateMessageToIAPManager(END_OF_LIST);
	sendIAPItemStateMessageToIAPManager(PURCHASED);

	checkSignal(IAPManager::RETURN_STATE_PURCHASED);
	checkQueryAPI(IAPManager::RETURN_STATE_PURCHASED);
}

TEST(When_IAPManager_on_android_receives_canceled_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	SetEmulatedPlatformID(PLATFORM_ID_ANDROID);

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPItemStateMessageToIAPManager(CANCELED);

	checkSignal(IAPManager::RETURN_STATE_FAILED);
	checkQueryAPI(IAPManager::RETURN_STATE_FAILED);
}

TEST(When_IAPManager_on_android_receives_refunded_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	SetEmulatedPlatformID(PLATFORM_ID_ANDROID);

	GetApp()->m_IAPManager.BuyItem(testItem);
	sendIAPItemStateMessageToIAPManager(REFUNDED);

	checkSignal(IAPManager::RETURN_STATE_FAILED);
	checkQueryAPI(IAPManager::RETURN_STATE_FAILED);
}
#endif

void MenuTestOnSelect(VariantList *pVList)
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();
	Entity *pMenu = pEntClicked->GetParent();

	if (pEntClicked->GetName() == "back")
	{
		GetApp()->m_IAPManager.m_sig_item_purchase_result.disconnect(&MenuTestPurchaseResult);

		KillEntity(pMenu);
		MenuMainCreate(GetEntityRoot());
	}
}

Entity * MenuTestCreate(Entity *pParentEnt)
{
	GetApp()->m_IAPManager.m_sig_item_purchase_result.connect(&MenuTestPurchaseResult);

	Entity *pBG = pParentEnt->AddEntity(new Entity);
	AddFocusIfNeeded(pBG);
	Entity * pButtonEntity;

	float x = 10;
	float y = 20;
	float ySpacer = 55;

	pButtonEntity = CreateTextButtonEntity(pBG, "back", x, y += ySpacer, "Back");
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&MenuTestOnSelect);

#ifdef RT_TESTFW
	ProtonTester::runAllTests();
	ProtonTester::showResultOnScreen(pBG);
#else
	CreateTextLabelEntity(pBG, "warning", x, y += ySpacer, "Tests are not enabled on this platform.\nMaybe the build scripts are not yet updated to include the tests on this platform...");
#endif

	return pBG;
}
