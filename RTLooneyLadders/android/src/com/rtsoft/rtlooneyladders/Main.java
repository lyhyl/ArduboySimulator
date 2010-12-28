//This is the only app-specific java file for the android project.  The real work is done in
//shared/android/src/rtsoft/shared/SharedActivity.java.  When editing that, be careful, as it is used
//by all Proton projects.

//Thanks to Phil Hassey for his help and code

package com.rtsoft.RTLooneyLadders;
import com.rtsoft.shared.SharedActivity;
import android.os.Bundle;

public class Main extends SharedActivity
{
	@Override
    protected void onCreate(Bundle savedInstanceState) 
	{
		//EDIT THESE TO MATCH YOUR PROJECT NAME
		BASE64_PUBLIC_KEY = "uh, it would go here if you had one and wanted to use the google licensing stuff";
		PackageName= "com.rtsoft.RTLooneyLadders";
		dllname= "RTLooneyLadders";
		securityEnabled = false; 
       super.onCreate(savedInstanceState);
    }
}
	
