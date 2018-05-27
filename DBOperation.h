#pragma once
class CDBOperation
{
public:
	CDBOperation(void);
	~CDBOperation(void);

	bool ConnToDB(char *ConnectionString, char *UserID, char*Password);

	_RecordsetPtr ExecuteWithResSQL(const char *);

private:
	_ConnectionPtr CreateConnPtr();
	_CommandPtr CreateCommPtr();
	_RecordsetPtr CreateRecsetPtr();

private:
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;

};

