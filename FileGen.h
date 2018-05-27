#pragma once
#include "stdafx.h"

#include <string>
#include <vector>
#include "DBOperation.h"
#include "ColField.h"

using namespace std;

class CFileGen
{
public:
	CFileGen(string userId, string pwd, string sid, string ip, const short port, string _pkg);
	~CFileGen();

public:
	void outputAll();
	void output(string table, string pkg, vector<string*> vectPk, vector<colField_t*> vectColField);
	void getPkList(string tableName, vector<string*> &vectPk);
	void getColList(string tableName, vector<colField_t*> &colFieldList);
	void getTables(vector<string *> &vectTable);

private:
	void copyTableField(char * dest, _variant_t filed, int maxlen);
	

private:
	CDBOperation dbOper;
	string userId;
	string pwd;
	string sid; 
	string ip;
	short port;
	string pkg;
};
