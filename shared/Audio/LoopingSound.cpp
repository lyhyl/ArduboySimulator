#include "PlatformPrecomp.h"
#include "LoopingSound.h"

LoopingSound::LoopingSound()
{
	m_loopingSoundHandle = AUDIO_HANDLE_BLANK;
	m_transitionSoundHandle = AUDIO_HANDLE_BLANK;
	m_moveStartTimeMS = 1400;
	m_moveEndTimeMS = 4300;
	m_bMoving = false;
	m_state = STATE_IDLE;
}

LoopingSound::~LoopingSound()
{
	KillAudio();
}

void LoopingSound::KillAudio()
{
	//kill any currently playing looping sound
	if (m_loopingSoundHandle)
	{
		GetAudioManager()->Stop(m_loopingSoundHandle);
		m_loopingSoundHandle = AUDIO_HANDLE_BLANK;
	}

	if (m_transitionSoundHandle != AUDIO_HANDLE_BLANK)
	{
		GetAudioManager()->Stop(m_transitionSoundHandle);
		m_transitionSoundHandle = AUDIO_HANDLE_BLANK;
	}
}

void LoopingSound::PlayIdleSound()
{
	if (!m_loopingIdle.empty())
	{
		m_loopingSoundHandle = GetAudioManager()->Play(m_loopingIdle, true, false);
	} else
	{
		m_loopingSoundHandle = NULL;
	}

	
	m_state = STATE_IDLE;
}
void LoopingSound::Init( string loopingMove, string moveStart /*= ""*/, string moveEnd /*= ""*/, string loopingIdle /*= ""*/ )
{

	m_loopingIdle = loopingIdle;
	m_loopingMove = loopingMove;
	m_moveStart = moveStart;
	m_moveEnd = moveEnd;

	//preload as needed
	
	if (!m_moveStart.empty()) GetAudioManager()->Preload(m_moveStart);
	if (!m_moveEnd.empty()) GetAudioManager()->Preload(m_moveEnd);

	PlayIdleSound();
	
}

void LoopingSound::PlayMoveSound()
{
	if (!m_loopingMove.empty())
	{
		m_loopingSoundHandle = GetAudioManager()->Play(m_loopingMove, true, false);
	} else
	{
		m_loopingSoundHandle = NULL;
	}
	m_state = STATE_MOVING;

}

void LoopingSound::SetMoving( bool bNew )
{

	if (bNew == m_bMoving) return; //nothing really changed

	m_bMoving = bNew;
  
	KillAudio();

	if (m_bMoving)
	{
	
		if (m_moveStart.empty())
		{
			//there is no move start sound, so just play the looping moving sound right now
			PlayMoveSound();
		} else
		{
			//play a starting sound first
			m_transitionSoundHandle =  GetAudioManager()->Play(m_moveStart);
			m_waitTimer = GetTick(TIMER_SYSTEM)+m_moveStartTimeMS;
			m_state = STATE_MOVE_START;
		}
	} else
	{
		//stop moving and play the idle sound

		if (m_moveEnd.empty())
		{
			PlayIdleSound();
		} else
		{
			//play a stopping sound first
			m_transitionSoundHandle =  GetAudioManager()->Play(m_moveEnd);
			m_waitTimer = GetTick(TIMER_SYSTEM)+m_moveEndTimeMS;
			m_state = STATE_MOVE_END;

		}

	}
}

void LoopingSound::Update()
{

	switch (m_state)
	{
	case STATE_MOVE_START:

		if (m_waitTimer < GetTick(TIMER_SYSTEM))
		{
			m_waitTimer = 0;
			//time to kick into the move loop sound
			GetAudioManager()->Stop(m_transitionSoundHandle);
			m_transitionSoundHandle = AUDIO_HANDLE_BLANK;

			PlayMoveSound();
		}
		break;

	case STATE_MOVE_END:

		if (m_waitTimer < GetTick(TIMER_SYSTEM))
		{
			m_waitTimer = 0;
			//time to kick into the move loop sound
			GetAudioManager()->Stop(m_transitionSoundHandle);
			m_transitionSoundHandle = AUDIO_HANDLE_BLANK;

			PlayIdleSound();
		}
		break;
	}

}

void LoopingSound::SetTransitionTimings( int transitionStartMS, int transitionStopMS )
{
	m_moveStartTimeMS = transitionStartMS;
	m_moveEndTimeMS = transitionStopMS;

}