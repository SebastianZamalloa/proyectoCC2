#pragma once
#include <mysql.h>
#include "image.h"
#include <iostream>
#include <string>
using namespace std;
template<typename T>
T getDataIndicator(int ID, string dataName)
{
	MYSQL* conectar;
	conectar = mysql_init(0);
	conectar = mysql_real_connect(conectar, "localhost", "root", "Gregllama_12", "juegocc", 3306, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;

	if (conectar)
	{
		string dataTemp = dataName;
		dataTemp = "SELECT " + dataName + " FROM indicatorsCharacter";
		const char* name = dataTemp.c_str();
		int qstate = mysql_query(conectar, name);
		if (!qstate)
		{
			int idCount = 1;
			res = mysql_store_result(conectar);
			while (row = mysql_fetch_row(res))
			{
				if (idCount == ID)
				{
					string temp = row[0];
					T tempInt = stof(temp);
					return tempInt;
				}
				idCount++;
			}
		}
	}
}

int getAnimationFromDB(int ID, string dataName)
{
	MYSQL* conectar;
	conectar = mysql_init(0);
	conectar = mysql_real_connect(conectar, "localhost", "root", "Gregllama_12", "juegocc", 3306, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;

	if (conectar)
	{
		string dataTemp = dataName;
		dataTemp = "SELECT " + dataName + " FROM animationCharacter";
		const char* name = dataTemp.c_str();
		int qstate = mysql_query(conectar, name);
		if (!qstate)
		{
			int idCount = 1;
			res = mysql_store_result(conectar);
			while (row = mysql_fetch_row(res))
			{
				if (idCount == ID)
				{
					string temp = row[0];
					int tempInt = stoi(temp);
					return tempInt;
				}
				idCount++;
			}
		}
	}
}
int* getArrayAnimationFromDB(int ID, string dataName, int countFrames)
{
	MYSQL* conectar;
	conectar = mysql_init(0);
	conectar = mysql_real_connect(conectar, "localhost", "root", "Gregllama_12", "juegocc", 3306, NULL, 0);
	MYSQL_ROW row;
	MYSQL_RES* res;

	if (conectar)
	{
		string dataTemp = dataName;
		dataTemp = "SELECT " + dataName + " FROM animationCharacter";
		const char* name = dataTemp.c_str();
		int qstate = mysql_query(conectar, name);
		if (!qstate)
		{
			int idCount = 1;
			res = mysql_store_result(conectar);
			while (row = mysql_fetch_row(res))
			{
				if (idCount == ID)
				{
					string temp = row[0];
					int j = 1;
					int numCount = countFrames-1;
					int* numbers = new int[countFrames];
					for (int i = 0; i < countFrames; i++)
						numbers[i] = 0;
					for (int i = temp.length()-1; i>=0; i--)
					{
						if (temp[i] == ',')
						{
							j = 1;
							numCount--;
						}
						else
						{
							int numTemp = temp[i]-'0';
							numbers[numCount] += j*numTemp;
							j *= 10;
						}
					}
					return numbers;
				}
				idCount++;
			}
		}
	}
}

