
#ifdef RT_TAPJOY_ENABLED

#import "TapjoyManagerV9.h"
#import <UIKit/UIKit.h>
#import "MyAppDelegate.h"
#import "EAGLView.h"
#import <Tapjoy/TJPlacement.h>

@interface TapjoyManager () <TJCViewDelegate, TJCVideoAdDelegate>
@property (nonatomic, strong) TJDirectPlayPlacementDelegate *dpDelegate;
@property (strong, nonatomic) TJPlacement *m_addPlacement;

@end

@implementation TapjoyManager

- (void) InitTapjoy: (UIApplication *)application viewController: (MyViewController*) viewController
{
	LogMsg("Initting tapjoy");

	//Register tapjoy callbacks
    m_viewController = viewController;
    
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(tjcConnectSuccess:) name:TJC_CONNECT_SUCCESS object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(tjcConnectFail:) name:TJC_CONNECT_FAILED object:nil];

	//[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getFeaturedApp:) name:TJC_FEATURED_APP_RESPONSE_NOTIFICATION object:nil];

    //apparently the above doesn't exist in SDK 10
    
    /*
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getUpdatedPoints:) name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getSpendPoints:) name:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getAwardPoints:) name:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION object:nil];

      
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getUpdatedPointsError:) name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getSpendPointsError:) name:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getAwardPointsError:) name:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR object:nil];

    
    
    // Add an observer for when a user has successfully earned currency.
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showEarnedCurrencyAlert:)
												 name:TJC_TAPPOINTS_EARNED_NOTIFICATION
											   object:nil];

	//Listen for when they close the offerwall
	[[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(offerwallClosed:)
                                                 name:TJC_VIEW_CLOSED_NOTIFICATION
                                               object:nil];

    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showFullscreenAd:) name:TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION object:nil];

     */

	//virtual items hosted by tapjoy not yet not supported
	/*
	// Watch for virtual good notification indicating that items are ready to go.
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(virtualGoodsUpdated:) name:TJC_PURCHASED_ITEMS_RESPONSE_NOTIFICATION object:nil];
	*/

	//TODO: ?
	// If you are not using Tapjoy Managed currency, you would set your own user ID here.
	//[TapjoyConnect setUserID:@"A_UNIQUE_USER_ID"];
   
    _dpDelegate = [[TJDirectPlayPlacementDelegate alloc] init];
    _dpDelegate.tjManager = self;

    
}


