//This is the only app-specific java file for the android project.  The real work is done in
//shared/android/v2_src/java.  When editing that, be careful, as it is used by all Proton projects.

//Thanks to Phil Hassey for his help and code

//Defines that help control the preprocessing step:

//************* EDIT THESE DEFINES ****************
//Because I'm using javapp preprocesser processing, //# means the command IS being used.  To comment out a preprocessor
//command, do // #command  (ie, insert a space after the //)

//#define RT_HOOKED_SUPPORT
//#define RT_TAPJOY_SUPPORT
//#define RT_CHARTBOOST_SUPPORT
//#define RT_FLURRY_SUPPORT

//**********************************************

package ${PACKAGE_NAME};
import ${PACKAGE_NAME}.SharedActivity;
import android.os.Bundle;

//#if defined(RT_TAPJOY_SUPPORT)

import com.tapjoy.TapjoyAwardPointsNotifier;
import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyConnectFlag;
import com.tapjoy.TapjoyConstants;
import com.tapjoy.TapjoyDailyRewardAdNotifier;
import com.tapjoy.TapjoyDailyRewardAdStatus;
import com.tapjoy.TapjoyDisplayAdNotifier;
import com.tapjoy.TapjoyEarnedPointsNotifier;
import com.tapjoy.TapjoyFullScreenAdNotifier;
import com.tapjoy.TapjoyLog;
import com.tapjoy.TapjoyNotifier;
import com.tapjoy.TapjoySpendPointsNotifier;
import com.tapjoy.TapjoyVideoNotifier;
import com.tapjoy.TapjoyVideoStatus;
import com.tapjoy.TapjoyViewNotifier;
import com.tapjoy.TapjoyViewType;


//#endif

//#if defined(RT_HOOKED_SUPPORT)
import com.hookedmediagroup.wasabi.WasabiApi;
//#endif

public class Main extends SharedActivity
{
	@Override
    protected void onCreate(Bundle savedInstanceState) 
	{
		//*************** EDIT THESE TO MATCH YOUR PROJECT STUFF ****************
		dllname= "RTAdTest";
		BASE64_PUBLIC_KEY = "<enter your android public key>";
		securityEnabled = false; 
   		IAPEnabled = true;

		//********************* You probably don't need to change anything below this **************
		
   		//#if defined(RT_HOOKED_SUPPORT)
   		HookedEnabled = true;
   		//#else
   		HookedEnabled = false;
   		//#endif
   		PackageName= "${PACKAGE_NAME}"; //edit build.xml to change this setting, not here
	
		System.loadLibrary(dllname);
	
	//#if defined(RT_TAPJOY_SUPPORT)
		TapjoyLog.enableLogging(true);
	
		// Connect with the Tapjoy server.  Call this when the application first starts.
		  TapjoyConnect.requestTapjoyConnect(getApplicationContext(), "<enter your tapjoy app id>", "<enter your tapjoy secret key>");
		
		  // For PAID APPS ONLY.  Replace your Paid App Pay-Per-Action ID as the parameter.
		  //TapjoyConnect.getTapjoyConnectInstance().enablePaidAppWithActionID("<replace this part>");
   
   	    // Init video.
	    TapjoyConnect.getTapjoyConnectInstance().initVideoAd(this);
 
   		//if we have tapjoy  managed currency, we need this
   		TapjoyConnect.getTapjoyConnectInstance().setEarnedPointsNotifier(this);
	
	
		//tapBannerSize = TapjoyDisplayAdSize.TJC_AD_BANNERSIZE_640X100;
	//#endif
		
		super.onCreate(savedInstanceState);
		if (HookedEnabled)
		{
			//#if defined(RT_HOOKED_SUPPORT)
				WasabiApi.init(this.getApplicationContext()); 
			//#endif
		}
    }
}
	
