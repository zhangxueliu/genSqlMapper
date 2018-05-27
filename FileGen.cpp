#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <algorithm>

#include "FileGen.h"
#include "log.h"
#include "FileOpr.h"
#include "BoostUtils.h"
#include "CamelNameUtils.h"

using namespace std;

CFileGen::CFileGen(string _userId, string _pwd, string _sid, string _ip, const short _port, string _pkg) 
:userId(_userId), pwd(_pwd), sid(_sid), ip(_ip), port(_port), pkg(_pkg)
{
}

CFileGen::~CFileGen()
{
}

void CFileGen::outputAll() {
	char connString[1024] = { 0 };
	_snprintf_s(connString, sizeof(connString),
		"Provider=OraOLEDB.Oracle;\
		DataSource=((DESCRIPTION =(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %d))(CONNECT_DATA =(SERVER = DEDICATED)(SID = %s)))\
		;Persist Security Info=False", (char*)this->ip.c_str(), this->port, (char*)this->sid.c_str());

	bool bConn = dbOper.ConnToDB(connString, (char*)this->userId.c_str(), (char*)this->pwd.c_str());
	if (false == bConn) {
		printf("Error when connect to db!\n");
		return;
	}

	printf("Connect to db successfully.\n");
	vector<string*> vectTables;
	vector<string*> vectPk;
	vector<colField_t*> vectColField;

	try {
		// query all tables on current user
		getTables(vectTables);
		for (int i = 0, size = (int)vectTables.size(); i < size; i++) {
			vectPk.clear();
			vectColField.clear();

			getPkList(*vectTables[i], vectPk);
			getColList(*vectTables[i], vectColField);

			output(vectTables[i]->c_str(), this->pkg, vectPk, vectColField);
		}
	}
	catch (const std::string& e) {
		PrintStringErrorInfo(e);
	}
	catch (_com_error &e) {
		PrintComErrorInfo(e);
	} catch (...) {
		printf("Error when outputAll");
	}
}

