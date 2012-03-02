#ifndef TOUCHDEVICEEMULATORPOINTEREVENTHANDLER_H
#define TOUCHDEVICEEMULATORPOINTEREVENTHANDLER_H

#include "PointerEventHandler.h"
#include "ClanLib-2.0/Sources/API/Core/Math/vec2.h"

/**
 * Emulates a multitouch device with a single pointer device (such as a mouse on a desktop computer).
 *
 * Handles pointer ids 0 and 1 specially. When pointer id 1 is pressed down the position of
 * the pointer becomes a center position for a future multitouch gesture. Afterwards when
 * pointer id 0 is pressed down (and pointer id 1 is still down) a two-touchpoint gesture is
 * started. The second touch point is on the opposite side of the gesture center point.
 * Pinch-zoom and two-finger rotate gestures can be performed with this system.
 *
 * Pointer ids 0 and 1 can translate to the left and right mouse buttons but this doesn't
 * always have to be the case (this class doesn't determine what pointer ids 0 and 1 mean).
 *
 * This class also draws a small marker on the place of the emulated touch point.
 */
class TouchDeviceEmulatorPointerEventHandler : public PointerEventHandler
{
public:
    TouchDeviceEmulatorPointerEventHandler();
	virtual ~TouchDeviceEmulatorPointerEventHandler();

	virtual void handlePointerDownEvent(float x, float y, unsigned int pointerId);
	virtual void handlePointerMoveEvent(float x, float y, unsigned int pointerId);
	virtual void handlePointerUpEvent(float x, float y, unsigned int pointerId);

private:
	enum Mode {
		NONE,
		NORMAL_TOUCH,
		ANTICIPATING_GESTURE,
		EMULATED_GESTURE
	};

	CL_Vec2f getFakeTouchPoint(float x, float y) const;

	Mode mMode;
	CL_Vec2f mFakeGestureCenter;
	bool mSecondaryButtonDown;

	static const unsigned int PRIMARY_POINTER_ID;
	static const unsigned int SECONDARY_POINTER_ID;
};

#endif // TOUCHDEVICEEMULATORPOINTEREVENTHANDLER_H
