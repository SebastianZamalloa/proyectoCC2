#pragma once
#include <mysql.h>
#include "image.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> getDataBase(int ID)
{
	MYSQL* conectar;
	conectar = mysql_init(0);
	conectar = mysql_real_connect(conectar, "localhost", "root", "Gregllama_12", "juegocc", 3306, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	vector<string> datos;

	if (conectar)
	{
		string dataTemp = "SELECT * FROM saveData WHERE ID = ";
		auto strInt = to_string(ID);
		dataTemp += strInt;
		const char* name = dataTemp.c_str();
		int qstate = mysql_query(conectar, name);
		if (!qstate)
		{
			res = mysql_store_result(conectar);
			while (row = mysql_fetch_row(res))
			{
				for(int i=0;i<5;i++)
					datos.push_back(row[i+1]);
			}
			return datos;
		}
	}
}
void setDataBase(int ID, vector<string> data)
{
	MYSQL* conectar;
	conectar = mysql_init(0);
	conectar = mysql_real_connect(conectar, "localhost", "root", "Gregllama_12", "juegocc", 3306, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;
	if (conectar)
	{
		string dataTemp = "UPDATE saveData SET allies = '"+data[0]+"', enemies = '"+data[1]+"', background = '"+data[2]+"', buttons = '"+data[3]+"', towers = '"+data[4]+"' WHERE ID = ";
		auto strInt = to_string(ID);
		dataTemp += strInt;
		cout << dataTemp << endl;
		const char* name = dataTemp.c_str();
		int qstate = mysql_query(conectar, name);
	}
}
