//Main java file.  You should scroll down and edit the part by "EDIT THESE!" as well as the "package" setting below.
//Thanks to Phil Hassey for his help and code

package com.rtsoft.rtsimpleapp;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.widget.Button;
import android.widget.TextView;
import android.provider.Settings.Secure;

import android.content.Context;
import android.view.KeyEvent;
import android.view.inputmethod.InputMethodManager;
import android.opengl.GLSurfaceView;
import android.graphics.PixelFormat;

import android.os.Bundle;
import android.os.Vibrator;
import android.view.MotionEvent;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import android.util.Log;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

// Licensing Server
import com.android.vending.licensing.AESObfuscator;
import com.android.vending.licensing.LicenseChecker;
import com.android.vending.licensing.LicenseCheckerCallback;
import com.android.vending.licensing.ServerManagedPolicy;
import com.android.vending.licensing.StrictPolicy;

import java.util.List;
import java.io.File;
import java.io.FileOutputStream;

import android.media.MediaPlayer;
import android.content.res.AssetFileDescriptor;
import android.media.SoundPool;
import android.media.AudioManager;
import java.util.Locale;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class Main extends Activity implements SensorEventListener

public class Main extends Activity implements SensorEventListener
{
	
	//********** EDIT THESE! DON'T NEED TO EDIT THE REST **************
	public static String PackageName= "com.rtsoft.rtsimpleapp";
	public static String dllname= "rtsimpleapp";
	public static boolean securityEnabled = false; //if false, it won't try to use the online license stuff
	//************************************************************************

	public static Main app; //a global to use in static functions with JNI
	
	//for the accelerometer
	private static float accelHzSave     = 0;
    private static Sensor sensor;
    private static SensorManager sensorManager;
	
	
    ////////////////////////////////////////////////////////////////////////////
    // Licensing Server code
    ////////////////////////////////////////////////////////////////////////////
        public boolean is_demo = false;
        private static final String BASE64_PUBLIC_KEY = "REPLACE THIS WITH YOUR PUBLIC KEY";
    
        // Generate your own 20 random bytes, and put them here.
        private static final byte[] SALT = new byte[] {
            24, -96, 10, 91, 65, -86, -54, -73, -64, 12, -84, -90, -23, -68, 20, -67, 45, 35, 85, 17
        };
    
        private LicenseCheckerCallback mLicenseCheckerCallback;
        private LicenseChecker mChecker;
        
        private class MyLicenseCheckerCallback implements LicenseCheckerCallback
		{
            public void allow() 
			{
                Log.v("allow()",getString(R.string.allow));
                
                if (isFinishing()) 
				{
                    // Don't update UI if Activity is finishing.
                    return;
                }
                // Should allow user access.
//                 displayResult(getString(R.string.allow));
            }
    
            public void dontAllow() 
			{
                Log.v("dontAllow()",getString(R.string.dont_allow));
                
                is_demo = true;
                
                if (isFinishing()) 
				{
                    // Don't update UI if Activity is finishing.
                    return;
                }
//                 displayResult(getString(R.string.dont_allow));
                // Should not allow access. In most cases, the app should assume
                // the user has access unless it encounters this. If it does,
                // the app should inform the user of their unlicensed ways
                // and then either shut down the app or limit the user to a
                // restricted set of features.
                // In this example, we show a dialog that takes the user to Market.
                showDialog(0);
            }
    
            public void applicationError(ApplicationErrorCode errorCode)
			{
                String result = String.format(getString(R.string.application_error), errorCode);
                Log.v("applicationError",result);
                
                dontAllow();
            
                if (isFinishing()) 
				{
                    // Don't update UI if Activity is finishing.
                    return;
                }
                // This is a polite way of saying the developer made a mistake
                // while setting up or calling the license checker library.
                // Please examine the error code and fix the error.
//                 displayResult(result);
            }
        }
        
