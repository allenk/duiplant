// magicbox2.cpp : main source file for magicbox2.exe
//

#include "stdafx.h"
#include "DuiSystem.h" 
#include "DuiDefaultLogger.h"
 

#include "MainDlg.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = CoInitialize(NULL);
	DUIASSERT(SUCCEEDED(hRes));
 
	TCHAR szCurrentDir[MAX_PATH+1]; memset( szCurrentDir, 0, sizeof(szCurrentDir) );
	GetModuleFileName( NULL, szCurrentDir, MAX_PATH );
	LPTSTR lpInsertPos = _tcsrchr( szCurrentDir, _T('\\') );
	*lpInsertPos = _T('\0');   

	DuiSystem duiSystem(hInstance);
	DefaultLogger loger;
	loger.setLogFilename(CDuiStringT(szCurrentDir)+_T("\\duiengine.log"));
	duiSystem.SetLogger(&loger);

	duiSystem.logEvent(_T("demo started"));
	
	//使用下面代码从一个XML文件中被化name2id,不使用ID时可以不用,新版本中不推荐使用ID来响应消息
	//pugi::xml_node xmlName2id; //从XML数据中初始化xmlName2id
	//DuiName2ID::getSingletonPtr()->Init(xmlName2id);
#ifdef __DUIFILE_RC 
	//从文件夹加载皮肤,指定皮肤位置
	_tcscat( szCurrentDir, _T("\\..\\skin") );
	DuiResProviderFiles *pResFiles=new DuiResProviderFiles;
	if(!pResFiles->Init(szCurrentDir))
	{
		DUIASSERT(0);
		return 1;
	}
	duiSystem.SetResProvider(pResFiles);
#else 
	//从资源加载皮肤
	duiSystem.SetResProvider(new DuiResProviderPE(hInstance));
#endif 
	BOOL bOK=duiSystem.Init(_T("IDR_DUI_INIT")); //初始化DUI系统,原来的系统初始化方式依然可以使用。
	duiSystem.SetMsgBoxTemplate(_T("IDR_DUI_MSGBOX"));

	int nRet = 0; 
	// BLOCK: Run application
	{
		CMainDlg dlgMain;  
		nRet = dlgMain.DoModal();  
	}

	duiSystem.logEvent(_T("demo end"));

	delete duiSystem.GetResProvider();

	CoUninitialize();
	return nRet;
}
