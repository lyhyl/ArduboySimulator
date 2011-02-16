#include "AudioManagerDenshion.h"
#include <CoreAudio/CoreAudioTypes.h>
#include <AudioToolbox/AudioToolbox.h>
#include "CocosDenshion/SimpleAudioEngine.h"

#if TARGET_OS_IPHONE == 1
#include <AVFoundation/AVFoundation.h>
#endif

AVAudioPlayer * m_bgMusicPlayer;


AudioManagerDenshion::AudioManagerDenshion()
{
	m_bgMusicPlayer = NULL;
	m_bDisabledMusicRecently = false;
}

AudioManagerDenshion::~AudioManagerDenshion()
{
}

bool AudioManagerDenshion::Init()
{
	[CDSoundEngine setMixerSampleRate: 22050];
	[SimpleAudioEngine sharedEngine];

	[[CDAudioManager sharedManager].soundEngine setSourceGroupNonInterruptible:0 isNonInterruptible:TRUE];
	[[CDAudioManager sharedManager] setResignBehavior:kAMRBStopPlay autoHandle:YES];
	
	LogMsg("Initialized Denshion");
	return true; //success
}

#if TARGET_OS_IPHONE == 1

bool CheckIfOtherAudioIsPlaying()
{
UInt32 isPlaying;
UInt32 propertySize = sizeof(isPlaying);

AudioSessionInitialize(NULL, NULL, NULL, NULL);
AudioSessionGetProperty(kAudioSessionProperty_OtherAudioIsPlaying, &propertySize, &isPlaying);

#ifdef _DEBUG
	LogMsg("Playing is %d", isPlaying);
#endif
if (isPlaying != 0) 
{
		return true;
}

	
/*
// since no other audio is *supposedly* playing, then we will make darn sure by changing the audio session category temporarily
// to kick any system remnants out of hardware (iTunes (or the iPod App, or whatever you wanna call it) sticks around)
UInt32 sessionCategory = kAudioSessionCategory_MediaPlayback;
AudioSessionSetProperty(kAudioSessionProperty_Audi oCategory, sizeof(sessionCategory), &sessionCategory);
AudioSessionSetActive(YES);

// now change back to ambient session category so our app honors the "silent switch"
sessionCategory = kAudioSessionCategory_AmbientSound;
AudioSessionSetProperty(kAudioSessionProperty_Audi oCategory, sizeof(sessionCategory), &sessionCategory);
*/

return false;
}

#endif

void AudioManagerDenshion::Vibrate()
{
#if TARGET_OS_IPHONE == 1

	if (!m_bVibrationDisabled)
	{
		AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
	}
#endif
}


void AudioManagerDenshion::DestroyAudioCache()
{

}

void AudioManagerDenshion::Stop(AudioHandle id)
{

	[[SimpleAudioEngine sharedEngine]stopEffect: id];
}

void AudioManagerDenshion::Preload(string fName, bool bLooping, bool bIsMusic, bool bAddBasePath , bool bForceStreaming)
{
 
	if (bIsMusic)
	{
		assert(!"We don't have that yet..");
		return;
	}
	
	NSString *soundFile =  [NSString stringWithCString:  fName.c_str() encoding: [NSString defaultCStringEncoding]];
	
	[[SimpleAudioEngine sharedEngine] preloadEffect: soundFile];
	//GetAudioObjectByFileName(fName, bLooping);
}

AudioHandle AudioManagerDenshion::PlayWithAVPlayer( string fName)
{
	NSString *soundFile =  [NSString stringWithCString:  fName.c_str() encoding: [NSString defaultCStringEncoding]];
	
	NSError *err;
	AVAudioPlayer* audioPlayer =  [[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL fileURLWithPath:soundFile] error:&err];
	//[audioPlayer setDelegate:self];
	[audioPlayer prepareToPlay];
	[audioPlayer play]; // returns BOOL
	
	return AUDIO_HANDLE_BLANK;
}