- (Boolean) onOSMessage:(OSMessage *)pMsg
{
	switch (pMsg->m_type)
	{
	
	case OSMessage::MESSAGE_TAPJOY_INIT_MAIN:
        {
	#ifdef _DEBUG
        LogMsg("Logging on to tapjoy: %s, %s", pMsg->m_string.c_str(), pMsg->m_string2.c_str());
    #endif
        NSString* appID = [NSString stringWithUTF8String:pMsg->m_string.c_str()];
        NSString* secretKey = [NSString stringWithUTF8String:pMsg->m_string2.c_str()];
            
           //[Tapjoy setDebugEnabled: true];
            
         #ifdef _DEBUG
             [Tapjoy setDebugEnabled: true];
        #endif
            [Tapjoy connect:appID];
            
         
		return true; //we handled it
        }
	break;

	case OSMessage::MESSAGE_TAPJOY_SET_USERID:
	{
	  NSString* userID = [NSString stringWithUTF8String:pMsg->m_string.c_str()];
      [Tapjoy setUserID: userID];

        LogMsg("Setting up TJ");
        
        _m_addPlacement = [TJPlacement placementWithName:@"WatchAd" delegate:_dpDelegate];
        
        [_m_addPlacement requestContent];

	  return true; //we handled it
	}
	break;
	
	case OSMessage::MESSAGE_TAPJOY_GET_AD: 

        LogMsg("Getting tapjoy ad");
        //TODO: Set banner size?
        
       // [Tapjoy getDisplayAdWithDelegate:self];
		return true; //we handled it
	break;
	
	case OSMessage::MESSAGE_TAPJOY_GET_FEATURED_APP:
        {
		//[TapjoyConnect getFeaturedApp];

            //note: ignoring currency ID, placement name must be setup in the Tapjoy V11 website portal for this work
            
            if (_m_addPlacement)
            {
                if(_m_addPlacement.isContentAvailable)
                {
                if (_m_addPlacement.isContentReady)
                {
                    LogMsg("Showing ad");
                [_m_addPlacement showContentWithViewController:m_viewController];
                } else
                {
                    LogMsg("No content to present");
                    GetMessageManager()->SendGUIEx( MESSAGE_TYPE_TAPJOY_NO_CONTENT_TO_PRESENT, 1,0,0);
                    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No video ads to display"
                                                                    message:@"Please try again later, there are no video ads to show you right now."
                                                                   delegate:nil
                                                          cancelButtonTitle:@"OK"
                                                          otherButtonTitles:nil];
                    [alert show];
                    [alert release];

                }
                } else
                {
                    
                    LogMsg("No content available");
                    GetMessageManager()->SendGUIEx( MESSAGE_TYPE_TAPJOY_NO_CONTENT_AVAILABLE, 1,0,0);

                    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No video ads to display"
                                                                   message:@"Please try again later, there are no video ads to show you right now."
                                                                  delegate:nil
                                                         cancelButtonTitle:@"OK"
                                                         otherButtonTitles:nil];
                    [alert show];
                    [alert release];
                }
                
            } else
            {
                LogMsg("Not ready to show another ad");
            
            }
            return true; //we handled it
        }
	break;

	case OSMessage::MESSAGE_TAPJOY_SHOW_FEATURED_APP:
		LogMsg("show tapjoy feature");
		//[TapjoyConnect showFeaturedAppFullScreenAd];
	//	[Tapjoy showFullScreenAd];
            return true; //we handled it
	break;
	
	case OSMessage::MESSAGE_TAPJOY_GET_TAP_POINTS:
		LogMsg("Getting tapjoy points");
	//	[Tapjoy getTapPoints];
		return true; //we handled it
	break;

	case OSMessage::MESSAGE_TAPJOY_SPEND_TAP_POINTS:
		//LogMsg("Spending tappoints: %d" , pMsg->m_parm1);
		[Tapjoy spendCurrency:pMsg->m_parm1];
		return true; //we handled it
	break;	
	
	case OSMessage::MESSAGE_TAPJOY_AWARD_TAP_POINTS:
		// This method call will award 10 virtual currencies to the users total
		[Tapjoy awardCurrency:pMsg->m_parm1];
		return true; //we handled it
	break;

	case OSMessage::MESSAGE_TAPJOY_SHOW_OFFERS:
		{
            // This method returns the offers view that you can add it in your required view. Initialize its bounds depending on the main window bounds.
		//[Tapjoy showOffers];

		// This method takes a view controller and sets its bounds according to viewController's and depend on the orientation of the view controller sent as an argument
            MyAppDelegate * appDelegate = [[UIApplication sharedApplication] delegate];
            
          
            [Tapjoy showOffersWithViewController:appDelegate.viewController];

		// OR

		// This method adds the offers view without the library's Navigation Bar, so you can add this view into your view which already has a navigation bar
		//[TapjoyConnect showOffersWithViewController:vController withInternalNavBar:NO];			break;
		}
            return true; //we handled it
	
		break;
		
	case OSMessage::MESSAGE_TAPJOY_SHOW_AD:
        {
		
		int showAd = (int)pMsg->m_x;
	//	LogMsg("Showing tapjoy ad, parm is: %d" + showAd);
		
            //TODO, add the button to our view controller?
            
            /*
        if (m_adView)
        {
            if (showAd)
            {
                m_adView.hidden = NO;
            } else
            {
                m_adView.hidden = YES;
            }
            
        } else
        {
           LogMsg("Can't set status of ad, its view doesn't exist yet");
                
        }
             */
            
		//[TapjoyConnect getDisplayAdWithDelegate:self];
		
		//TapjoyConnect.getTapjoyConnectInstance().enableBannerAdAutoRefresh(app.tapjoy_ad_show != 0);

		return true; //we handled it
        }
	break;

	case OSMessage::MESSAGE_REQUEST_AD_SIZE:
		m_adBannerWidth = (int)pMsg->m_x;
		m_adBannerWidth = (int)pMsg->m_y;
	
		//app.adBannerWidth = 480;
		//app.adBannerHeight = 72;
		//app.tapBannerSize = app.adBannerWidth+"x"+app.adBannerHeight;		
		LogMsg("Setting tapjoy banner size to %d x %d.. TODO: Not really handled yet!", m_adBannerWidth, m_adBannerWidth);
		return true; //we handled it
	break;
	
	default:
	
	return false; //not handled
	}
	
	return false; //not handled
}


//************* callback handlers for tapjoy

- (void)contentDidDisappear:(TJPlacement *)placement
{
    
  //  LogMsg("Caching another Tapjoy ad");
   // m_addPlacement = [TJPlacement placementWithName:@"WatchAd" delegate:self];
   // [m_addPlacement requestContent];
  
}

