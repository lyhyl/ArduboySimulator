#include "PlatformPrecomp.h"
#include "AboutMenu.h"
#include "MainMenu.h"
#include "Entity/EntityUtils.h"


void AboutMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->Get(1).GetEntity();
	LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[1].Print().c_str());
	Entity *pMenu = GetEntityRoot()->GetEntityByName("AboutMenu"); //we're sort of cheating by just grabbing the top level parent
	//entity by name instead of GetParent() a bunch of times to reach the top level, but meh
	
	
	if (pEntClicked->GetName() == "Back")
	{
		DisableAllButtonsEntity(pEntClicked->GetParent());
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MainMenuCreate(pEntClicked->GetParent()->GetParent());
		return;
	}

	if (pEntClicked->GetName() == "plogo")
	{
		//Let's add an entity to the scroll window saying something
		CL_Vec2f vPos = pEntClicked->GetVar("pos2d")->GetVector2();
		
		//add a text message that types itself onto the screen.  Slightly randomize its position
		Entity *pEnt = CreateTextLabelEntity(pEntClicked->GetParent(), "tempText", vPos.x+170, vPos.y+50+RandomRangeFloat(-50,50), "`$You `#clicked`` the logo!");
		TypeTextLabelEntity(pEnt, 0, 50); //make it type out the text
		FadeOutAndKillEntity(pEnt, true, 500, 3000);

		OneTimeBobEntity(pEntClicked, -20, 5, 250);
		//let's have the logo bob as well, I mean, it's easy to do
		return;
	}


	GetEntityRoot()->PrintTreeAsText(); //useful for debugging
}

void AboutMenuAddScrollContent(Entity *pParent)
{
	pParent = pParent->GetEntityByName("scroll_child");
	pParent->RemoveAllEntities(); //clear it out in case we call this more than once, say, to update/change something

	float x = 5; //inset
	float y = 0;
	float spacerY = 30; //space between thingies

	//first, a title in a big font
	Entity *pTitle = CreateTextLabelEntity(pParent, "Title", x, 0, "About & Help"); 
	SetupTextEntity(pTitle, FONT_LARGE);
	y += pTitle->GetVar("size2d")->GetVector2().y +spacerY;

	//define an area for a text box, so it will wrap in  the right places.  Height is actually ignored.
	CL_Vec2f vTextBoxPos(x,y);
	CL_Vec2f vTextBounds(434, 200);
	string msg; //we could load text, but let's just manually put junk in there:

	msg += \
		"About this example:\n\n"\
		"Particle Test ``-`7 It shows the built in 2d particle system.  Tap anywhere to cause an explosion.``\n\n"\
		"Input Test ``- `7This option demonstrates getting input from the user.  The user must tap/click the input field to modify its text.  A native keyboard will pop up if needed.\n\n``"\
		"Debug and Music Test ``- `7This option lets you groove to some background techno music.  It's an mp3 on iOS and Windows, or loaded as .ogg on Android.\n\n``"\
		"About (scroll bar test) ``- `7Why, that's this!  It illustrates using a scroll bar.  You can flick up and down.  Hmm, maybe I should make it flick backwards on Windows, as it feels wrong.\n\n``"\
		"Also, notice that you can add any kind of entity within a scroll box, here is a clickable image:";

		//actually create the text box with the above text

	Entity *pEnt = CreateTextBoxEntity(pParent, "SomeTextBox", vTextBoxPos, vTextBounds, msg);
	y += pEnt->GetVar("size2d")->GetVector2().y; //move our Y position down the exact size of the text
	y += spacerY; //don't forget our spacer

	//add an image
	pEnt = CreateOverlayButtonEntity(pParent, "plogo", "interface/proton-128.rttex", x, y);
	pEnt->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&AboutMenuOnSelect);

	//do something if someone clicks it

	//automatically calculate the total size of this entity with all its children for the scroll bars, do this at the end
	ResizeScrollBounds(&VariantList(pParent->GetParent()));
}



Entity * AboutMenuCreate( Entity *pParentEnt)
{
	Entity *pBG = NULL;
	pBG =  CreateOverlayEntity(pParentEnt, "AboutMenu", "interface/bkgd_stone.rttex", 0,0);
	AddFocusIfNeeded(pBG, true, 500);
	pBG->AddComponent(new FocusRenderComponent);

	//setup the dimensions of where the scroll area will go
	CL_Vec2f vTextAreaPos = iPhoneMap(2,10);
	float offsetFromBottom = iPhoneMapY(42);
	float offsetFromRight = iPhoneMapY(0);

	CL_Vec2f vTextAreaBounds = (GetScreenSize()- CL_Vec2f(offsetFromRight,offsetFromBottom))-vTextAreaPos;
	Entity *pScroll = pBG->AddEntity(new Entity("scroll"));
	pScroll->GetVar("pos2d")->Set(vTextAreaPos);
	pScroll->GetVar("size2d")->Set(vTextAreaBounds);
	pScroll->AddComponent(new TouchHandlerComponent);

	EntityComponent *pFilter = pScroll->AddComponent(new FilterInputComponent);
	EntityComponent *pScrollComp = pScroll->AddComponent(new ScrollComponent);
	
	//note: If you don't want to see a scroll bar progress indicator, comment out the next line.  Also note that it only draws
	//a vertical progress bar if needed but doesn't draw a horizontal if needed (I just haven't needed a horizontal scroll bar yet)
	EntityComponent *pScrollBarComp = pScroll->AddComponent(new ScrollBarRenderComponent); 	//also let's add a visual way to see the scroller position
	//pScroll->GetVar("color")->Set(MAKE_RGBA(61,155, 193, 255)); 
	Entity *pScrollChild = pScroll->AddEntity(new Entity("scroll_child"));
	
	/*
	//this is one way to clip the image on the bottom - glClipPlane is too slow/broken on Android so we actually won't use it
	EntityComponent *pClip = pScroll->AddComponent(new RenderClipComponent);
	pClip->GetVar("clipMode")->Set(uint32(RenderClipComponent::CLIP_MODE_BOTTOM));
	*/

	//another way would be to just blit a colored bar over the bottom:
	//pEnt = CreateOverlayRectEntity(pBG, CL_Rectf(0, GetScreenSizeYf()-offsetFromBottom, GetScreenSizeXf(), 320), MAKE_RGBA(0,0,0,100));

	//but the nicest way is to blit a matching bar at the bottom with transparency:
	Entity *pOverlay = CreateOverlayEntity(pBG, "", "interface/bg_stone_overlay.rttex", 0, GetScreenSizeYf()+1); 
	SetAlignmentEntity(pOverlay, ALIGNMENT_DOWN_LEFT);
	
	//actually add all our content
	AboutMenuAddScrollContent(pBG);


	//oh, let's put the Back button on the bottom bar thing
	Entity * pEnt = CreateTextButtonEntity(pBG, "Back", 20, GetScreenSizeYf()-30, "Back", false);
	pEnt->GetFunction("OnButtonSelected")->sig_function.connect(&AboutMenuOnSelect);
	SetupTextEntity(pEnt, FONT_SMALL);
	AddHotKeyToButton(pEnt, VIRTUAL_KEY_BACK); //for androids back button and window's Escape button

	SlideScreen(pBG, true, 500);
	return pBG;
}