void CFileGen::output(string table, string pkg, vector<string*> vectPk, vector<colField_t*> vectColField) {

	string dao(".dal");
	string pkgDao = pkg + dao;
	string className;
	char bufLine[1024 * 2] = {0};

	std::vector<std::string> vec = CBoostUtils::split(table, "_");
	for (int i = 0, size = (int)vec.size(); i < size; i++) {
		transform(vec[i].begin() + 1, vec[i].end(), vec[i].begin() + 1, ::tolower);
		std::cout << vec[i] << std::endl;
	}

	for (int i = 0, size = (int)vec.size(); i < size; i++) {
		className += vec[i];
	}

	string fileext(".xml");
	string filename = this->userId + "\\" +  className + fileext;
	CFileOpr fileOpr(filename.c_str());
	fileOpr.AppendData("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>");
	fileOpr.AppendData("<!DOCTYPE mapper PUBLIC \"-//mybatis.org//DTD Mapper 3.0//EN\" \"http://mybatis.org/dtd/mybatis-3-mapper.dtd\">");

	_snprintf_s(bufLine, sizeof(bufLine), "<mapper namespace=\"%s.mapper.dao.%sMapper\">", pkgDao.c_str(), className.c_str());
	fileOpr.AppendData(bufLine);
	_snprintf_s(bufLine, sizeof(bufLine), "<resultMap id=\"BaseResultMap\" type=\"%s.model.%sDO\">", pkgDao.c_str(), className.c_str());
	fileOpr.AppendData(bufLine);

	bool isPk;
	string pkName, pkType, pkField;
	for (int i = 0, size1 = (int)vectColField.size(); i < size1; i++) {
		isPk = false;
		for (int j = 0, size2 = (int)vectPk.size(); i < size2; j++) {
			if (0 == strncmp(vectPk[i]->c_str(), vectColField[j]->columnName, sizeof(vectColField[j]->columnName))) {
				isPk = true;
			    _snprintf_s(bufLine, sizeof(bufLine), "<id column=\"%s\" property=\"%s\" jdbcType=\"%s\" />"
					, vectColField[j]->columnName, vectColField[j]->columnName, vectColField[j]->type);
				fileOpr.AppendData(bufLine);
				pkName = *vectPk[i];
				pkType = vectColField[j]->type;
				pkField = CamelNameUtils::UnderScore2camel(pkName, pkField);
				break;
			}
		}

		if (isPk) {
		    continue;
		}

		string columnName(vectColField[i]->columnName);
		CamelNameUtils::UnderScore2camel(columnName, columnName);
		_snprintf_s(bufLine, sizeof(bufLine), "<result column=\"%s\" property=\"%s\" jdbcType=\"%s\" />"
			, vectColField[i]->columnName, columnName.c_str(), vectColField[i]->type);
		fileOpr.AppendData(bufLine);
	}

	fileOpr.AppendData("</resultMap>");
   	
	fileOpr.AppendData("<sql id=\"Base_Column_List\">");
	string baseColumns;
	for (int i = 0, size1 = (int)vectColField.size(); i < size1; i++) {
		baseColumns += vectColField[i]->columnName;
		if (i != vectColField.size() - 1) {
			baseColumns += ", ";
		}		
	}
	fileOpr.AppendData(baseColumns);
	fileOpr.AppendData("</sql>");

	// selectByPrimaryKey
	fileOpr.AppendData("<select id = \"selectByPrimaryKey\" parameterType = \"java.lang.String\" resultMap = \"BaseResultMap\">");
	fileOpr.AppendData("select");
	fileOpr.AppendData("<include refid = \"Base_Column_List\" />");
	fileOpr.AppendData("from T_WEB_CONSULT");
	if (string::npos != pkType.find("VARCHAR")) {
		fileOpr.AppendData("where " + pkName + " = #{" + pkField + ", jdbcType = VARCHAR}");
	} else {
		fileOpr.AppendData("where " + pkName + " = #{" + pkField + ", jdbcType = " + pkType + "}");
	}
	
	fileOpr.AppendData("</select>");
	
	// deleteByPrimaryKey
	fileOpr.AppendData("<delete id = \"deleteByPrimaryKey\" parameterType = \"java.lang.String\">");
	fileOpr.AppendData("delete from T_WEB_CONSULT");
	fileOpr.AppendData("where ID = #{" + pkField + ", jdbcType = VARCHAR}");
	fileOpr.AppendData("</delete>");

	// insertSelective
	fileOpr.AppendData("<insert id = \"insertSelective\" parameterType = \"" + pkg + ".dal.model." + className +"DO\">");
	fileOpr.AppendData("<selectKey keyProperty = \"id\" order = \"BEFORE\" resultType = \"java.lang.String\">");
	if (string::npos != pkType.find("VARCHAR")) {
		fileOpr.AppendData("SELECT 3 || TO_CHAR(SYSDATE, 'yymmddhh24mi') || LPAD(" + this->userId + ".SEQ_" + table + "_ID.NEXTVAL, 6, 0) FROM DUAL");
	} else {
		fileOpr.AppendData("SELECT " + this->userId + ".SEQ_" + table + "_ID.NEXTVAL FROM DUAL");
	}

	fileOpr.AppendData("</selectKey>");
	fileOpr.AppendData("insert into " +table);
	fileOpr.AppendData("<trim prefix = \"(\" suffix = \")\" suffixOverrides = \", \">");
	fileOpr.AppendData(pkName + ",");

	for (int i = 0, size1 = (int)vectColField.size(); i < size1; i++) {
		string column_Name(vectColField[i]->columnName);
		string columnName(vectColField[i]->columnName);
		fileOpr.AppendData("<if test=\"" + CamelNameUtils::UnderScore2camel(column_Name, columnName) + " != null and " + columnName + "!= ''\">");
		fileOpr.AppendData(column_Name + ",");
		fileOpr.AppendData("</if>");
	}

	fileOpr.AppendData("</trim>");

	fileOpr.AppendData("<trim prefix = \"values(\" suffix = \")\" suffixOverrides = \", \">");
	fileOpr.AppendData("#{" + pkField + ",jdbcType=VARCHAR},");

	for (int i = 0, size1 = (int)vectColField.size(); i < size1; i++) {
		string column_Name(vectColField[i]->columnName);
		string columnName(vectColField[i]->columnName);
		fileOpr.AppendData("<if test=\"" + CamelNameUtils::UnderScore2camel(column_Name, columnName) + " != null and " + columnName + "!= ''\">");
		fileOpr.AppendData("#{" + columnName + ",jdbcType=VARCHAR},");
		fileOpr.AppendData("</if>");
	}

	fileOpr.AppendData("</trim>");
	fileOpr.AppendData("</insert>");

		
	// updateByPrimaryKeySelective
	fileOpr.AppendData("<update id = \"updateByPrimaryKeySelective\" parameterType = \"" + pkg + ".dal.model." + className + "DO\">");
	fileOpr.AppendData("update "+table);
	fileOpr.AppendData("<set>");

	for (int i = 0, size1 = (int)vectColField.size(); i < size1; i++) {
		string column_Name(vectColField[i]->columnName);
		string columnName(vectColField[i]->columnName);
		fileOpr.AppendData("<if test=\"" + CamelNameUtils::UnderScore2camel(column_Name, columnName) + " != null and " + columnName + "!= ''\">");
		fileOpr.AppendData(column_Name + " = #{" + columnName + ", jdbcType = VARCHAR},");
		fileOpr.AppendData("</if>");
	}

	fileOpr.AppendData("</set>");
	if (string::npos != pkType.find("VARCHAR")) {
		fileOpr.AppendData("where " + pkName + " = #{" + pkField + ", jdbcType = VARCHAR}");
	}
	else {
		fileOpr.AppendData("where " + pkName + " = #{" + pkField + ", jdbcType = " + pkType + "}");
	}
	fileOpr.AppendData("</update>");

	// add tail here
	fileOpr.AppendData("</mapper>");

	fileOpr.SaveToFile();
}