- (void) requestDidFail:(TJPlacement *)placement error:(NSError *)error
{
    NSLog(@"Tapjoy request failed: %@", [error localizedDescription]);
}

- (void)getFeaturedApp:(NSNotification*)notifyObj
{
	LogMsg("Got tapjoy featured app");
    // Displays a full screen ad, showing the current featured app. 
    //[TapjoyConnect showFeaturedAppFullScreenAd]; 
    // OR 
    // This is used when you want to add the full screen ad to an existing view controller. 
    //[TapjoyConnect showFeaturedAppFullScreenAdWithViewController:self];
	GetMessageManager()->SendGUIEx(MESSAGE_TYPE_TAPJOY_FEATURED_APP_READY, 1,0,0);

}

// This method is called after Ad data has been successfully received from the server. 
- (void)didReceiveAd:(TJCAdView*)adView
{
		LogMsg("Got ad");
		GetMessageManager()->SendGUIEx(MESSAGE_TYPE_TAPJOY_AD_READY, (int)1,0,0);
    
    //TODO:
   /*
    if (!m_adView)
    {
        //setup our view controller, we'll keep the same one for the rest of the app
      
        m_adView = adView;
        [m_viewController.view addSubview:adView];
        m_adView.hidden = YES;
        
    }
    */
}

// This method is called if an error has occurred while requesting Ad data.
- (void)didFailWithMessage:(NSString*)msg
{
	LogMsg("Error getting tapjoy ad");
	GetMessageManager()->SendGUIEx(MESSAGE_TYPE_TAPJOY_AD_READY, 0,0,0);
	
}

// This method must return one of TJC_AD_BANNERSIZE_320X50, TJC_AD_BANNERSIZE_640X100, or TJC_AD_BANNERSIZE_768X90. 
- (NSString*)adContentSize
{
	return 0;
//    return TJC_AD_BANNERSIZE_640X100; Uh, this is no longer part of the SDK..
}

- (void)showFullscreenAd:(NSNotification*)notifyObj
{
    LogMsg("Got fullscreen ad, showing");

    //[TJPlacement placementWithName:@"WatchAd" delegate:nil];
    //[TapjoyConnect showFullScreenAd];
   
    /*
    MyAppDelegate * appDelegate = [[UIApplication sharedApplication] delegate];
    
    [Tapjoy showFullScreenAdWithViewController: appDelegate.viewController];
    
     */
}

// This method must return a boolean indicating whether the Ad will automatically refresh itself.
- (BOOL)shouldRefreshAd
{
	return NO;
}

- (void)showFullscreenAdError:(NSNotification*)notifyObj
{
    NSLog(@"There is no Ad available!");
}

- (void)getUpdatedPoints:(NSNotification*)notifyObj
{
	NSNumber *tapPoints = notifyObj.object;
	NSString *tapPointsStr = [NSString stringWithFormat:@"Tap Points: %d", [tapPoints intValue]];
	// Print out the updated points value.
	NSLog(@"%@", tapPointsStr);
	GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_TAP_POINTS_RETURN, [tapPoints intValue],0,0, "");

}

- (void)getSpendPoints:(NSNotification*)notifyObj
{
	NSNumber *tapPoints = notifyObj.object;
	NSString *tapPointsStr = [NSString stringWithFormat:@"Tap Points: %d", [tapPoints intValue]];
	// Print out the updated points value.
	NSLog(@"%@", tapPointsStr);
	GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_SPEND_TAP_POINTS_RETURN, [tapPoints intValue],0,0, "");
}

- (void)getAwardPoints:(NSNotification*)notifyObj
{
	NSNumber *tapPoints = notifyObj.object;
	NSString *tapPointsStr = [NSString stringWithFormat:@"Tap Points: %d", [tapPoints intValue]];
	// Print out the updated points value.
	NSLog(@"%@", tapPointsStr);
	GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_AWARD_TAP_POINTS_RETURN, [tapPoints intValue],0,0, "");
}

- (void)getAwardPointsError:(NSNotification*)notifyObj
{
	LogMsg("getAwardPointsError");
	GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_AWARD_TAP_POINTS_RETURN_ERROR, 0,0,0, "");
}

- (void)getUpdatedPointsError:(NSNotification*)notifyObj
{
	LogMsg("GetUpdatedPointsError");
	GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_TAP_POINTS_RETURN_ERROR, 0,0,0, "");
}

- (void)getSpendPointsError:(NSNotification*)notifyObj
{
	LogMsg("GetUpdatedPointsError");
	GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_SPEND_TAP_POINTS_RETURN_ERROR, 0,0,0, "");
}

-(void)tjcConnectSuccess:(NSNotification*)notifyObj
{
	NSLog(@"Tapjoy connect Succeeded");
}

