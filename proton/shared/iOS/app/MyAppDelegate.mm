//
//  MyAppDelegate.m
//  Created by Seth Robinson on 3/6/09.
//  For license info, check the license.txt file that should have come with this.
//


#import "MyAppDelegate.h"
#import "EAGLView.h"


@implementation MyAppDelegate

@synthesize window;
@synthesize viewController;
@synthesize glView;

- (void) KillNetInit
{
	//LogMsg("Killing net init");
	 if (m_ReadRef) 
	 {
       // CFReadStreamUnscheduleFromRunLoop(m_ReadRef, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
        //(void) CFReadStreamSetClient(info->readStream, kCFStreamEventNone, NULL, NULL);
        
        /* CFReadStreamClose terminates the stream. */
        CFReadStreamClose(m_ReadRef);
        CFRelease(m_ReadRef);
        m_ReadRef = NULL;
    }

    if (m_WriteRef) 
	{
       // CFWriteStreamUnscheduleFromRunLoop(info->writeStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
       // (void) CFWriteStreamSetClient(info->writeStream, kCFStreamEventNone, NULL, NULL);
        
        /* CFWriteStreamClose terminates the stream. */
        
        CFWriteStreamClose(m_WriteRef);
        CFRelease(m_WriteRef);
		m_WriteRef = NULL;
    }
	
	m_HostRef = NULL;
	
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
	
	[window addSubview:viewController.view];
	[window makeKeyAndVisible];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];

	//we don't want to waste cycles doing the accelerometer so we half disable it
	[[UIAccelerometer sharedAccelerometer] setDelegate:nil];
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:1.0f];

	//[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / pMsg->m_x)];
	//[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	

	_textField = NULL;
	m_inputboxMaxLen = 10;
	m_requestedKeyboardType = UIKeyboardTypeASCIICapable; //default
	m_HostRef = NULL;
	m_ReadRef = NULL;
	m_WriteRef = NULL;
	
}

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	//LogMsg("%.2f, %.2f, %.2f", acceleration.x, acceleration.y, acceleration.z);
	GetMessageManager()->SendGUI(MESSAGE_TYPE_GUI_ACCELEROMETER,  Variant(acceleration.x, acceleration.y, acceleration.z));
	
	//Use a basic low-pass filter to only keep the gravity in the accelerometer values
	//accel[0] = acceleration.x * kFilteringFactor + accel[0] * (1.0 - kFilteringFactor);
	//accel[1] = acceleration.y * kFilteringFactor + accel[1] * (1.0 - kFilteringFactor);
	//accel[2] = acceleration.z * kFilteringFactor + accel[2] * (1.0 - kFilteringFactor);
}

