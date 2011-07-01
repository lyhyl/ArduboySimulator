
//  ***************************************************************
//  MySQLManager - Creation date: 04/01/2011
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2011 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

//an interface to use mysql.  For this to compile you have to have mysql/include (from mysql's source dir) added to your paths and link in
//libmysql.lib (win) or add mysqlclient library to the make file if linux.  Design to also work in console mode. (_CONSOLE defined)

//Useful tip: It can return a select query as a vector VariantDBs

#ifndef MySQLManager_h__
#define MySQLManager_h__

#include <mysql.h>
#include "Manager/VariantDB.h"

class MySQLManager
{
public:
	MySQLManager();
	virtual ~MySQLManager();
	bool Init(string name, string password);
	void Kill();
	void ShowError();
	bool Query(string query);
	bool DoesTableExist(string tableName);
	int GetLastAutoIncrementInsertID();
	int AddSelectResults(vector<VariantDB> &vdb); //adds to existing vector, returns how many items it added
	int GetArrayCountOfLastQuery();
	void Update(); //call every frame, it avoids disconnection by pinging its sql connection every once in a while
	
protected:
	

private:
	MYSQL *m_conn;
	uint32 m_pingTimer; //do a query every 4 hours to avoid being disconnected
	
};

#endif // MySQLManager_h__