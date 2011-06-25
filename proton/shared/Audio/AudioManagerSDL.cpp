#include "PlatformPrecomp.h"

//NOTE:  Much of this code is from the tiltodemo sample from the Palm WebBOS PDK

#ifdef RT_WEBOS

#include "SDL.h"

#define NUM_CHANNELS 16

#include "AudioManagerSDL.h"
#include "util/MiscUtils.h"


AudioManagerSDL::AudioManagerSDL()
{
	m_pMusicChannel = NULL;
	
}

AudioManagerSDL::~AudioManagerSDL()
{
	Kill();
}

AudioHandle AudioManagerSDL::Play( const string fileName )
{
	LogMsg("Playing %s", fileName.c_str());
	assert(!"huh?");
	return AUDIO_HANDLE_BLANK;
}

bool AudioManagerSDL::Init()
{
	// we'll use SDL_Mixer to do all our sound playback. 
	// It's a simple system that's easy to use. The most 
	// complicated part about using SLD_Mixer is initting it. So 
	// let's talk about that. 
	// this is the function definition:
	// Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
	// here's what it wants:
	// 
	// frequency: this is the sample rate you want the audio to
	// play at. On the Palm, things are optimized for 44100 samples per
	// second (CD quality). Though you can send in whatever sample rate you like.
	// 
	// format: For the Palm, you should use AUDIO_S16
	// 
	// channels: 1 for mono. 2 for stereo
	// 
	// chunksize: How big one audio buffer is, in bytes.
	// 
	// this example has the recommended settings for initting the mixer:

	int rate = 44100;
	Uint16 format = AUDIO_S16;
	int channels = 2;
	int bufferSize = 1024;
	
	if ( Mix_OpenAudio(rate, format, channels, bufferSize) )
	{
		// we had an error opening the audio
		LogMsg("unable to open Audio! Reason: %s\n", Mix_GetError());
		return false;

	}

	// SDL mixer requires that you specify the maximum number of simultaneous
	// sounds you will have. This is done through the function Mix_AllocateChannels. 
	// We'll need an answer for that. A channel has very little overhead,
	// so we'll arbitrarily allocate 16 channels (even though we're only using
	// 1 actual sound channel). Among other things, we need a channel per unique
	// simultaneous sound playback. So if we play the same sound 4 times, and have
	// 4 copies of the sound playing from different start times, we need four channels.
	// Note - streamed music does not consume a channel. Only sound playback does.
	// So why allocate more than we need? Because it's such a small overhead, and
	// later we don't need to come and revisit this every time we add a new sound. 
	Mix_AllocateChannels(NUM_CHANNELS);

	return true;
}

void AudioManagerSDL::KillCachedSounds(bool bKillMusic, bool bKillLooping, int ignoreSoundsUsedInLastMS, int killSoundsLowerPriorityThanThis, bool bKillSoundsPlaying)
{
	LogMsg("Killing sound cache");
	list<SoundObject*>::iterator itor = m_soundList.begin();


	while (itor != m_soundList.end())
	{

		if (!bKillLooping && (*itor)->m_bIsLooping) 
		{
			itor++;
			continue;
		}

		if (!bKillSoundsPlaying)
		{
			//are any channels currently using this sound?

			if ((*itor)->m_pLastChannelToUse && IsPlaying( (AudioHandle)(*itor)->m_pLastChannelToUse) )
			{
				itor++;
				continue;
			}
		}

		if (!bKillMusic && (*itor)->m_fileName == m_lastMusicFileName)
		{
			itor++;
			continue; //skip this one
		}

		delete (*itor);
		list<SoundObject*>::iterator itorTemp = itor;
		itor++;
		m_soundList.erase(itorTemp);
	}

	if (bKillMusic)
	{
		m_pMusicChannel = NULL;
	}
}

