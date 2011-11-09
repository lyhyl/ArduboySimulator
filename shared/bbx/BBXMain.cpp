/*
 * main.cpp
 *
 *  Created on: Oct 26, 2011
 *      Author: User
 */

#include <assert.h>
#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/dialog.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "bbutil.h"

#include <string>
#include "App.h"

using namespace std;
int exit_application = 0;

static screen_context_t screen_cxt;
//Query g_primaryGLX and g_primaryGLY of the window surface created by utility code
 EGLint g_primaryGLX, g_primaryGLY;

 int GetPrimaryGLX() {return g_primaryGLX;}
 int GetPrimaryGLY() {return g_primaryGLY;}
 bool g_landScapeNoNeckHurtMode = false;
 bool g_leftMouseButtonDown = false;

void handleScreenEvent(bps_event_t *event)
{
    screen_event_t screen_event = screen_event_get_event(event);

    int screen_val;
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

    switch (screen_val) {
    case SCREEN_EVENT_MTOUCH_TOUCH:
    case SCREEN_EVENT_MTOUCH_MOVE:
    case SCREEN_EVENT_MTOUCH_RELEASE:
        break;
    }
}

int initialize()
{

	eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &g_primaryGLX);
    eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &g_primaryGLY);

	EGLint err = eglGetError();
	if (err != 0x3000) {
		fprintf(stderr, "Unable to querry egl surface dimensions\n");
		return EXIT_FAILURE;
	}

	 /*
    glShadeModel(GL_SMOOTH);

    //set clear color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glViewport(0, 0, g_primaryGLX, g_primaryGLY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrthof(0.0f, (float)(g_primaryGLX) / (float)(g_primaryGLY), 0.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef((float)(g_primaryGLX) / (float)(g_primaryGLY) / 2, 0.5f, 0.0f);
*/
	return EXIT_SUCCESS;
}

