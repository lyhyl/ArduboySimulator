//  ***************************************************************
//  MessageManager - Creation date: 03/18/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef MessageManager_h__
#define MessageManager_h__

#include "util/Variant.h"

class Entity;
class EntityComponent;

enum eMessageClass
{
	MESSAGE_CLASS_GAME, //generic global game-specific messages
	MESSAGE_CLASS_GUI, //these come from GUI events like clicks
	MESSAGE_CLASS_ENTITY, //delivered directly to specific entities
};

enum eTimingSystem
{
	TIMER_SYSTEM,
	TIMER_GAME //message deliveries are paused when the game is paused/slowed down
};


enum eOSSTreamEvent{
	RT_kCFStreamEventNone = 0,
	RT_kCFStreamEventOpenCompleted = 1,
	RT_kCFStreamEventHasBytesAvailable = 2,
	RT_kCFStreamEventCanAcceptBytes = 4, 
	RT_kCFStreamEventErrorOccurred = 8,
	RT_kCFStreamEventEndEncountered = 16
};

enum eVirtualKeys
{
	VIRTUAL_KEY_BACK = 500000,
	VIRTUAL_KEY_PROPERTIES,
	VIRTUAL_KEY_HOME,
	VIRTUAL_KEY_SEARCH
};

enum eMessageType
{
	MESSAGE_TYPE_GUI_CLICK_START,
	MESSAGE_TYPE_GUI_CLICK_END,
	MESSAGE_TYPE_GUI_CLICK_MOVE, //only send when button/finger is held down
	MESSAGE_TYPE_GUI_CLICK_MOVE_RAW, //win only, the raw mouse move messages
	MESSAGE_TYPE_GUI_ACCELEROMETER,
	MESSAGE_TYPE_GUI_CHAR, //the input box uses it on windows since we don't have a virtual keyboard
	MESSAGE_TYPE_GUI_COPY,
	MESSAGE_TYPE_GUI_PASTE,

	MESSAGE_TYPE_SET_ENTITY_VARIANT,
	MESSAGE_TYPE_CALL_ENTITY_FUNCTION,
	MESSAGE_TYPE_CALL_COMPONENT_FUNCTION_BY_NAME,
	MESSAGE_TYPE_PLAY_SOUND,
	MESSAGE_TYPE_VIBRATE,
	MESSAGE_TYPE_REMOVE_COMPONENT,
	MESSAGE_TYPE_ADD_COMPONENT,
	MESSAGE_TYPE_OS_CONNECTION_CHECKED, //sent by macOS, will send an eOSSTreamEvent as parm1
	MESSAGE_TYPE_PLAY_MUSIC,
	MESSAGE_TYPE_UNKNOWN,
	MESSAGE_TYPE_PRELOAD_SOUND,
	MESSAGE_USER = 1000 //users can add their own messages starting here
	
};

class Message: public boost::signals::trackable
{

	public:

	Message(eMessageClass messageClass, eTimingSystem timer, eMessageType type) : m_class(messageClass), m_timerMethod(timer), m_type(type)
	{
		m_deliveryTime = 0; //delivery right away
		m_pTargetEntity = NULL;
		m_pComponent = NULL;
	}

	void SetParm1(int parm1) {m_parm1 = parm1;}
	int GetParm1() {return m_parm1;}
	void SetParm2(int parm2) {m_parm2 = parm2;}
	int GetParm2() {return m_parm2;}
	eTimingSystem GetTimingMethod() {return m_timerMethod;}
	unsigned int GetDeliveryTime() {return m_deliveryTime;}
	eMessageType GetType() {return m_type;}
	eMessageClass GetClass() {return m_class;}
	void SetDeliveryTime(int deliveryTimeMS);
	Variant & Get(){return m_variant;}
	VariantList & GetVariantList(){return m_variantList;}
	void Set(const Variant &v) {m_variant = v;}
	void Set(const VariantList *v) {if (v) m_variantList = *v;}
	void SetTargetEntity(Entity *pEnt);
	Entity * GetTargetEntity() {return m_pTargetEntity;}
	
	void SetTargetComponent(EntityComponent *pComp);
	void SetComponentToAdd(EntityComponent *pComp);
	void ClearComponent() {m_pComponent = NULL;}
	EntityComponent * GetTargetComponent() {return m_pComponent;}
	
	void OnEntityDestroyed(Entity *pEnt);
	void OnComponentDestroyed(VariantList *pVList);
	void SetVarName(string const &varName) {m_varName = varName;}
	string const & GetVarName(){return m_varName;}
	void SetStringParm(string const &msg) {m_stringParm = msg;}
	string const & GetStringParm(){return m_stringParm;}

private:

	eMessageType m_type;
	eTimingSystem m_timerMethod;
	eMessageClass m_class;
	int m_parm1;
	int m_parm2;
	unsigned int m_deliveryTime;
	Variant m_variant;
	VariantList m_variantList;
	Entity *m_pTargetEntity;
	EntityComponent *m_pComponent;
	string m_varName;
	string m_stringParm;
	

};

eTimingSystem GetTiming();

class MessageManager
{
public:
	MessageManager();
	virtual ~MessageManager();

	void SendGUI( eMessageType type, int parm1, int parm2 = 0, int deliverTimeMS = 0, eTimingSystem timing = TIMER_SYSTEM);
	void SendGUI( eMessageType type, const Variant &v, int deliverTimeMS = 0);
	void SendGame( eMessageType type, const string msg, int deliverTimeMS = 0, eTimingSystem timing = TIMER_GAME);
	void SetEntityVariable( Entity *pEnt, int timeMS, const string &varName, const Variant &v, eTimingSystem timing = GetTiming());
	void SetComponentVariable( EntityComponent *pComp, int timeMS, const string &varName, const Variant &v, eTimingSystem timing = GetTiming() );
	void RemoveComponentByName(Entity *pEnt, int timeMS, const string &compName, eTimingSystem timing = GetTiming());
	void CallEntityFunction( Entity *pEnt, int timeMS, const string &funcName, const VariantList *v = NULL, eTimingSystem timing = GetTiming());
	void CallComponentFunction( EntityComponent *pComp, int timeMS, const string &funcName, const VariantList *v = NULL, eTimingSystem timing = GetTiming()); //assumes you know the components address
	void CallComponentFunction( Entity *pEnt, const string &compName, int timeMS,const string &funcName, const VariantList *v = NULL, eTimingSystem timing = GetTiming() ); //useful for calling components that don't exist yet
	void AddComponent(Entity *pEnt, int timeMS, EntityComponent *pComp, eTimingSystem timing = GetTiming());
	
	void DeleteMessagesByFunctionCallName(const string &name, eTimingSystem timing = GetTiming());
	void DeleteMessagesToComponent( EntityComponent *pComponent);
	void DeleteMessagesByType(eMessageType type, eTimingSystem timing = GetTiming());

	void DumpMessages();
	void Update(); //run every tick
	
private:

	void Send(Message *m);
	void AddMessageToList(list <Message*> &messageList, Message *m);
	void DumpMessagesInList(list<Message*> m);
	void Deliver(Message *m);
	void DeleteAllMessages();
	
	//a separate queue for each timing system
	list <Message*> m_gameMessages;
	list <Message*> m_systemMessages;

};

#endif // MessageManager_h__