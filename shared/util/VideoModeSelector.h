#ifndef videomodeselector_h__
#define videomodeselector_h__

#include "PlatformEnums.h"
#include <map>


/*!
 * Representation of a video mode.
 */
class VideoMode
{
private:
	std::string name;
	int x;
	int y;
	ePlatformID platformID;
	eOrientationMode orientationMode;

public:
	/*!
	 * Initializes a video mode that doesn't represent a valid video mode.
	 * The name will be empty, size will be zero and platform will be unknown.
	 */
	VideoMode();
	
	VideoMode(std::string name, int x, int y, ePlatformID platformID, eOrientationMode orientationMode = ORIENTATION_DONT_CARE);
	
	/*!
	 * The human readable name of the mode.
	 */
	std::string getName() const;
	
	/*!
	 * The horizontal resolution.
	 */
	int getX() const;
	
	/*!
	 * The vertical resolution.
	 */
	int getY() const;
	
	/*!
	 * The id of the platform.
	 */
	ePlatformID getPlatformID() const;
	
	/*!
	 * How to interpret this modes orientation.
	 */
	eOrientationMode getOrientationMode() const;

};

/*!
 * A class that can be used to select a video mode. Provides a number of video modes
 * that are usable on the platform.
 */
class VideoModeSelector
{
private:
	static std::map<std::string, VideoMode> g_videoModes;

	static void addVideoMode(std::string name, int x, int y, ePlatformID platformID, eOrientationMode orientationMode = ORIENTATION_DONT_CARE);

	static void initializeModes();
	
public:
	VideoModeSelector();
	
	/*!
	 * Requests a named video mode and returns it. If the requested mode
	 * can't be found returns \c NULL.
	 */
	const VideoMode* getNamedMode(const std::string& name) const;
};

#endif
