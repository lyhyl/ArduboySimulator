//  ***************************************************************
//  AudioManagerDenshion - Creation date: 02/16/2011
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef AudioManagerDenshion_h__
#define AudioManagerDenshion_h__

#include "AudioManager.h"



class AudioManagerDenshion: public AudioManager
{
public:
	AudioManagerDenshion();
	virtual ~AudioManagerDenshion();

	virtual bool Init();
	virtual void Kill();

	virtual AudioHandle Play(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false);
	virtual AudioHandle PlayWithAVPlayer(string fName); //also doesn't cache and uses AV player, useful for the TTS system or oneshots
	virtual void Preload(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false);
	virtual void Stop(AudioHandle id);
	virtual void SetMusicEnabled( bool bNew );
	virtual void Vibrate();
	virtual void StopMusic();

protected:
	void DestroyAudioCache();
	bool m_bDisabledMusicRecently;
};

#endif // AudioManagerDenshion_h__