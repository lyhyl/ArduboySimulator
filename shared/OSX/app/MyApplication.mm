//
//  MyApplication.m
//  Created by Seth Robinson on 12/13/10.

#import "MyApplication.h"
#import "BaseApp.h"

bool g_shiftDown = false;
bool g_commandDown = false;
bool g_optionDown = false;
bool g_controlDown = false;

@implementation MyApplication


- (void)sendControlKeyMessage: (int) key keyDown: (bool) keyDown
{
	if (!GetBaseApp()->IsInitted()) return;
	
//	LogMsg("Sending key %d as %d", key, int(keyDown));
	if ( keyDown)
	{
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW,   key, 1);  	
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR,   key, 0);  	
		
	} else
	{
		GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_CHAR_RAW,   key, 0);  	
	}
	
}

- (void)sendEvent:(NSEvent *)anEvent
{
	//This works around an AppKit bug, where key up events while holding
	//down the command key don't get sent to the key window.
	
	/*	
	 if([anEvent type] == NSKeyUp && ([anEvent modifierFlags] & NSCommandKeyMask))
	 {
	 [[self keyWindow] sendEvent:anEvent];
	 }else{
	 [super sendEvent:anEvent];
	 }
	 */
	
	if ([anEvent type] == NSFlagsChanged)
	{
		bool bKeyDown;
		
		switch ([anEvent keyCode])
		{
		case 54:
		case 55:
				bKeyDown = [anEvent modifierFlags]&NSCommandKeyMask;
				if (bKeyDown != g_commandDown)
				{
					g_commandDown = bKeyDown;
					[self sendControlKeyMessage: VIRTUAL_KEY_COMMAND keyDown: bKeyDown];
				}
				break;

			case 56:
			case 60:
				bKeyDown = [anEvent modifierFlags]&NSShiftKeyMask;
				if (bKeyDown != g_shiftDown)
				{
					g_shiftDown = bKeyDown;
					[self sendControlKeyMessage: VIRTUAL_KEY_SHIFT keyDown: bKeyDown];
				}
				break;
				
			case 59:
			case 62:
				bKeyDown = [anEvent modifierFlags]&NSControlKeyMask;
				if (bKeyDown != g_controlDown)
				{
					g_controlDown = bKeyDown;
					[self sendControlKeyMessage: VIRTUAL_KEY_CONTROL keyDown: bKeyDown];
				}
				break;
	
			case 58:
			case 61:
				bKeyDown = [anEvent modifierFlags]&NSAlternateKeyMask;
				if (bKeyDown != g_optionDown)
				{
					g_optionDown = bKeyDown;
					[self sendControlKeyMessage: VIRTUAL_KEY_ALT keyDown: bKeyDown];
				}
				break;
				
				
		}
			
	}
	
	//LogMsg("Event: %d: Ctrl: %d", [anEvent type], int(bCtrlDown));
	[super sendEvent:anEvent];
}

@end
