#include "PlatformPrecomp.h"
#include "EnterNameMenu.h"
#include "Entity/EntityUtils.h"
#include "MainMenu.h"

string HighScoreNameFilter(string s)
{
	string temp;

	for (unsigned int i=0; i < s.length(); i++)
	{
		if ( 
			(s[i] >= 65 && s[i] <= 90)
			|| (s[i] >= 97 && s[i] <= 122)
			)
		{
			temp += s[i];		
		}
	}

	return temp;
}

void EnterNameMenuOnSelect(VariantList *pVList) //0=vec2 point of click, 1=entity sent from
{
	Entity *pEntClicked = pVList->m_variant[1].GetEntity();

	//LogMsg("Clicked %s entity at %s", pEntClicked->GetName().c_str(),pVList->m_variant[0].Print().c_str());
	if (pEntClicked->GetName() == "Continue")
	{
		string name = GetEntityRoot()->GetEntityByName("name_input_box")->GetComponentByName("InputTextRender")->GetVar("text")->GetString();
		GetApp()->GetVar("name")->Set(HighScoreNameFilter(name)); //save it to our database so we can remember the default
		LogMsg("Read %s for the name", GetApp()->GetVar("name")->GetString().c_str());
		SlideScreen(pEntClicked->GetParent(), false);
		GetMessageManager()->CallEntityFunction(pEntClicked->GetParent(), 500, "OnDelete", NULL);
		MainMenuCreate(pEntClicked->GetParent()->GetParent());
	}
}

Entity * EnterNameMenuCreate(Entity *pParentEnt)
{
	Entity *pBG = CreateOverlayEntity(pParentEnt, "EnterNameMenu", "interface/summary_bg.rttex", 0,0);
	AddFocusIfNeeded(pBG);

	Entity *pButtonEntity;
	CL_Vec2f vTextAreaPos = CL_Vec2f(45,40);
	CL_Vec2f vTextAreaBounds = CL_Vec2f(384,234);
	
	string title = "`$New Highscore!";
	pButtonEntity = CreateTextLabelEntity(pBG, "title", vTextAreaPos.x, vTextAreaPos.y, title);
	pButtonEntity->GetComponentByName("TextRender")->GetVar("font")->Set(uint32(FONT_LARGE));
	pButtonEntity->GetVar("scale2d")->Set(CL_Vec2f(0.6f, 0.6f));
	vTextAreaPos.y += 25;
	
	string msg = "Your score of `w"+FloatToMoney(500000)+"`` is a high score!  `wTap below`` to set your name.";
	Entity *pText = CreateTextBoxEntity(pBG, "text", vTextAreaPos, vTextAreaBounds, msg);

	//the continue button
	pButtonEntity = CreateOverlayButtonEntity(pBG, "Continue","interface/summary_continue.rttex", 100, 282);
	pButtonEntity->GetShared()->GetFunction("OnButtonSelected")->sig_function.connect(&EnterNameMenuOnSelect);
	AddHotKeyToButton(pButtonEntity, VIRTUAL_KEY_BACK); //for android's back button, or escape key in windows

	pButtonEntity = CreateTextLabelEntity(pBG, "name", vTextAreaPos.x, vTextAreaPos.y+73, "`$Name: ");
//	pButtonEntity->GetComponentByName("TextRender")->GetVar("font")->Set(uint32(FONT_LARGE));
	float nameWidth = pButtonEntity->GetVar("size2d")->GetVector2().x;

	//create input box
	pButtonEntity = CreateInputTextEntity(pBG, "name_input_box", vTextAreaPos.x+nameWidth, vTextAreaPos.y+71, GetApp()->GetShared()->GetVarWithDefault("name", string("Player"))->GetString());
	
	//if you wanted to allow spaces and other symbols, change to "loose" filtering like this:
	//pButtonEntity->GetComponentByName("InputTextRender")->GetVar("filtering")->Set(uint32(InputTextRenderComponent::FILTERING_LOOSE));

	//you also need to do this for loose filtering to make sure the best keyboard is chosen on the device
	//pButtonEntity->GetComponentByName("InputTextRender")->GetVar("inputType")->Set(uint32(InputTextRenderComponent::INPUT_TYPE_ASCII_FULL));

	//if you wanted text that appear until it's activated:
	//pButtonEntity->GetComponentByName("InputTextRender")->GetVar("placeHolderText")->Set("Tap here to enter your name");


	
	//pButtonEntity->GetComponentByName("InputTextRender")->GetVar("font")->Set(uint32(FONT_LARGE));

	SlideScreen(pBG, true);
	return pBG;
}