void ProcessEvents()
{

	    while (true)
	    {
	        bps_event_t* event = NULL;
	        int rc = bps_get_event(&event, 0);

	        assert(BPS_SUCCESS == rc);
	        if (rc != BPS_SUCCESS)
	        {
	         LogMsg("HUH?!");
	            break;
	        }

	        if (event == NULL)
	        {
	            // No more events in the queue
	            break;
	        }

	        int domain = bps_event_get_domain(event);
	        int code = bps_event_get_code(event);

	        if (navigator_get_domain() == domain)
	        {
	        	LogMsg("Got nav msg: %d", code);

	            switch(code)
	            {

	            case NAVIGATOR_ORIENTATION_CHECK:

	            	LogMsg("Device rotated, but telling device to ignore it.  Change later?");
	            	navigator_orientation_check_response(event, false); //tell it we won't rotate.. for now

	            	break;
	            case NAVIGATOR_EXIT:
	            	LogMsg("Leaving BBX app");
	            	exit_application = 1;

	            	break;

	            case NAVIGATOR_WINDOW_STATE:
	            {

	            	navigator_window_state_t winState = navigator_event_get_window_state(event);

	            	switch (winState)
	            	{
	            	case NAVIGATOR_WINDOW_FULLSCREEN:
	            		LogMsg("Full screen");

	            		break;
	            	case NAVIGATOR_WINDOW_THUMBNAIL:
	            		          LogMsg("App thumbnailed");

	            		            		break;

	            	case NAVIGATOR_WINDOW_INVISIBLE:
	            		LogMsg("App in background");
	            		break;

	            	}
	            }

	            	break;

	            case NAVIGATOR_WINDOW_INACTIVE:
	               // m_isPaused = true;
	               // m_handler->onPause();
		               LogMsg("Window unactive");
		           	GetBaseApp()->OnEnterBackground();
		               break;
	            case NAVIGATOR_WINDOW_ACTIVE:
	               LogMsg("Window active");
	               GetBaseApp()->OnEnterForeground();
	               break;
	            }

	        } else if (screen_get_domain() == domain)
	        {
	            screen_event_t screenEvent = screen_event_get_event(event);
	            int screenEventType;
	            int screenEventPosition[2];
	            int screenTouchID;

	            screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_TYPE, &screenEventType);
	            screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_SOURCE_POSITION, screenEventPosition);

	           screenTouchID = 0;
	            // screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_TOUCH_ID,  &screenTouchID);

	         //  LogMsg("Input type: %d, (touchid %d)", screenEventType, screenTouchID);

	            if (screenEventType == SCREEN_EVENT_MTOUCH_TOUCH)
	            {
	            	screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_TOUCH_ID,  &screenTouchID);
	            	LogMsg("Touched");
	        	  	float xPos = screenEventPosition[0];
            	    float yPos = screenEventPosition[1];
            	    ConvertCoordinatesIfRequired(xPos, yPos);
        			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, xPos, yPos, screenTouchID);


	            	//    m_handler->onLeftPress(static_cast<float>(screenEventPosition[0]), static_cast<float>(screenEventPosition[1]));
	            } else if (screenEventType == SCREEN_EVENT_MTOUCH_RELEASE)
	            {
	            	  screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_TOUCH_ID,  &screenTouchID);
	            	   LogMsg("Touch release");
	            	 	float xPos = screenEventPosition[0];
	            	    float yPos = screenEventPosition[1];
	            	    ConvertCoordinatesIfRequired(xPos, yPos);
	            	    GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, xPos, yPos, screenTouchID);
    //    m_handler->onLeftRelease(static_cast<float>(screenEventPosition[0]), static_cast<float>(screenEventPosition[1]));
	            } else if (screenEventType == SCREEN_EVENT_MTOUCH_MOVE)
	            {
	            	screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_TOUCH_ID,  &screenTouchID);

	            	float xPos = screenEventPosition[0];
	            	    float yPos = screenEventPosition[1];
	            	    ConvertCoordinatesIfRequired(xPos, yPos);
	            	    GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, xPos, yPos, screenTouchID);
    //    m_handler->onLeftRelease(static_cast<float>(screenEventPosition[0]), static_cast<float>(screenEventPosition[1]));
	            } else if (screenEventType == SCREEN_EVENT_POINTER)
	            {
	                int pointerButton;
	                screen_get_event_property_iv(screenEvent, SCREEN_PROPERTY_BUTTONS, &pointerButton);
		          //	   LogMsg("PointerType: %d - ", screenEventType);

	                if (pointerButton == SCREEN_LEFT_MOUSE_BUTTON)
	                {
	              	  	float xPos = screenEventPosition[0];
	            	    float yPos = screenEventPosition[1];
	            	    ConvertCoordinatesIfRequired(xPos, yPos);


	                	if (!g_leftMouseButtonDown)
	                	{

	                     	g_leftMouseButtonDown = true;
	          	  			LogMsg("Mouse down at %.2f, %.2f", xPos, yPos);
	            			GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_START, xPos, yPos, screenTouchID);


	                	} else
	                	{
	          	  			LogMsg("Mouse move at %.2f, %.2f", xPos, yPos);
	          				GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_MOVE, xPos, yPos, screenTouchID);
	                	}


	                 } else
	                {
	                	  //well, it's the mouse moving or the button was released.  No way to tell which one in the
	                	  //current API?
	                  	if (g_leftMouseButtonDown)
	                  	{
	                  		LogMsg("Releasing mouse button");
	                  		g_leftMouseButtonDown = false;
	                   	  	float xPos = screenEventPosition[0];
	                	    float yPos = screenEventPosition[1];
	                	    ConvertCoordinatesIfRequired(xPos, yPos);
	                		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_GUI_CLICK_END, xPos, yPos, screenTouchID);
	                  	} else
	                  	{

	                  		//LogMsg("Uh, already released, so what is this, a mouse move? ",pointerButton );

	                  	}

	                }
	            }
	        } else if (dialog_get_domain() == domain)
	        {

	        	/*

	        	 /if (DIALOG_RESPONSE == code)
	            {
	             //   ASSERT(m_promptInProgress);
	              //  m_promptInProgress = false;
	                //m_handler->onPromptOk(dialog_event_get_prompt_input_field(event));
	            	LogMsg("Got dialog response %d", code);
	            }
	            */
	        } else {
	           LogMsg("Unrecognized and unrequested event! domain: %d, code: %d", domain, code);
	        }
	    }

}