        protected Dialog onCreateDialog(int id) 
		{
            // We have only one dialog.
            return new AlertDialog.Builder(this)
                .setTitle(R.string.unlicensed_dialog_title)
                .setMessage(R.string.unlicensed_dialog_body)
                .setPositiveButton(R.string.buy_button, new DialogInterface.OnClickListener() 
				{
                    public void onClick(DialogInterface dialog, int which)
					{
                        Intent marketIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(
                            "http://market.android.com/details?id=" + getPackageName()));
                        startActivity(marketIntent);
                     
						app.finish();
						android.os.Process.killProcess(android.os.Process.myPid());
                    }
                })
                .setNegativeButton(R.string.quit_button, new DialogInterface.OnClickListener()
				{
                    public void onClick(DialogInterface dialog, int which)
					{
                      
					app.finish();
					android.os.Process.killProcess(android.os.Process.myPid());
                    }
                })
                .setNeutralButton("Retry", new DialogInterface.OnClickListener()
				{
                    public void onClick(DialogInterface dialog, int which)
					{
                        is_demo = false;
                        doCheck();
                    }
                })
                .create();
        }
    
        private void doCheck() 
		{
    //         mCheckLicenseButton.setEnabled(false);
    //         setProgressBarIndeterminateVisibility(true);
    //         mStatusText.setText(R.string.checking_license);
            mChecker.checkAccess(mLicenseCheckerCallback);
        }
        
        private void license_init() 
		{
                // Try to use more data here. ANDROID_ID is a single point of attack.
            String deviceId = Secure.getString(getContentResolver(), Secure.ANDROID_ID);
    
            // Library calls this when it's done.
            mLicenseCheckerCallback = new MyLicenseCheckerCallback();
            // Construct the LicenseChecker with a policy.
            mChecker = new LicenseChecker(
                this,
                new ServerManagedPolicy(this,new AESObfuscator(SALT, getPackageName(), deviceId)),
//                 new StrictPolicy(),
                BASE64_PUBLIC_KEY);
            doCheck();
        }
    ////////////////////////////////////////////////////////////////////////////




	@Override
    protected void onCreate(Bundle savedInstanceState) 
	{
        app = this;
		super.onCreate(savedInstanceState);
        mGLView = new AppGLSurfaceView(this, this);
     	
		setContentView(mGLView);
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		if (securityEnabled) this.license_init();
    }

    @Override
    protected synchronized void onPause()
	{
		float hzTemp = accelHzSave;
	    setup_accel(0); //disable accelerator messages to save on CPU/battery
		accelHzSave = hzTemp;
        super.onPause();
        mGLView.onPause();
    }

    @Override
	protected synchronized void onResume()
	{
  
		super.onResume();
        mGLView.onResume();
		setup_accel(accelHzSave);
  
	}
 
  // JNI used to get Save data dir
    public static String get_docdir() 
	{
		File fdir = app.getFilesDir();
     	return fdir.getAbsolutePath();
    }
 	
	// JNI used to get Save data dir
    public static String get_apkFileName() 
	{
		String apkFilePath = null;
			ApplicationInfo appInfo = null;
			PackageManager packMgmr = app.getPackageManager();
			try {
			appInfo = packMgmr.getApplicationInfo(PackageName, 0);
		} catch (NameNotFoundException e) 
		{
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
			return appInfo.sourceDir;	
	 }

	public static String get_region()
	{
		//will return region in the format "en_us"
		Locale l = java.util.Locale.getDefault();    
		return (l.getLanguage()+"_"+l.getCountry()).toLowerCase();
	}
	
  @Override
	public void onSensorChanged(SensorEvent event) 
	{
     		
        switch (event.sensor.getType())
		{
            case Sensor.TYPE_ACCELEROMETER:
			
			if (event.values.length < 3) return; //who knows what this is
			//		Log.d(PackageName, "Accel: " + "x:"+Float.toString(event.values[0]) + " y:"+Float.toString(event.values[1]) + " z:"+Float.toString(event.values[2]));
    			nativeOnAccelerometerUpdate(event.values[0], event.values[1], event.values[2]);
     

            break;

        case Sensor.TYPE_ORIENTATION:

				//Log.d(PackageName, "Orientation: " + "x:"+Float.toString(event.values[0]));
       break;
        }
    }
	
  @Override
	public void onAccuracyChanged(Sensor sensor, int accuracy)
	{
    	//Log.d(PackageName,"onAccuracyChanged: " + sensor + ", accuracy: " + accuracy);
    }


	public void setup_accel(float hz) //0 to disable
	{
	  	accelHzSave = hz;
		sensorManager = (SensorManager) app.getSystemService(Context.SENSOR_SERVICE);
		
		sensorManager.unregisterListener(this);

		if (hz > 0)
		{
			sensorManager.registerListener(app, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), sensorManager.SENSOR_DELAY_GAME);
		}
		
		//if we ever need orientation data, do this:
		//sensorManager.registerListener(app, sensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), sensorManager.SENSOR_DELAY_GAME);
	
	}
	
	/**
     * The listener that listen to events from the accelerometer listener
     */
    
	    // JNI to open_url
    public static void LaunchURL(String url)
	{
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setData(Uri.parse(url));
        app.startActivity(intent);
    }

	  public void toggle_keyboard(boolean show) 
	  {
        Log.v("Msg", "Toggling keyboard");
		 InputMethodManager mgr = (InputMethodManager)app.getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow (mGLView.getWindowToken(),0);
        if (show) 
		{
            Log.v("Msg","Enabling keyboard");
			mgr.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
            // On the Nexus One, SHOW_FORCED makes it impossible
            // to manually dismiss the keyboard.
            // On the Droid SHOW_IMPLICIT doesn't bring up the keyboard.
        }
    }

	final static int VIRTUAL_KEY_BACK = 500000;

	 public boolean onKeyDown(int keycode, KeyEvent e) 
	 {
        //Log.v("keydown",Character.toString(Character.toChars(e.getUnicodeChar())[0]));

		 if (keycode == KeyEvent.KEYCODE_BACK)
		{
            Log.v("onKeyDown","KEYCODE_BACK");
        }

		 if (keycode == KeyEvent.KEYCODE_MENU)
		{
            Log.v("onKeyDown","KEYCODE_MENU");
        }

        if (keycode == KeyEvent.KEYCODE_SEARCH)
		{
            Log.v("onKeyDown","KEYCODE_SEARCH");
        }


        switch (keycode)
		{
			case KeyEvent.KEYCODE_BACK:
			{
				//Log.v("onKeyDown","Sending virtual back");
				nativeOnKey(1, VIRTUAL_KEY_BACK, e.getUnicodeChar()); 
				return true; //signal that we handled it
			}
			case KeyEvent.KEYCODE_MENU:
			case KeyEvent.KEYCODE_SEARCH:

			 return super.onKeyDown(keycode, e);
		}
      
       /*
		if (keycode == KeyEvent.KEYCODE_BACK)
		{
                Log.v("onKeyUp:is_quit","KEYCODE_BACK");
                this.finish();
                return true;
        }
	    */

    	
		nativeOnKey(1, keycode,e.getUnicodeChar()); //0 is type keyup
 
        return super.onKeyDown(keycode, e);
//         return super.onKeyDown(keycode,e);
    }
    

    public boolean onKeyUp(int keycode, KeyEvent e)
	{
        Log.v("onKeyUp","Keyup Got "+keycode+" "+Character.toString(Character.toChars(e.getUnicodeChar())[0]));
       
		  if (keycode == KeyEvent.KEYCODE_BACK)
		{
            Log.v("onKeyUp","KEYCODE_BACK");
        }
        if (keycode == KeyEvent.KEYCODE_MENU)
		{
            Log.v("onKeyUp","KEYCODE_MENU");
        }

        if (keycode == KeyEvent.KEYCODE_SEARCH)
		{
            Log.v("onKeyUp","KEYCODE_SEARCH");
        }

	
        switch (keycode)
		{
			case KeyEvent.KEYCODE_BACK:
			{
				nativeOnKey(0, VIRTUAL_KEY_BACK, e.getUnicodeChar()); //0 is type keyup
				return true; //signal that we handled it
			}

			case KeyEvent.KEYCODE_MENU:
			case KeyEvent.KEYCODE_SEARCH:

			 return super.onKeyUp(keycode, e);
		}
      
       /*
		if (keycode == KeyEvent.KEYCODE_BACK)
		{
                Log.v("onKeyUp:is_quit","KEYCODE_BACK");
                this.finish();
                return true;
        }
	    */

		nativeOnKey(0, keycode,e.getUnicodeChar()); //0 is type keyup
    
        return super.onKeyUp(keycode, e);
    }
    
	//************** SOUND STUFF *************

	
    // JNI to play music, etc
    public MediaPlayer _music = null;
    public static void music_play(String fname)
	{
		if (app._music != null)
		{
			app._music.reset();
		} else
		{
	       app._music = new MediaPlayer();
       
		}
       // Log.v("music_play",fname);

        AssetManager am = app.getAssets();
        try { 
            AssetFileDescriptor fd = am.openFd(fname);
            app._music.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength());
            fd.close();
            app._music.setLooping(true);
            app._music.prepare();
            app._music.start();
        } catch(IOException e) 
		{ 
		Log.v("Can't load music", fname);
		}
    }

    public static void music_stop() 
	{
        if (app._music == null) { return; }
        app._music.stop();
    }
	
	public static void music_set_volume(float v) 
	{
        if (app._music == null) { return; }
        app._music.setVolume(v,v);
    }

    public static void vibrate(int vibMS) 
	{
        Vibrator v = (Vibrator) app.getSystemService(Context.VIBRATOR_SERVICE);
		v.vibrate(vibMS);
    }

    public static int music_get_pos() 
	{
        if (app._music == null) { return 0; }
        return app._music.getCurrentPosition();
    }
  
	public static boolean music_is_playing() 
	{
        if (app._music == null) { return false; }
        return app._music.isPlaying();
    }

    public static void music_set_pos(int positionMS) 
	{
        if (app._music == null) 
		{
			Log.v("warning: music_set_position:", "no music playing, can't set position");
			return; 
		}
		app._music.seekTo(positionMS);
    }


	// JNI to play sounds
    public SoundPool _sounds = new SoundPool(8,AudioManager.STREAM_MUSIC,0);
    
	public static int sound_load(String fname) 
	{
        //Log.v("sound_load",fname);
        AssetManager am = app.getAssets();
        try { 
            AssetFileDescriptor fd = am.openFd(fname);
            int sid = app._sounds.load(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength(),1);
            return sid;
        } catch(IOException e)
		{
			Log.v("Can't load sound", fname);
		}
        return 0;
    }
    public static int sound_play(int soundID, float leftVol, float rightVol, int priority, int loop, float speedMod ) 
	{
		//Log.v("MSG", "Playing sound: "+soundID);
		//Log.v("Sound vol:", ""+leftVol);
        return app._sounds.play(soundID,leftVol,rightVol,priority,loop,speedMod);
    }
  
	public static void sound_kill(int soundID ) 
	{
		//Log.v("MSG", "Playing sound: "+soundID);
		//Log.v("Sound vol:", ""+leftVol);
        app._sounds.unload(soundID);
    }

	  public static void sound_stop(int streamID ) 
	{
		//Log.v("MSG", "Playing sound: "+soundID);
		//Log.v("Sound vol:", ""+leftVol);
        app._sounds.stop(streamID);
    }

	//****************************************

	private GLSurfaceView mGLView;
	private static native void nativeOnKey(int type, int keycode, int c);
	private static native void nativeLaunchURL();
	private static native void nativeOnAccelerometerUpdate(float x, float y, float z);

	static 
    {
        System.loadLibrary(dllname);
    }
}