void AudioManagerSDL::Kill()
{
	Mix_HaltMusic();
	if ( m_pMusicChannel != NULL )
	{
		// free up any memore in use by the music track
		Mix_FreeMusic(m_pMusicChannel);

		// set it to null for safety (ensuring that 
		// if we acidentally refer to this variable after
		// deletion, at least it will be null)
		m_pMusicChannel = NULL; 
	}

	// close out the audio
	Mix_CloseAudio();
}

bool AudioManagerSDL::DeleteSoundObjectByFileName(string fName)
{
	list<SoundObject*>::iterator itor = m_soundList.begin();

	while (itor != m_soundList.end())
	{
		if ( (*itor)->m_fileName == fName)
		{
			delete (*itor);
			m_soundList.erase(itor);
			return true; //deleted
		}
		itor++;
	}

	return false; //failed
}

SoundObject * AudioManagerSDL::GetSoundObjectByFileName(string fName)
{
	list<SoundObject*>::iterator itor = m_soundList.begin();

	while (itor != m_soundList.end())
	{
		if ( (*itor)->m_fileName == fName)
		{
			return (*itor); //found a match
		}
		itor++;
	}

	return 0; //failed
}

void AudioManagerSDL::Preload( string fName, bool bLooping /*= false*/, bool bIsMusic /*= false*/, bool bAddBasePath /*= true*/, bool bForceStreaming )
{

	if (bIsMusic) return;//we don't preload music that way

	if (bIsMusic && !GetMusicEnabled()) return; //ignoring because music is off right now


	if (bIsMusic) 
	{
		m_lastMusicFileName = fName;
		bForceStreaming = true;
	}

	string basePath;

	if (bAddBasePath)
	{
		basePath = GetBaseAppPath();
	}
	SoundObject *pObject = GetSoundObjectByFileName((GetBaseAppPath()+fName).c_str());

	if (!pObject)
	{
		//create it
		pObject = new SoundObject;
		pObject->m_fileName = fName;

		assert(! (GetFileExtension(fName) == "mp3") && "SDL mixer doesn't support mp3 on webos yet though");

		pObject->m_pSound = Mix_LoadWAV( (basePath+fName).c_str());
		if (!pObject->m_pSound)
		{
			LogMsg("Error loading %s (%s)", (basePath+fName).c_str(), Mix_GetError());
			delete pObject;
			return;
		}
#ifdef _DEBUG
		LogMsg("Caching out %s", fName.c_str());
#endif
		
		pObject->m_bIsLooping = bLooping;

		m_soundList.push_back(pObject);
	}
}


AudioHandle AudioManagerSDL::Play( string fName, bool bLooping /*= false*/, bool bIsMusic, bool bAddBasePath, bool bForceStreaming)
{

	if (!GetSoundEnabled()) return 0;
	if ( !GetMusicEnabled() && bIsMusic )
	{
		m_bLastMusicLooping = bLooping;
		m_lastMusicFileName = fName;

		return 0;
	}

	if (bIsMusic && m_bLastMusicLooping == bLooping && m_lastMusicFileName == fName && m_bLastMusicLooping && IsPlaying((AudioHandle) m_pMusicChannel))
	{
		return (AudioHandle) m_pMusicChannel;
	}
	if (bIsMusic)
	{
		StopMusic();
	}
	int loops = 0;
	if (bLooping) loops = -1;


	if (bIsMusic)
	{
		string basePath;

		if (bAddBasePath)
		{
			basePath = GetBaseAppPath();
		}

		m_lastMusicFileName = fName;

		if (GetFileExtension(fName) == "mp3")
		{
			fName = ModifyFileExtension(fName, "ogg");
		}

		m_pMusicChannel = Mix_LoadMUS((GetBaseAppPath()+fName).c_str());
		
		m_lastMusicID = (AudioHandle) m_pMusicChannel;
		m_bLastMusicLooping = bLooping;
		
		SetMusicVol(m_musicVol);

		Mix_PlayMusic(m_pMusicChannel, loops);

		return (AudioHandle) m_pMusicChannel;

	}


	//non music

	SoundObject *pObject = GetSoundObjectByFileName(fName);

	if (!pObject)
	{
		//create it
		Preload(fName, bLooping, bIsMusic, bAddBasePath, bForceStreaming);
		pObject = GetSoundObjectByFileName(fName);
		if (!pObject)
		{
			LogError("Unable to cache sound %s", fName.c_str());
			return false;

		}
	}

	//play it
	pObject->m_pLastChannelToUse = Mix_PlayChannel(-1, pObject->m_pSound, loops);
	
	return (AudioHandle)pObject->m_pLastChannelToUse ;
}