AudioHandle AudioManagerDenshion::Play( string fName, bool bLooping /*= false*/, bool bIsMusic /*= false*/, bool bAddBasePath, bool bForceStreaming )
{
	#ifdef _DEBUG
	LogMsg("Playing %s", fName.c_str());
	#endif

	if (bIsMusic)
	{
		
		if (!GetMusicEnabled()) 
		{
			
			m_lastMusicFileName = fName;
			m_bLastMusicLooping = bLooping;

	#ifdef _DEBUG
LogMsg("Music disabled, pretending to play");
#endif
			return AUDIO_HANDLE_BLANK;
		}
		
		
		if (m_lastMusicFileName == fName && m_bLastMusicLooping == true && !m_bDisabledMusicRecently)
		{
			//it's already playing!
			#ifdef _DEBUG
			LogMsg("Seems to already be playing");
			#endif
			return AUDIO_HANDLE_BLANK;
		}
		
		string basePath;
		
		if (bAddBasePath)
		{
			basePath = GetBaseAppPath();
		}
		NSString *soundFile =  [NSString stringWithCString:  (basePath+fName).c_str() encoding: [NSString defaultCStringEncoding]];
	
		if (m_bgMusicPlayer)
		{
			[m_bgMusicPlayer stop];
			[m_bgMusicPlayer release];
 			 m_bgMusicPlayer = NULL;
		}
		
		m_bgMusicPlayer = [AVAudioPlayer alloc ];
		
		[m_bgMusicPlayer initWithContentsOfURL: [NSURL fileURLWithPath:soundFile] error:nil];
		if (bLooping)
		{
			m_bgMusicPlayer.numberOfLoops = -1;
		}

	m_lastMusicFileName = fName;
	m_bLastMusicLooping = bLooping;
	m_bDisabledMusicRecently = false;
	
	[m_bgMusicPlayer prepareToPlay];

	// when you want to play the file

	[m_bgMusicPlayer play];
		
		
	return AUDIO_HANDLE_BLANK;
	}

	UInt32 soundId = AUDIO_HANDLE_BLANK;

	//AudioObjectOS *pAudio = GetAudioObjectByFileName(fName, bLooping);
	//if (!pAudio) return soundId;

	//soundId = pAudio->m_id;

	//AudioServicesPlaySystemSound(soundId);
	
	NSString *soundFile =  [NSString stringWithCString:  fName.c_str() encoding: [NSString defaultCStringEncoding]];
	
	//soundId = [[SimpleAudioEngine sharedEngine] playEffect: soundFile];
	
	soundId = [[SimpleAudioEngine sharedEngine] playEffect: soundFile pitch:1.0f pan:0.0f gain:1.0f loop:bLooping];
	return soundId;
}


void AudioManagerDenshion::Kill()
{
	DestroyAudioCache();
	
}


void AudioManagerDenshion::SetMusicEnabled( bool bNew )
{
	if (bNew != m_bMusicEnabled)
	{
		AudioManager::SetMusicEnabled(bNew);
		if (!bNew)
		{
		
		//music has been disabled
		 if (m_bgMusicPlayer)
		 {
			 [m_bgMusicPlayer stop];
			 [m_bgMusicPlayer release];
			 m_bgMusicPlayer = NULL;
			 m_bDisabledMusicRecently = true;
			 
		 }

			m_lastMusicID = AUDIO_HANDLE_BLANK;
	
		 
		} else
		{
		
		//turn music back on?
		
			if (!m_lastMusicFileName.empty())
			{
				Play(m_lastMusicFileName, m_bLastMusicLooping, true);
			}
			m_bDisabledMusicRecently = true;
		}
		
	}
}

void AudioManagerDenshion::StopMusic()
{
		
		LogMsg("Killing music..");
		
		if (m_bgMusicPlayer)
		{
			LogMsg("bgMusicPlayer was active, killing it");
			[m_bgMusicPlayer stop];
			[m_bgMusicPlayer release];
 			 m_bgMusicPlayer = NULL;
 			 
 		
		}
		
		 	m_lastMusicFileName = "";
			m_bLastMusicLooping = false;
			m_lastMusicID = AUDIO_HANDLE_BLANK;
			m_bDisabledMusicRecently = false;
	
}

