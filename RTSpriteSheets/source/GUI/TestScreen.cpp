#include "TestScreen.h"

#include "GUICommon.h"

#include "testfw/ProtonTester.h"
#include "testfw/ProtonTesterGUI.h"

#include "Renderer/SpriteAnimation.h"

void fillAnimationWithFramesWithLengths(SpriteAnimation &animation, unsigned int frameLengths[], unsigned int frameCount)
{
	for (int i = 0; i < frameCount; ++i) {
		SpriteFrame frame(frameLengths[i]);

		for (int j = 0; j < i; ++j) {
			frame.AddCell(SpriteCell("", 0, 0, 0, 0));
		}

		animation.AddFrame(frame);
	}
}

TEST(Sprite_animation_returns_first_frame_for_phases_less_than_0_5_for_an_animation_with_two_frames_and_a_length_of_two)
{
	SpriteAnimation animation;

	unsigned int frameLengths[] = { 1, 1 };
	fillAnimationWithFramesWithLengths(animation, frameLengths, 2);

	TestEq((unsigned int)0, animation.GetFrameAtPhase(0.49f)->GetCellCount());
}

TEST(Sprite_animation_returns_second_frame_for_phases_more_than_0_5_for_an_animation_with_two_frames_and_a_length_of_two)
{
	SpriteAnimation animation;

	unsigned int frameLengths[] = { 1, 1 };
	fillAnimationWithFramesWithLengths(animation, frameLengths, 2);

	TestEq((unsigned int)1, animation.GetFrameAtPhase(0.51f)->GetCellCount());
}

TEST(Sprite_animation_returns_correct_frames_for_phases_for_an_animation_with_three_frames_and_a_length_of_six)
{
	SpriteAnimation animation;

	unsigned int frameLengths[] = { 1, 2, 3 };
	fillAnimationWithFramesWithLengths(animation, frameLengths, 3);

	TestEq((unsigned int)0, animation.GetFrameAtPhase(0.0f)->GetCellCount());
	TestEq((unsigned int)0, animation.GetFrameAtPhase(0.16f)->GetCellCount());
	TestEq((unsigned int)1, animation.GetFrameAtPhase(0.17f)->GetCellCount());
	TestEq((unsigned int)1, animation.GetFrameAtPhase(0.49f)->GetCellCount());
	TestEq((unsigned int)2, animation.GetFrameAtPhase(0.51f)->GetCellCount());
	TestEq((unsigned int)2, animation.GetFrameAtPhase(0.99f)->GetCellCount());
	TestEq((unsigned int)2, animation.GetFrameAtPhase(1.0f)->GetCellCount());
}

Entity* TestScreenCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateCommonControls(pParentEnt);

	ProtonTester::runAllTests();
	ProtonTester::showResultOnScreen(pBG);

	return pBG;
}
