// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "FileGen.h"
#include "resource.h"
#include "atlmisc.h"
#include <winuser.h>

#include <string>  
#include <algorithm>  
using namespace std;

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	//
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_BUTTON_GEN, OnCodeGen)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)



	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		SetDlgText(IDC_EDIT_IP, _T("127.0.0.1"));
		SetDlgText(IDC_EDIT_PACKAGE, _T("com.test.proj"));

		return TRUE;
	}

	void SetDlgText(unsigned int resourceId, TCHAR * tcText) {
		CEdit ceditId = (CEdit)GetDlgItem(resourceId);
		::SetWindowTextW(ceditId, tcText);
	}

	void GetDlgText(unsigned int resourceId, TCHAR * tcText, int lenText) {
		CEdit ceditId = (CEdit)GetDlgItem(resourceId);
		::GetWindowTextW(ceditId, tcText, lenText);
	}

	std::string TCHAR2STRING(TCHAR *tcString, string &str)
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, tcString, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen*sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, tcString, -1, chRtn, iLen, NULL, NULL);
		str = chRtn;
		//std::string str(chRtn);
		delete chRtn;
		return str;
	}

	std::string GetTheDlgText(unsigned int resourceId, string &str, int lenText) {
		TCHAR * tcText = new TCHAR[lenText + 1];
		memset(tcText, 0, lenText + 1);
		this->GetDlgText(resourceId, tcText, lenText);
		this->TCHAR2STRING(tcText, str);
		delete tcText;
		return str;
	}


	LRESULT OnCodeGen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		string userId, pwd,
			   ip, sid,
			   package,
			   port
		;		
		short nPort = 2521;

		GetTheDlgText(IDC_EDIT_USERID, userId, 255);
		GetTheDlgText(IDC_EDIT_PWD, pwd, 255);
		GetTheDlgText(IDC_EDIT_IP, ip, 255);
		GetTheDlgText(IDC_EDIT_SID, sid, 255);
		GetTheDlgText(IDC_EDIT_PACKAGE, package, 255);
		GetTheDlgText(IDC_EDIT_PORT, port, 255);

		if (0 == userId.length()) {
			MessageBox(_T("数据库用户名不能为空!"), _T("参数错误"), MB_ICONWARNING);
			return 0;
		} else if (0 == pwd.length()) {
			MessageBox(_T("密码不能为空!"), _T("参数错误"), MB_ICONWARNING);
			return 0;
		} else if (0 == ip.length()) {
			MessageBox(_T("IP不能为空!"), _T("参数错误"), MB_ICONWARNING);
			return 0;
		} else if (0 == sid.length()) {
			MessageBox(_T("SID不能为空!"), _T("参数错误"), MB_ICONWARNING);
			return 0;
		} else if (0 == package.length()) {
			MessageBox(_T("代码包路径不能为空(如:com.ob.proj)!"), _T("参数错误"), MB_ICONWARNING);
			return 0;
		}

		UINT nRet = -1;
		if (0 == port.length()) {
			CString str;
			str.Format(_T("端口默认:2521"));
			nRet = MessageBox(str, _T("提示"), MB_YESNO);
		}
		if (nRet == IDYES) {
		} else if (nRet == IDNO) {
			return 0;
		}

		nPort = atoi(port.c_str());

		transform(userId.begin(), userId.end(), userId.begin(), toupper);
		CFileGen t1(userId, pwd, sid, ip, nPort, package);
		t1.outputAll();

		return 0;
    }

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DestroyWindow();
		::PostQuitMessage(0);
		return 1;
	}

	LRESULT OnExit(WORD wNotify, WORD wID, HWND hCtrl, BOOL &bHandeld)
	{
		GetParent().PostMessage(WM_CLOSE, 0, 0);
		return 0;
	}
};
