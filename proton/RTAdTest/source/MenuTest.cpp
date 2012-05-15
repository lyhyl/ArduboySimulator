#include "MenuTest.h"
#include "MenuMain.h"

#include "Entity/EntityUtils.h"
#include "Manager/MessageManager.h"


unsigned int numSignals = 0;
IAPManager::eReturnState sigReturnState = IAPManager::RETURN_STATE_NONE;
string sigExtraData;
string sigItemId;

void MenuTestPurchaseResult(VariantList *vList)
{
	++numSignals;
	sigReturnState = IAPManager::eReturnState(vList->Get(0).GetUINT32());
	sigExtraData = vList->Get(1).GetString();
	sigItemId = vList->Get(2).GetString();
}

#ifdef RT_TESTFW
#include "testfw/ProtonTester.h"
#include "testfw/ProtonTesterGUI.h"

void initTest()
{
	GetApp()->m_IAPManager.Reset();
}

void cleanTest()
{
	GetApp()->m_IAPManager.Reset();

	numSignals = 0;
	sigReturnState = IAPManager::RETURN_STATE_NONE;
	sigExtraData.clear();
	sigItemId.clear();
}

const string testItem("testItem");
const string extraData("extraData");

void sendIAPResultMessageToIAPManager(IAPManager::ResponseCode responseCode)
{
	Message msg(MESSAGE_CLASS_GUI, TIMER_SYSTEM, MESSAGE_TYPE_IAP_RESULT);
	msg.SetParm1(responseCode);
	msg.SetStringParm(extraData);
	GetApp()->m_IAPManager.OnMessage(msg);
}

TEST(When_requesting_to_buy_an_item_then_IAPManager_adds_a_correct_OSMessage)
{
	if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
	{
		initTest();

		GetApp()->m_IAPManager.BuyItem(testItem);

		bool wantedMessageFound = false;

		deque<OSMessage> *msgs = GetBaseApp()->GetOSMessages();
		for (deque<OSMessage>::const_iterator it(msgs->begin()); it != msgs->end(); it++)
		{
			if ((*it).m_type == OSMessage::MESSAGE_IAP_PURCHASE && (*it).m_string == testItem)
			{
				wantedMessageFound = true;
				break;
			}
		}

		CheckTrue(wantedMessageFound);

		cleanTest();
	}
}

void execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters(IAPManager::ResponseCode iapResult, IAPManager::eReturnState expectedReturnState)
{
	if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
	{
		initTest();

		GetApp()->m_IAPManager.BuyItem(testItem);
		sendIAPResultMessageToIAPManager(iapResult);

		CheckEq((unsigned int)1, numSignals);
		CheckEq(expectedReturnState, sigReturnState);
		CheckEq(extraData, sigExtraData);
		CheckEq(testItem, sigItemId);

		cleanTest();
	}
}

void execute_test_when_IAPManager_receives_OSMessage_then_it_returns_correct_values_via_the_query_interface(IAPManager::ResponseCode iapResult, IAPManager::eReturnState expectedReturnState)
{
	if (GetEmulatedPlatformID() != PLATFORM_ID_ANDROID)
	{
		initTest();

		GetApp()->m_IAPManager.BuyItem(testItem);
		sendIAPResultMessageToIAPManager(iapResult);

		CheckEq(expectedReturnState, GetApp()->m_IAPManager.GetReturnState());
		CheckEq(extraData, GetApp()->m_IAPManager.GetExtraData());
		CheckEq(testItem, GetApp()->m_IAPManager.GetItemID());

		cleanTest();
	}
}

void execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::ResponseCode iapResult, IAPManager::eReturnState expectedReturnState)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters(iapResult, expectedReturnState);
	execute_test_when_IAPManager_receives_OSMessage_then_it_returns_correct_values_via_the_query_interface(iapResult, expectedReturnState);
}

TEST(When_IAPManager_receives_item_purchased_OSMessage_then_return_state_purchased_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_OK, IAPManager::RETURN_STATE_PURCHASED);
}

TEST(When_IAPManager_receives_item_already_purchased_OSMessage_then_return_state_already_purchased_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_OK_ALREADY_PURCHASED, IAPManager::RETURN_STATE_ALREADY_PURCHASED);
}

TEST(When_IAPManager_receives_user_canceled_OSMessage_then_return_state_failed_is_signaled_and_returned_via_the_query_interface)
{
	execute_test_when_IAPManager_receives_OSMessage_then_it_sends_a_signal_with_the_correct_parameters_and_returns_correct_values_via_the_query_interface(IAPManager::RESULT_USER_CANCELED, IAPManager::RETURN_STATE_FAILED);
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
