#include "TouchDeviceEmulatorPointerEventHandler.h"

#include "BaseApp.h"
#include "Entity/RectRenderComponent.h"
#include "Entity/FocusRenderComponent.h"

const unsigned int TouchDeviceEmulatorPointerEventHandler::PRIMARY_POINTER_ID = 0;
const unsigned int TouchDeviceEmulatorPointerEventHandler::SECONDARY_POINTER_ID = 1;

TouchDeviceEmulatorPointerEventHandler::TouchDeviceEmulatorPointerEventHandler() :
	mMode(NONE),
	mSecondaryButtonDown(false)
{
}

TouchDeviceEmulatorPointerEventHandler::~TouchDeviceEmulatorPointerEventHandler()
{
}

void TouchDeviceEmulatorPointerEventHandler::handlePointerDownEvent(float x, float y, unsigned int pointerId)
{
	switch (pointerId) {
	case PRIMARY_POINTER_ID:
		switch (mMode) {
		case NONE:
			mMode = NORMAL_TOUCH;
			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, x, y, PRIMARY_POINTER_ID);
			break;

		case ANTICIPATING_GESTURE:
			mMode = EMULATED_GESTURE;

			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, x, y, PRIMARY_POINTER_ID);
			CL_Vec2f fakePoint(getFakeTouchPoint(x, y));
			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, fakePoint.x, fakePoint.y, SECONDARY_POINTER_ID);

			//Entity* marker = CreateOverlayRectEntity(GetBaseApp()->GetEntityRoot(), fakePoint, CL_Vec2f(10, 10), MAKE_RGBA(150, 240, 150, 128));
			Entity* marker = GetBaseApp()->GetEntityRoot()->AddEntity(new Entity("FakeTouchMarker"));
			marker->AddComponent(new RectRenderComponent);
			marker->GetVar("pos2d")->Set(fakePoint);
			marker->GetVar("size2d")->Set(CL_Vec2f(10, 10));
			marker->GetVar("color")->Set(MAKE_RGBA(150, 240, 150, 128));
			marker->AddComponent(new FocusRenderComponent);
			break;
		}

		break;

	case SECONDARY_POINTER_ID:
		if (mMode == NONE) {
			mMode = ANTICIPATING_GESTURE;
			mFakeGestureCenter.x = x;
			mFakeGestureCenter.y = y;
		}
		mSecondaryButtonDown = true;
		break;

	default:
		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, x, y, pointerId);
		break;
	}
}

void TouchDeviceEmulatorPointerEventHandler::handlePointerMoveEvent(float x, float y, unsigned int pointerId)
{
	switch (mMode) {
	case NORMAL_TOUCH:
		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, x, y, PRIMARY_POINTER_ID);
		break;

	case EMULATED_GESTURE:
		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, x, y, PRIMARY_POINTER_ID);
		CL_Vec2f fakePoint(getFakeTouchPoint(x, y));
		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, fakePoint.x, fakePoint.y, SECONDARY_POINTER_ID);

		GetBaseApp()->GetEntityRoot()->GetEntityByName("FakeTouchMarker")->GetVar("pos2d")->Set(fakePoint);
		break;
	}
}

void TouchDeviceEmulatorPointerEventHandler::handlePointerUpEvent(float x, float y, unsigned int pointerId)
{
	switch (pointerId) {
	case PRIMARY_POINTER_ID:
		switch (mMode) {
		case NORMAL_TOUCH:
			mMode = NONE;
			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, x, y, PRIMARY_POINTER_ID);
			break;

		case EMULATED_GESTURE:
			CL_Vec2f fakePoint(getFakeTouchPoint(x, y));

			// Randomize the order in which the pointers get lifted
			if (GetBaseApp()->GetTick() & 1) {
				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, x, y, PRIMARY_POINTER_ID);
				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, fakePoint.x, fakePoint.y, SECONDARY_POINTER_ID);
			} else {
				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, fakePoint.x, fakePoint.y, SECONDARY_POINTER_ID);
				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, x, y, PRIMARY_POINTER_ID);
			}

			GetBaseApp()->GetEntityRoot()->RemoveEntityByName("FakeTouchMarker");

			if (mSecondaryButtonDown) {
				mMode = ANTICIPATING_GESTURE;
			} else {
				mMode = NONE;
			}
			break;
		}

		break;

	case SECONDARY_POINTER_ID:
		if (mMode == ANTICIPATING_GESTURE) {
			mMode = NONE;
		}
		mSecondaryButtonDown = false;
		break;

	default:
		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, x, y, pointerId);
		break;
	}
}

CL_Vec2f TouchDeviceEmulatorPointerEventHandler::getFakeTouchPoint(float x, float y) const
{
	CL_Vec2f realPoint(x, y);
	return CL_Vec2f(realPoint + (mFakeGestureCenter - realPoint) * 2);
}