- (void)OnOpenTextBox: (OSMessage *)pMsg
{
	
	[self KillTextBox];
	LogMsg("OnOpenTextBox: Setting up textbox");
	m_inputboxMaxLen =  pMsg->m_parm1;
	
	//LogMsg("Text is %s, len is %d.  x is %.2f, y is %.2f", pMsg->m_string.c_str(), pMsg->m_parm1, xPos, yPos);
		
	//Create and editable text field. This is used only when the user successfully lands the rocket.
	_textField = [[UITextField alloc] initWithFrame:CGRectMake( pMsg->m_x,  pMsg->m_y, pMsg->m_sizeX, pMsg->m_sizeY)];
	[_textField setDelegate:self];
	[_textField setBackgroundColor:[UIColor colorWithWhite:0.0 alpha:0.5]];
	[_textField setTextColor:[UIColor whiteColor]];
	[_textField setFont:[UIFont fontWithName:@"Arial" size:pMsg->m_fontSize]];
	[_textField setPlaceholder:@""];	
	[_textField setText:[NSString stringWithCString: pMsg->m_string.c_str() encoding: [NSString defaultCStringEncoding]]];
	[_textField setAutocorrectionType: UITextAutocorrectionTypeNo];
	[_textField setClearsOnBeginEditing: NO];
	
	switch (pMsg->m_parm2)
	{
		case OSMessage::PARM_KEYBOARD_TYPE_NUMBERS:
		m_requestedKeyboardType = UIKeyboardTypeNumbersAndPunctuation;
		break;
		
		case OSMessage::PARM_KEYBOARD_TYPE_URL:
		m_requestedKeyboardType = UIKeyboardTypeURL;
		break;
		
		case OSMessage::PARM_KEYBOARD_TYPE_EMAIL:
		m_requestedKeyboardType = UIKeyboardTypeEmailAddress;
		break;
	
	default:
		m_requestedKeyboardType = UIKeyboardTypeASCIICapable;
	}
	
	[_textField setKeyboardType: m_requestedKeyboardType]; 
	
	[_textField setTextAlignment: UITextAlignmentLeft];
	[_textField setBorderStyle: UITextBorderStyleBezel];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(OnTextChangedNotification:) name:@"UITextFieldTextDidChangeNotification" object:_textField];
	[viewController.view addSubview:_textField];
	[_textField becomeFirstResponder];  //make the keyboard pop up now
	SetIsUsingNativeUI(true);
	
}
- (void) keyboardWillShow:(NSNotification *) notification
{
   
   LogMsg("Keyboarding showing");
    UIView *keyboardView = nil;
 
    NSArray *windows = [[UIApplication sharedApplication] windows];
 
    for (int i = 0; i < [windows count]; ++i) 
	{
         UIWindow *myWindow =  [windows objectAtIndex:i];
 
        for (int j = 0; j < [myWindow.subviews count]; ++j) 
		{
            keyboardView = [myWindow.subviews objectAtIndex:j];
 
            if ([[keyboardView description] hasPrefix:@"<UIKeyboard"] == YES)
			{
                if (m_inputboxMaxLen == 0)
				{
                LogMsg("Making keyboard hidden");
                    keyboardView.hidden = YES;
                    keyboardView.userInteractionEnabled = NO;
                } else
				{
                    keyboardView.hidden = NO;
                    keyboardView.userInteractionEnabled = YES;
                }
                return;
            }
        }
    }
}
- (void)OnTextChangedNotification:(NSNotification *)note 
{
    
   // NSLog(_textField.text);
    
    string  s = string([ _textField.text cStringUsingEncoding:NSUTF8StringEncoding]);
	SetLastStringInput(s);
	//LogMsg("Did encoding");
	
    //LogMsg(s.c_str());
    //_textField.text = [NSString stringWithCString: s.c_str() encoding: [NSString defaultCStringEncoding]];
    //if ([[myTextField stringValue] length] > limit) {
      //  [myTextField setStringValue:[[myTextField stringValue] substringToIndex:limit]];
   
}

- (void) KillTextBox
{
	if (_textField != NULL)
	{
		//LogMsg("killing text box");
	
		[[NSNotificationCenter defaultCenter] removeObserver:self name:@"UITextFieldTextDidChangeNotification" object:_textField];
		[_textField endEditing:YES];
		[_textField removeFromSuperview];
		_textField = NULL;
		SetIsUsingNativeUI(false);
	}	
}

- (void)onOSMessage:(OSMessage *)pMsg
{
	
	switch (pMsg->m_type)
	{
	
		case OSMessage::MESSAGE_OPEN_TEXT_BOX:
			LogMsg("Textbox msg");
			[self OnOpenTextBox:pMsg];
			break;
		
		case OSMessage::MESSAGE_CHECK_CONNECTION:
			[self ActivateNetworkConnection];
			break;
		
		case OSMessage::MESSAGE_CLOSE_TEXT_BOX:
			[self KillTextBox];
			break;
		case OSMessage::MESSAGE_SET_FPS_LIMIT:
			glView.animationIntervalSave = 1.0/pMsg->m_x;
			break;
		case OSMessage::MESSAGE_SET_ACCELEROMETER_UPDATE_HZ:
			if (pMsg->m_x == 0)
			{
				//disable it
				[[UIAccelerometer sharedAccelerometer] setDelegate:nil];
				[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0)];
			} else
			{
				LogMsg("Enabling acceler with %.2f", pMsg->m_x);
				[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / pMsg->m_x)];
				[[UIAccelerometer sharedAccelerometer] setDelegate:self];
				
			}
			break;
			
		default:
			LogMsg("Error, unknown message type: %d", pMsg->m_type);
	}
}

