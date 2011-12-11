//  ***************************************************************
//  AudioManager - Creation date: 05/25/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

/*

//Designed to be a generic interface for audio.  You instantiate AudioManagerFMOD or AudioManagerOpenAL (iphone os only) and pass as this.

//Note, you can init this directory as a dummy to disable audio, useful for debugging a leak/crash.


*/


#ifndef AudioManager_h__
#define AudioManager_h__


typedef uint32 AudioHandle;

#define AUDIO_HANDLE_BLANK 0

class AudioManager
{
public:
	AudioManager();
	virtual ~AudioManager();

	virtual bool Init() {return true;}
	virtual void Kill() {};
	virtual AudioHandle Play(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false){return AUDIO_HANDLE_BLANK;}
	
	//virtual AudioHandle PlayEx(string fName, bool bLooping = false, uint32 freq = 0, float pan = 0){return AUDIO_HANDLE_BLANK;}

	virtual AudioHandle PlayWithAVPlayer(string fName) { return Play(fName, false, false); } //also doesn't cache and uses AV player, useful for the TTS system or oneshots

	virtual void Stop(AudioHandle soundID) {};
	virtual void Preload(string fName, bool bLooping = false, bool bIsMusic = false, bool bAddBasePath = true, bool bForceStreaming = false){};
	bool GetMusicEnabled() {return m_bMusicEnabled;}
	virtual void SetMusicEnabled(bool bNew) {m_bMusicEnabled = bNew;}
	string GetLastMusicFileName() {return m_lastMusicFileName;}
	bool GetLastMusicLooping() {return m_bLastMusicLooping;}
	AudioHandle GetLastMusicID() {return m_lastMusicID;}
	virtual void Vibrate(int duration = 300){ if (!m_bVibrationDisabled) {LogMsg("Vibrate!");} };
	void SetVibrateDisabled(bool bNew) {m_bVibrationDisabled = bNew;}
	virtual void StopMusic();
	virtual void Update() {}; //must be called once per frame
	virtual void FadeOutMusic() {StopMusic();}
	virtual void KillCachedSounds(bool bKillMusic, bool bKillLooping, int ignoreSoundsUsedInLastMS, int killSoundsLowerPriorityThanThis, bool bKillSoundsPlaying) {};
	virtual int GetMemoryUsed(){return 0;};
	virtual void SetFrequency(AudioHandle soundID, int freq) {};
	virtual void SetPan(AudioHandle soundID, float pan) {}; //0 is normal stereo, -1 is all left, +1 is all right
	virtual bool IsPlaying(AudioHandle soundID) {return false;}
	virtual void SetVol(AudioHandle soundID, float vol) {};
	virtual void SetPriority(AudioHandle soundID, int priority) {};
	virtual uint32 GetPos( AudioHandle soundID ){return 0;};
	virtual void SetPos( AudioHandle soundID, uint32 posMS ){};
	virtual void SetDLS(string fName); //example, "dink/midi/TimGM6mbTiny.dls" - if not set, FMOD will try to use whatever the system has.
	virtual void SetMusicVol(float vol){};
	bool GetSoundEnabled() {return m_bSoundEnabled;}
	virtual void SetSoundEnabled(bool bNew) {m_bSoundEnabled = bNew;}
	virtual void Suspend(){}; //stop all audio, app when into background or something
	virtual void Resume(){}; //restore audio that was stopped

protected:
	
	string m_lastMusicFileName;
	bool m_bLastMusicLooping;
	bool m_bMusicEnabled;
	AudioHandle m_lastMusicID;
	bool m_bVibrationDisabled;
	string m_midiSoundBankFile; //only used by FMOD currently
	float m_musicVol; //only used by FMOD currently.  0 means none, 1 means full blast
	bool m_bSoundEnabled;
};

bool CheckIfOtherAudioIsPlaying(); //are they playing ipod stuff before the app was run?  Should call this before playing your own.

#endif // AudioManager_h__