//  ***************************************************************
//  AudioManagerSDL - Creation date: 09/14/2010
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2010 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef AudioManagerSDL_h__
#define AudioManagerSDL_h__

#include "AudioManager.h"

#ifdef RT_WEBOS

#include "SDL_mixer.h"

class SoundObject
{
public:

	SoundObject()
	{
		m_pSound = NULL;
		m_bIsLooping = false;
		m_pLastChannelToUse = NULL;
	}

	~SoundObject()
	{
		if (m_pSound)
		{
			Mix_FreeChunk(m_pSound);
			m_pSound = NULL;
			m_pLastChannelToUse = NULL;
		}
	}

	Mix_Chunk *m_pSound;
	string m_fileName;
	bool m_bIsLooping;
	int m_pLastChannelToUse;
};

class AudioManagerSDL: public AudioManager
{
public:
	AudioManagerSDL();
	virtual ~AudioManagerSDL();

	virtual AudioHandle Play(const string fileName);

	virtual bool Init();
	virtual void Kill();

	virtual AudioHandle Play(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false);
	virtual AudioHandle Play(string fName, int vol, int pan = 0);

	virtual void Preload(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false);

	SoundObject * GetSoundObjectByFileName(string fName);
	virtual void KillCachedSounds(bool bKillMusic, bool bKillLooping, int ignoreSoundsUsedInLastMS, int killSoundsLowerPriorityThanThis, bool bKillSoundsPlaying);
	virtual void Update();
	virtual void Stop(AudioHandle soundID);
	virtual AudioHandle GetMusicChannel();
	virtual bool IsPlaying(AudioHandle soundID);
	virtual void SetMusicEnabled(bool bNew);
	virtual void StopMusic();
	virtual int GetMemoryUsed();
	bool DeleteSoundObjectByFileName(string fName);
	virtual void SetFrequency(AudioHandle soundID, int freq);
	virtual void SetPan(AudioHandle soundID, float pan); //0 is normal stereo, -1 is all left, +1 is all right
	virtual void SetVol(AudioHandle soundID, float vol);
	virtual void SetPriority(AudioHandle soundID, int priority);
	virtual uint32 GetPos( AudioHandle soundID );
	virtual void SetPos( AudioHandle soundID, uint32 posMS );
	virtual void SetMusicVol(float vol);

private:

	list<SoundObject*> m_soundList;
	Mix_Music *m_pMusicChannel;

protected:


private:
};


#endif // AudioManagerSDL_h__
#endif