AudioHandle AudioManagerSDL::Play( string fName, int vol, int pan /*= 0*/ )
{	
	
	assert(!"We don't support this");
	return NULL;
}

void AudioManagerSDL::Update()
{
	
}

void AudioManagerSDL::Stop( AudioHandle soundID )
{
	
	if (!soundID) return;
	
	if (soundID == (int)m_pMusicChannel)
	{
		StopMusic();
		return;
	}
	//pChannel->stop();

	Mix_HaltChannel(soundID);

}

AudioHandle AudioManagerSDL::GetMusicChannel()
{
	return (AudioHandle)m_pMusicChannel;

}

bool AudioManagerSDL::IsPlaying( AudioHandle soundID )
{
	if (soundID == 0) return false;

	if (soundID == (AudioHandle) m_pMusicChannel)
	{
		return Mix_PlayingMusic() != 0;
	}
	return Mix_Playing(soundID) != 0;
}


void AudioManagerSDL::SetMusicEnabled( bool bNew )
{
	if (bNew != m_bMusicEnabled)
	{
		AudioManager::SetMusicEnabled(bNew);
		if (bNew)
		{
			if (!m_lastMusicFileName.empty())
			{
				Play(m_lastMusicFileName, GetLastMusicLooping(), true);

			}
		} else
		{
			//kill the music
			
				StopMusic();
		}
	}

}

void AudioManagerSDL::StopMusic()
{
	DeleteSoundObjectByFileName(m_lastMusicFileName);
	Mix_HaltMusic();
	Mix_FreeMusic(m_pMusicChannel);
	m_pMusicChannel = NULL;
}

int AudioManagerSDL::GetMemoryUsed()
{
	
	return 0;
}

void AudioManagerSDL::SetFrequency( AudioHandle soundID, int freq )
{
	assert(soundID);
	//SDL::Channel *pChannel = (SDL::Channel*) soundID;
	//pChannel->setFrequency(float(freq));

}

void AudioManagerSDL::SetPan( AudioHandle soundID, float pan )
{
	assert(soundID);
	
	//SDL::Channel *pChannel = (SDL::Channel*) soundID;
	//pChannel->setPan(pan);
}

uint32 AudioManagerSDL::GetPos( AudioHandle soundID )
{
	assert(soundID);
	//unsigned int pos;
//	pChannel->getPosition(&pos, SDL_TIMEUNIT_MS);
	return 0;
}

void AudioManagerSDL::SetPos( AudioHandle soundID, uint32 posMS )
{
	assert(soundID);
	
	//pChannel->setPosition(posMS, SDL_TIMEUNIT_MS);
}


void AudioManagerSDL::SetVol( AudioHandle soundID, float vol )
{
	int ivol =  int(vol*128.0f);

#ifdef _DEBUG
	//ivol = 128;
	LogMsg("Setting audio handle %d to %d", soundID, ivol);

#endif
	Mix_Volume(soundID,ivol);
	//assert(soundID);
	//pChannel->setVolume(vol);
}

void AudioManagerSDL::SetMusicVol(float vol )
{
	/*
	if (m_pMusicChannel)
	{
		m_pMusicChannel->setVolume(vol);

	}
	m_musicVol = vol;
	*/
}


void AudioManagerSDL::SetPriority( AudioHandle soundID, int priority )
{

}
#endif