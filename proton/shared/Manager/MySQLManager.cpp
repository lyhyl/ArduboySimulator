#include "PlatformPrecomp.h"
#include "MySQLManager.h"

#define HAVE_INT32
#define HAVE_UINT32
#define byte_defined
#include <my_global.h>
#include <mysql.h>

#define C_MYSQL_PING_TIMER_MS (1000*60*60)*4

MySQLManager::MySQLManager()
{
	m_conn = NULL;
	m_pingTimer = 0;
}

MySQLManager::~MySQLManager()
{
	Kill();
}

void MySQLManager::Kill()
{
	if (m_conn)
	{
		mysql_close(m_conn);
		m_conn = NULL;
	}
}

void MySQLManager::ShowError()
{
	LogError("MySQLManager error: %u: %s", mysql_errno(m_conn), mysql_error(m_conn));
}

bool MySQLManager::DoesTableExist(string tableName)
{
	assert(m_conn);
	MYSQL_RES *result = NULL;

	bool bSuccess = Query("SELECT * FROM "+tableName);

	if (!bSuccess) return false;

	result = mysql_store_result(m_conn);
	int fields = mysql_num_fields(result);
	mysql_free_result(result);
	return bSuccess;
}

bool MySQLManager::DoResultsExist()
{
	MYSQL_RES *result = NULL;
	result = mysql_store_result(m_conn);

	if (!result)
	{
		return false;
	}
	int rows = (int)mysql_num_rows(result);

	return rows != 0;
}


int MySQLManager::AddSelectResults(vector<VariantDB> &vdb)
{
	MYSQL_RES *result = NULL;
	result = mysql_store_result(m_conn);

	if (!result)
	{
		return 0;
	}

	int num_fields = mysql_num_fields(result);
	int rows = (int)mysql_num_rows(result);

	MYSQL_ROW row;
	MYSQL_FIELD *field;
	
	vector<string> fieldNames;
	vector<enum_field_types> fieldType;

	int curRow = (int)vdb.size();
	vdb.resize(curRow+rows);

	while ((row = mysql_fetch_row(result)))
	{
	
	  VariantDB &db = vdb[curRow++];

	  for(int i = 0; i < num_fields; i++)
		{
			if (i == 0) 
			{
				while(field = mysql_fetch_field(result)) 
				{
					fieldNames.push_back(field->name);
			//		printf("%s ", field->name);
					fieldType.push_back(field->type);
				}
		//		printf("\n");
			}
		//	printf("%s  ", row[i] ? row[i] : "NULL");
			switch(fieldType[i])
			{

			case FIELD_TYPE_DECIMAL:
			case FIELD_TYPE_FLOAT:
				db.GetVar(fieldNames[i])->Set((float)atof(row[i]));
				break;

			case FIELD_TYPE_SHORT:
			case FIELD_TYPE_LONG:
				db.GetVar(fieldNames[i])->Set((int32)atoi(row[i]));
				break;

			default: //assume string
				db.GetVar(fieldNames[i])->Set(string(row[i]));
			}

		}
	}
	printf("\n");

	mysql_free_result(result);

  return rows;
}

bool MySQLManager::Init(string name, string password)
{
	LogMsg("MySQL client version: %s", mysql_get_client_info());
	Kill();
	
	
	m_conn = mysql_init(NULL);
	
	if (!m_conn)
	{
		ShowError();
		return false;
	}

	//actually connect?

	if (!mysql_real_connect(m_conn, "localhost", name.c_str(), password.c_str(), NULL, 0, NULL, 0))
	{
		ShowError();
		return false;
	}

	return true;
}

bool MySQLManager::Query( string query )
{
#ifdef _DEBUG
	//LogMsg("Queting %s", query.c_str() );
#endif
	if (mysql_query(m_conn, query.c_str()))
	{
		ShowError();
		return false;
	}

	return true;
}

int MySQLManager::GetArrayCountOfLastQuery()
{
	MYSQL_RES *result = NULL;
	result = mysql_store_result(m_conn);

	//MYSQL_ROW row = mysql_fetch_a
	return 0;

}

int MySQLManager::GetLastAutoIncrementInsertID()
{
	return (int)mysql_insert_id(m_conn);
}

void MySQLManager::Update()
{

	if (m_conn && m_pingTimer < GetSystemTimeTick())
	{
		LogMsg("Ping!");
		DoesTableExist("BogusTable");
		m_pingTimer = GetSystemTimeTick()+C_MYSQL_PING_TIMER_MS;
	}
}