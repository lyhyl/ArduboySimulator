This example is a basic game (not a great one..) but shows how to link all kinds of input up using ArcadeInputComponent
and GamepadManager. (which is hooked to ArcadeInputComponent so you get directional movement in a unified way)

* Can control with touch screen d-pad
* Can control with 360 controller on Win
* Can control with trackball or Xperia control pad on Android
* Can control with iCade on android and iOS
* Can control with 60beat controller on iOS (if you don't want to hassle with downloading their library, undefine
  RT_IOS_60BEAT_GAMEPAD_SUPPORT in the xcode project and remove the sbjoystick.* files from the project)
* Music only works on Android/Win because there are only .ogg files, iOS would need .mp3 versions.

This game enables all available gamepads at once.  In a real game, you'd probably want to have finer-tuned on/off
type settings in the options.

Builds tested on Win, Android, iOS, and linux.

For the documentation, tutorials, wiki, and forums, please visit:

www.protonsdk.com

-Seth (seth@rtsoft.com)