-(void)offerwallClosed:(NSNotification*)notifyObj
{
	NSLog(@"Offerwall closed");
    GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_OFFERWALL_CLOSED, 0,0,0, "");

}
             
             
- (void)showEarnedCurrencyAlert:(NSNotification*)notifyObj
{
    NSNumber *tapPointsEarned = notifyObj.object;
   int earnedNum = [tapPointsEarned intValue];
   NSLog(@"Currency earned: %d", earnedNum);
    
    
    //Well, in my games Proton handles the alert.  Maybe some people want Tapjoy to do it though?
    
   // Pops up a UIAlert notifying the user that they have successfully earned some currency.
   // This is the default alert, so you may place a custom alert here if you choose to do so.
    //   [TapjoyConnect showDefaultEarnedCurrencyAlert];

    // This is a good place to remove this notification since it is undesirable to have a pop-up alert during gameplay.
    //[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_TAPPOINTS_EARNED_NOTIFICATION object:nil];
    
    GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_TAPJOY_EARNED_TAP_POINTS, earnedNum,0,0, "");
}

             
-(void)tjcConnectFail:(NSNotification*)notifyObj
{
	NSLog(@"Tapjoy connect Failed");	
}


/*


- (void)virtualGoodsUpdated:(NSNotification*)notifyObj
{
	// Example on how to access the downloaded virtual goods.
	StoreItem * strItem;
	
	NSMutableArray* vgStoreItemArray = [TapjoyConnect getPurchasedVGStoreItems];
	
	for (int i = 0; i < [vgStoreItemArray count]; ++i) 
	{
		NSLog(@"VG Item %d Start================================================================================================", i);
		strItem = [vgStoreItemArray objectAtIndex:i];
		
		NSLog(@"Item Name:					%@", [strItem title]);
		NSLog(@"Item Type:					%@", [strItem storeItemType]);
		NSLog(@"Item Description:			%@", [strItem description]);
		NSLog(@"Item Price:					%d", [strItem price]);
		NSLog(@"Item Currency Name:			%@", [strItem currencyName]);
		NSLog(@"Item Data File Location:	%@", [strItem datafileLocation]);
		
		// Print out contents of data file.
		if(![[strItem datafileLocation] isEqualToString:@""])
		{
			NSError *error;
			NSArray *contentsArray = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[strItem datafileLocation] error:&error];
			for (int j = 0; j < [contentsArray count]; ++j) 
			{
				NSLog(@"     %d Data File Contents: %@", j, [contentsArray objectAtIndex:j]);
			}
		}
		
		NSMutableDictionary *dict = [strItem attributeValues];
		
		for (id key in dict)
		{
			id value = [dict objectForKey:key];
			NSLog(@"     Attribute:%@ Value:%@", key, value);
		}
		NSLog(@"VG Item %d End==================================================================================================", i);
	}
	
	[vgStoreItemArray release];
}

*/

@end


@interface TJDirectPlayPlacementDelegate ()

@end

@implementation TJDirectPlayPlacementDelegate
-(id)init
{
    self = [super init];
    
    if (self)
    {}
    
    return self;
}

- (void)requestDidSucceed:(TJPlacement*)placement
{
    NSLog(@"Tapjoy request did succeed, contentIsAvailable:%d", placement.isContentAvailable);
}

- (void)contentIsReady:(TJPlacement*)placement
{
    NSLog(@"Tapjoy placement content is ready to display");
}

- (void)requestDidFail:(TJPlacement*)placement error:(NSError *)error
{
    NSLog(@"Tapjoy request failed with error: %@", [error localizedDescription]);
}

- (void)contentDidAppear:(TJPlacement*)placement
{
    NSLog(@"Content did appear for %@ placement", [placement placementName]);
}

- (void)contentDidDisappear:(TJPlacement*)placement
{
    //[_tjViewController enableButton:_tjViewController.getDirectPlayVideoAdButton enable:YES];
    
    LogMsg("Showed ad or thing, reinitting");
    // Request next placement after the previous one is dismissed
   // _tjManager.m_addPlacement = [TJPlacement placementWithName:@"WatchAd" delegate:self];
   [_tjManager.m_addPlacement requestContent];
   // [_tjManager retain];
  //  LogMsg("retain count is %d", [_tjManager retainCount]);
    //_tjManager.m_addPlacement = nil;
    
    // Best Practice: We recommend calling getCurrencyBalance as often as possible so the user’s balance is always up-to-date.
     // [Tapjoy getCurrencyBalance];

    NSLog(@"Content did disappear for %@ placement", [placement placementName]);
}
@end

#endif