int main(int argc, char *argv[])
{
    int rc;

    //Create a screen context that will be used to create an EGL surface to to receive libscreen events
    screen_create_context(&screen_cxt, 0);

    //Initialize BPS library
	bps_initialize();

	//Use utility code to initialize EGL for 2D rendering with GL ES 1.1
	if (EXIT_SUCCESS != bbutil_init_egl(screen_cxt, GL_ES_1, AUTO)) {
		fprintf(stderr, "bbutil_init_egl failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	//Initialize application logic
	if (EXIT_SUCCESS != initialize()) {
		fprintf(stderr, "initialize failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	//Signal BPS library that navigator and screen events will be requested
	if (BPS_SUCCESS != screen_request_events(screen_cxt)) {
		fprintf(stderr, "screen_request_events failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	if (BPS_SUCCESS != navigator_request_events(0)) {
		fprintf(stderr, "navigator_request_events failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	 navigator_rotation_lock(true);
	 bbutil_init_egl(screen_cxt, GL_ES_1, LANDSCAPE);

	 /*
	//Signal BPS library that navigator orientation is not to be locked
	if (BPS_SUCCESS != navigator_rotation_lock(false)) {
		fprintf(stderr, "navigator_rotation_lock failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	*/


	//get proton going

	/*
	if (g_landScapeNoNeckHurtMode || GetOrientation() == ORIENTATION_PORTRAIT)
	{
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
	} else
	{
		SetupScreenInfo(GetPrimaryGLY(), GetPrimaryGLX(), ORIENTATION_LANDSCAPE_LEFT);
	}
	*/

	//Well, the emualtor is starting in landscape, let's do this for now

	//SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_LANDSCAPE_LEFT);

	if (!GetBaseApp()->Init())
	{
		LogMsg("Proton failed to init");
		return 0;
	}

	if (GetFakePrimaryScreenSizeX() == 0)
	{
		//without this, RTBasicApp never sets up its screen.  But RTSimpleApp does, so we don't want this happening with that.
		//Waiting to test on a real device to really nail down rotation/portait mode, it's sort of hardcoded to landscape now.. I think..
		SetupScreenInfo(GetPrimaryGLX(), GetPrimaryGLY(), ORIENTATION_PORTRAIT);
	}

   while(exit_application == 0)
    {
    	//Request and process BPS next available event

	    ProcessEvents();

	    if (!GetBaseApp()->IsInBackground())
	    {
			GetBaseApp()->Update();
			GetBaseApp()->Draw();
			bbutil_swap();
	    }

    	while (!GetBaseApp()->GetOSMessages()->empty())
    		{
    			OSMessage m = GetBaseApp()->GetOSMessages()->front();
    			GetBaseApp()->GetOSMessages()->pop_front();
#ifdef _DEBUG
    			LogMsg("Got OS message %d, %s", m.m_type, m.m_string.c_str());
#endif

    			switch (m.m_type)
    			{
    			case OSMessage::MESSAGE_CHECK_CONNECTION:
    				//pretend we did it
    				GetMessageManager()->SendGUI(MESSAGE_TYPE_OS_CONNECTION_CHECKED, (float)RT_kCFStreamEventOpenCompleted, 0.0f);
    				break;
    			case OSMessage::MESSAGE_OPEN_TEXT_BOX:

    				LogMsg("TODO: Open keyboard");
    				break;

    			case OSMessage::MESSAGE_CLOSE_TEXT_BOX:
    				LogMsg("TODO: Close keyboard");

    				SetIsUsingNativeUI(false);
    				break;

    			case OSMessage::MESSAGE_SET_FPS_LIMIT:
    				LogMsg("Todo: FPS limit");
    				//fpsTimerLoopMS = (int) (1000.0f/m.m_x);
    				break;

    			case OSMessage::MESSAGE_SET_ACCELEROMETER_UPDATE_HZ:

    				//well, it's hardcoded at 30 by the pdk I guess, but we can still enable/disable it
    				LogMsg("TODO: Accel setup");
    				break;
    			}
    		}
    }

    //Stop requesting events from libscreen
    screen_stop_events(screen_cxt);

    //Shut down BPS library for this process
    bps_shutdown();

    //Use utility code to terminate EGL setup
    bbutil_terminate();

    //Destroy libscreen context
    screen_destroy_context(screen_cxt);
    return 0;
}