class AppGLSurfaceView extends GLSurfaceView
{
    public AppGLSurfaceView(Context context, Main _app) 
	{
    	super(context);
        app = _app;
	
		//if we wanted a true 32 bit surface... warning, can be very slow.

		//setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		//getHolder().setFormat(PixelFormat.TRANSLUCENT);
		
		mRenderer = new AppRenderer(_app);
   
		setRenderer(mRenderer);
	  }
    	
	public void onPause() 
    {
        super.onPause();
        nativePause();   
    }
    public void onResume() 
    {
        super.onResume();
        nativeResume();   
    }
    	
    // single touch version, works starting API4/??
    public synchronized boolean onTouchEvent(final MotionEvent e)
	{
      	
		if (app.is_demo)
		{
			app.showDialog(0);
		}
		 
		float x = e.getX(); float y = e.getY();
     	int finger = 0; //planning ahead for multi touch
		nativeOnTouch(e.getAction(), x,y,finger);
        return true;
    }
    

	AppRenderer mRenderer;

	private static native void nativePause();
	private static native void nativeResume();
	private static native void nativeOnTouch(int action, float x, float y, int finger);
	public Main app;
 
}

class AppRenderer implements GLSurfaceView.Renderer 
{
    
	public AppRenderer(Main _app)
	{
		app = _app;
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) 
    {
        //gl.glViewport(0, 0, w, h);
        nativeResize(w, h);
        nativeInit();
    }
	
