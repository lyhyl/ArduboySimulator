/* ============================================================================================ */
/* FMOD iPhone Specific header file. Copyright (c), Firelight Technologies Pty, Ltd. 2005-2009. */
/* ============================================================================================ */

#ifndef _FMODIPHONE_H
#define _FMODIPHONE_H

#include "fmod.h"


/*
[ENUM]
[
    [DESCRIPTION]
    This will set the default audio session category to use for your application, see
    the Apple documentation for detailed information about what each audio session
    category does.
 
    [REMARKS]
    When using the recording API, FMOD will automatically switch to the "PlayAndRecord"
    audio session category then switch back when recording is complete.
 
    [PLATFORMS]
    iPhone

    [SEE_ALSO]
]
*/
typedef enum
{
    FMOD_IPHONE_SESSIONCATEGORY_DEFAULT,            /* Default for the device OS version (MediaPlayback for iPhone OS 2.0 to 2.1, SoloAmbientSound for iPhone OS 2.2 onwards) */
    FMOD_IPHONE_SESSIONCATEGORY_AMBIENTSOUND,       /* kAudioSessionCategory_AmbientSound ~ obeys slient switch, silent when locked, mixes with other audio */
    FMOD_IPHONE_SESSIONCATEGORY_SOLOAMBIENTSOUND,   /* kAudioSessionCategory_SoloAmbientSound (iPhone OS >= 2.2 required) ~ obeys slient switch, silent when locked, doesn't mix with other audio */
    FMOD_IPHONE_SESSIONCATEGORY_MEDIAPLAYBACK,      /* kAudioSessionCategory_MediaPlayback ~ ignores slient switch, plays when locked, doesn't mix with other audio */
    FMOD_IPHONE_SESSIONCATEGORY_PLAYANDRECORD,      /* kAudioSessionCategory_PlayAndRecord ~ ignores slient switch, plays when locked, doesn't mix with other audio */    
    
    FMOD_IPHONE_SESSIONCATEGORY_FORCEINT = 65536    /* Makes sure this enum is signed 32bit */
} FMOD_IPHONE_SESSIONCATEGORY;


/*
[STRUCTURE] 
[
    [DESCRIPTION]
    Use this structure with System::init to set the information required for
    iPhone initialisation.

    Pass this structure in as the "extradriverdata" parameter in System::init.

    [REMARKS]

    [PLATFORMS]
    iPhone

    [SEE_ALSO]
    System::init
]
*/
typedef struct FMOD_IPHONE_EXTRADRIVERDATA
{
    FMOD_IPHONE_SESSIONCATEGORY  sessionCategory;        /* (IN)  Default audio session category to use for output */
    bool                         forceSpeakerOutput;     /* (IN)  Force audio out the main speaker, don't allow output to be redirected to the receiver speaker when recording (iPhone OS >= 2.1 required) */
    bool                        *otherAudioPlaying;      /* (OUT) Whether an external audio source (i.e. iPod) is playing after System::init is complete. */  
} FMOD_IPHONE_EXTRADRIVERDATA;

#endif