#define LEGAL	@"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-0123456789"
#define LEGAL_FULL	@"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz~`1234567890!@#$%^&*()_+-=[]{}|;': ,.<>/?\\\""

#define LEGAL_NUM	@"-0123456789."

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	//NSLog(string);
	//NSLog(textField.text);
	
	if (textField.text.length >= m_inputboxMaxLen && range.length == 0)
	{
		return NO; // return NO to not change text
	}else{
		
		
		NSCharacterSet *cs;
		switch (m_requestedKeyboardType)
		{
		
			case UIKeyboardTypeNumbersAndPunctuation:
				cs = [[NSCharacterSet characterSetWithCharactersInString:LEGAL_NUM] invertedSet];
			break;
			
			case UIKeyboardTypeASCIICapable:
				cs = [[NSCharacterSet characterSetWithCharactersInString:LEGAL] invertedSet];
				break;
			
			
			default:
			cs = [[NSCharacterSet characterSetWithCharactersInString:LEGAL_FULL] invertedSet];

		}
		
		NSString *filtered = [[string componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
		return [string isEqualToString:filtered];
	}

	return YES;
}

// Saves the user name and score after the user enters it in the provided text field. 
- (void)textFieldDidEndEditing:(UITextField*)textField 
{
	//Save name
	//NSLog([textField text]);
	//SetLastStringInput([[textField text] UTF8String]);
	
	if (_textField != NULL)
	{
		[_textField endEditing:YES];
		[self KillTextBox];
	}

}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	[textField setPlaceholder:@""];	
}

// Terminates the editing session
- (BOOL)textFieldShouldReturn:(UITextField*)textField
{
	//Terminate editing
	[textField resignFirstResponder];
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application 
{
	[glView stopAnimation];
	//	glView.animationInterval = 1.0 / 5.0;
	
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[glView onKill];
}

- (void)applicationDidBecomeActive:(UIApplication *)application 
{
	glView.animationInterval = glView.animationIntervalSave;
	[glView startAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	GetBaseApp()->OnEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	GetBaseApp()->OnEnterForeground();
}

- (void)ActivateNetworkConnection
{
	
	[self KillNetInit];
	
	m_HostRef = CFHostCreateWithName(nil, (CFStringRef)@"rtsoft.com");
	CFStreamCreatePairWithSocketToCFHost(nil, m_HostRef, 80, &m_ReadRef, &m_WriteRef);
	memset(&context, 0, sizeof(context));
	context.version = 0;
	context.info = self;
	CFWriteStreamSetClient(m_WriteRef,
						   kCFStreamEventOpenCompleted | kCFStreamEventErrorOccurred,
						   MyWriteStreamCallback,
						   &context);
	
	// put the write ref on the runloop
	CFWriteStreamScheduleWithRunLoop(m_WriteRef,
									 [[NSRunLoop currentRunLoop] getCFRunLoop],
									 (CFStringRef)NSDefaultRunLoopMode);
	
	CFWriteStreamOpen(m_WriteRef);
}


- (void)dealloc 
{
	[window release];
	[super dealloc];
}

@end

static void MyWriteStreamCallback(CFWriteStreamRef streamRef,
								  CFStreamEventType eventType,
								  void *clientCallBackInfo)

{
	
    MyAppDelegate *v = (MyAppDelegate *)clientCallBackInfo;
	
    if (eventType == kCFStreamEventOpenCompleted)
    {
		// at this point, the connection is up.  It's possible this is wifi, or EDGE/3G
        // and BSD socket functions will work.
    }
    //LogMsg("Connection is ready: %d", eventType);
    [v KillNetInit];
	GetMessageManager()->SendGUI(MESSAGE_TYPE_OS_CONNECTION_CHECKED, eventType, 0);		
    CFRunLoopStop(CFRunLoopGetCurrent());
}