	final static int MESSAGE_NONE = 0;
	final static int MESSAGE_OPEN_TEXT_BOX = 1;
	final static int MESSAGE_CLOSE_TEXT_BOX = 2;
	final static int MESSAGE_CHECK_CONNECTION = 3;
	final static int MESSAGE_SET_FPS_LIMIT = 4;
	final static int MESSAGE_SET_ACCELEROMETER_UPDATE_HZ = 5;
	final static int MESSAGE_FINISH_APP = 6; //only respected by windows and android right now.  webos and iphone don't really need it

    public synchronized void onDrawFrame(GL10 gl)
    {
	

		nativeUpdate(); //maybe later we'll want to adjust this for performance reasons..
        nativeRender();

		//let's process OS messages sent from the app if any exist
		int type = MESSAGE_NONE;

		while ( (type = nativeOSMessageGet()) != 0) //it returns 0 if none is available
		{
			switch (type)
			{
				case MESSAGE_OPEN_TEXT_BOX: //open text box
					app.toggle_keyboard(true);
					break;

				case MESSAGE_CLOSE_TEXT_BOX: //close text box
					app.toggle_keyboard(false);
					break;
		
				case MESSAGE_SET_ACCELEROMETER_UPDATE_HZ: 
					app.setup_accel(nativeGetLastOSMessageX());
				break;

				case MESSAGE_FINISH_APP: 
					Log.v(app.PackageName, "Finishing app from java side");
					nativeDone();
					app.finish();
					android.os.Process.killProcess(android.os.Process.myPid());
				break;

				default:

					Log.v("Unhandled","Unhandled OS message");
			}
			 
		}

    }

    private static native void nativeInit();
    private static native void nativeResize(int w, int h);
    private static native void nativeUpdate();
    private static native void nativeRender();
    private static native void nativeDone();
	
	//yes, I probably should do this as a Java class and init it from C++ and send that over but..

	private static native int nativeOSMessageGet();
	private static native float nativeGetLastOSMessageX();
	public Main app;
}
