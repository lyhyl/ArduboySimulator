//This is shared between all android p+ projects so be careful!

package com.rtsoft.shared;
import com.rtsoft.shared.SharedActivity;
import com.rtsoft.shared.AppGLSurfaceView;

import android.os.Build;
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


//tricks for being compatible with Android 1.5 but still being able to use new features of 2.2
public class SharedMultiTouchInput
{
	public static SharedActivity app;

	public static void init(SharedActivity _app)
	{
		app = _app;
	}

	private static void dumpEvent(MotionEvent event)
	{
		String names[] = { "DOWN" , "UP" , "MOVE" , "CANCEL" , "OUTSIDE" ,
      "POINTER_DOWN" , "POINTER_UP" , "7?" , "8?" , "9?" };
		StringBuilder sb = new StringBuilder();
		int action = event.getAction();
		int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_").append(names[actionCode]);
		
		
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN
			  || actionCode == MotionEvent.ACTION_POINTER_UP)
		{
			sb.append("(pid ").append(
			action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			sb.append(")");
		}
		sb.append("[");
		for (int i = 0; i < event.getPointerCount(); i++)
		{
			sb.append("#").append(i);
			sb.append("(pid ").append(event.getPointerId(i));
			sb.append(")=").append((int)event.getX(i));
			sb.append(",").append((int)event.getY(i));
			if (i + 1 < event.getPointerCount())
				sb.append(";");
		}
		sb.append("]");
		Log.d("TEST", sb.toString());
	}


    public static boolean OnInput(final MotionEvent e)
	{
		//dumpEvent(e);
		
		int finger = 0; 
		int action = e.getAction();
		int actionCode = action & MotionEvent.ACTION_MASK;
		//Log.d(":", " getPointerCount " + e.getPointerCount());

		int pointerID = -1; //default, this means we don't care, send messages from all of them

		switch (actionCode)
		{
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_POINTER_DOWN:
				//these are the new style, telling us which finger they belong too
				pointerID = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				//Log.d(":", "Only sending data from pointer " + pointerID);
		}

		//translate for uniformity

		if (actionCode == MotionEvent.ACTION_POINTER_UP) actionCode = MotionEvent.ACTION_UP; else
			if (actionCode == MotionEvent.ACTION_POINTER_DOWN) actionCode = MotionEvent.ACTION_DOWN;

		float x, y;

		for (int i = 0; i < e.getPointerCount(); i++)
		{
			if (pointerID != -1 && pointerID != i) continue; //skip this, we only want to send info from one specific pointer

			finger = e.getPointerId(i);
			x = e.getX(i);
			y = e.getY(i);
			
			//Log.d("Input:", " Action# " + actionCode+ " id: "+finger +" x: " + x+" y: " + y);
			AppGLSurfaceView.nativeOnTouch(actionCode, x, y, finger);
		}
		

		return true;
	}
}