void CFileGen::getTables(vector<string *> &vectTable) {

	char sql[1024 * 2] = { 0 };
	_snprintf_s(sql, sizeof(sql) - 1,
		" select" \
		" object_name name" \
		" from all_objects"
		" where 1 = 1" \
		" and object_type = 'TABLE'"
		" and owner = '%s'"
		, this->userId.c_str()
		);

	_RecordsetPtr pRst = dbOper.ExecuteWithResSQL(sql);
	if (NULL == pRst) {
		throw exception("Error when query data£¡\n");
	}

	if (pRst->adoEOF) {
		pRst->Close();
		throw exception("There is no records in this table\n");
	}

	printf("querying...\n");
	_variant_t vName;
	pRst->MoveFirst();
	while (!pRst->adoEOF) {
		vName = pRst->GetCollect(_variant_t("name"));
		printf("%s\t", (LPSTR)(LPCSTR)(_bstr_t)vName);
		printf("\n");

		string *pkName = new string((LPSTR)(LPCSTR)(_bstr_t)vName);
		vectTable.push_back(pkName);
		pRst->MoveNext();
	}

	pRst->Close();
}


void CFileGen::getPkList(string tableName, vector<string *> &vectPk) {

	char sql[1024 * 3] = { 0 };
	_snprintf_s(sql, sizeof(sql) - 1,
		" select" \
		" a.constraint_name constraint, a.column_name name" \
		" from user_cons_columns a"
		" inner join user_constraints b on a.constraint_name = b.constraint_name" \
		" where 1 = 1" \
		" and b.constraint_type = 'P'"
		" and a.table_name = '%s'"
		, tableName.c_str()
		);
	OutputDebugPrintf(sql);
	_RecordsetPtr pRst = dbOper.ExecuteWithResSQL(sql);
	if (NULL == pRst) {
		throw exception("Error when query data£¡\n");
	}

	if (pRst->adoEOF) {
		pRst->Close();
		throw exception("There is no records in this table\n");
	}

	printf("querying...\n");
	_variant_t constraint, vName, nullable, defaultV, comments;
	pRst->MoveFirst();
	while (!pRst->adoEOF) {
		constraint = pRst->GetCollect(_variant_t("constraint"));
		vName = pRst->GetCollect(_variant_t("name"));
		printf("%s\t", constraint);
		printf("%s\t", (LPSTR)(LPCSTR)(_bstr_t)vName);
		printf("\n");

		string *pkName = new string((LPSTR)(LPCSTR)(_bstr_t)vName);
		vectPk.push_back(pkName);
		pRst->MoveNext();
	}

	pRst->Close();
}



void CFileGen::getColList(string tableName, vector<colField_t*> &colFieldList) {
	char sql[1024 * 2] = { 0 };
	_snprintf_s(sql, sizeof(sql),
		" select" \
		" utc.COLUMN_NAME columnName, utc.DATA_TYPE type, utc.NULLABLE nullable, utc.DATA_DEFAULT defaultValue, ucc.COMMENTS comments" \
		" from user_tab_columns utc"
		" inner join user_col_comments ucc on utc.table_NAME = ucc.table_name and utc.COLUMN_NAME = ucc.column_name" \
		" where 1 = 1" \
		" and utc.table_name = '%s'"
		, tableName.c_str()
		);

	_RecordsetPtr pRst;
	pRst = dbOper.ExecuteWithResSQL(sql);
	if (NULL == pRst) {
        throw exception("Error when query data£¡\n");
	}

	if (pRst->adoEOF) {
		pRst->Close();
        throw exception("There is no records in this table\n");
	}

	printf("querying...\n");
	colField_t * pcolField_t = NULL;
	pRst->MoveFirst();
	while (!pRst->adoEOF) {
		pcolField_t = new colField_t();
		copyTableField(pcolField_t->columnName, pRst->GetCollect(_variant_t("columnName")), sizeof(pcolField_t->columnName));
		copyTableField(pcolField_t->type, pRst->GetCollect(_variant_t("type")), sizeof(pcolField_t->type));
		copyTableField(pcolField_t->nullable, pRst->GetCollect(_variant_t("nullable")), sizeof(pcolField_t->nullable));
		copyTableField(pcolField_t->defaultValue, pRst->GetCollect(_variant_t("defaultValue")), sizeof(pcolField_t->defaultValue));
		copyTableField(pcolField_t->comments, pRst->GetCollect(_variant_t("comments")), sizeof(pcolField_t->comments));

		colFieldList.push_back(pcolField_t);
		pRst->MoveNext();
	}

	pRst->Close();
}


void CFileGen::copyTableField(char *dest, _variant_t srcFiled, int maxlen){
	if (VT_NULL != srcFiled.vt && VT_EMPTY != srcFiled.vt) {
		printf("%s\t", (LPSTR)(LPCSTR)(_bstr_t)srcFiled);
		strncpy_s(dest, maxlen, (LPSTR)(LPCSTR)(_bstr_t)srcFiled, maxlen);
	}
}
