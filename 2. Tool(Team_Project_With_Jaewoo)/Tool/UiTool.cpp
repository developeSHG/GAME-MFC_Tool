// UiTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UiTool.h"
#include "MyTextureMgr.h"
#include "MySingleTexture.h"
#include "MyMultiTexture.h"
#include "RenderMgr.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "afxdialogex.h"


// CUiTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUiTool, CDialog)

CUiTool::CUiTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UITOOL, pParent)
{
	
}

CUiTool::~CUiTool()
{
	for (auto& MyMap : m_mapUiPos)
	{
		SAFE_DELETE(MyMap.second);
	}
	m_mapUiPos.clear();
}

void CUiTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

void CUiTool::Initialize()
{
	for(auto& MyMap : *CMyTextureMgr::Get_Instance()->Get_mapTexture())
	{
		if (dynamic_cast<CMySingleTexture*>(MyMap.second) != nullptr)
		{
			WCHAR textureID[3] = L"";
			textureID[0] = MyMap.first.c_str()[0];
			textureID[1] = MyMap.first.c_str()[1];

			if (lstrcmp(textureID, L"UI"))
				continue;

			m_ListBox.AddString(MyMap.first.c_str());
			UI_POS* pUi = new UI_POS;
			pUi->strName = MyMap.first.c_str();
			pUi->vPos.x = -1000;
			pUi->vPos.y = -1000;
			pUi->vPos.z = pUi->vPos.w = 0.f;
			m_mapUiPos[MyMap.first.c_str()] = pUi;
		}
	}

	CRenderMgr::Get_Instance()->Set_MapUiInfo(&m_mapUiPos);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);
	pToolView->Set_MapUIPos(&m_mapUiPos);

	
}


BEGIN_MESSAGE_MAP(CUiTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUiTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CUiTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUiTool::OnLbnSelchangeList)
END_MESSAGE_MAP()


// CUiTool 메시지 처리기입니다.


void CUiTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(
		FALSE,  /* 열기는 TRUE, 저장은 FALSE*/
		L"dat", /* 디폴트 확장자*/
		L"UI.dat", /*디폴트 파일이름*/
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, /* 읽기전용 열기 체크박스 숨기겠다, 덮어쓸 시 메세지 보이겠다*/
		L"Data Files(*.dat)|*.dat||",	/*파일 형식*/
		this);

	//모달리스 형식   : 현재 창 뿐만 아니라 다른창에도 제어권 있음. 
	//모달형식		: 제어권이 현재 창에만 있음

	TCHAR szCurDir[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);


	lstrcat(szCurDir, L"\\SaveData");

	//대화상자를 열었을 때 초기경로 설정
	Dlg.m_ofn.lpstrInitialDir = szCurDir;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Save Failed");
			return;
		}
		DWORD dwByte = 0;


		for (auto& UIMap : m_mapUiPos)
		{
		
			WriteFile(hFile, UIMap.second->strName.GetString(), sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
			WriteFile(hFile, &UIMap.second->vPos, sizeof(D3DXVECTOR4), &dwByte, nullptr);
			
		}

		CloseHandle(hFile);
		ERR_MSG(L"Save Complete");
	}
}


void CUiTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(
		TRUE,  /* 열기는 TRUE, 저장은 FALSE*/
		L"dat", /* 디폴트 확장자*/
		L"UI.dat", /*디폴트 파일이름*/
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, /* 읽기전용 열기 체크박스 숨기겠다, 덮어쓸 시 메세지 보이겠다*/
		L"Data Files(*.dat)|*.dat||",	/*파일 형식*/
		this);

	//모달리스 형식   : 현재 창 뿐만 아니라 다른창에도 제어권 있음. 
	//모달형식		: 제어권이 현재 창에만 있음

	TCHAR szCurDir[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);


	lstrcat(szCurDir, L"\\SaveData");

	//대화상자를 열었을 때 초기경로 설정
	Dlg.m_ofn.lpstrInitialDir = szCurDir;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Load Failed");
			return;
		}
		DWORD dwByte = 0;



		while (true)
		{
			WCHAR	strUnit[MAX_STR] = L"";

			ReadFile(hFile, strUnit, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);

			if (dwByte == 0)
				break;

			auto& iter = m_mapUiPos.find(strUnit);
			if (iter == m_mapUiPos.end())
			{
				ERR_MSG(L"잘못된 저장파일");
				break;
			}

			D3DXVECTOR4 loadUiPos;
		
			ReadFile(hFile, &loadUiPos, sizeof(D3DXVECTOR4), &dwByte, nullptr);
			

			UI_POS* pData = new UI_POS;
			pData->strName = strUnit;
			pData->vPos = loadUiPos;

			SAFE_DELETE(iter->second);
			iter->second = pData;


		}




		CloseHandle(hFile);
		ERR_MSG(L"Load Complete");
	}
	UpdateData(FALSE);
}


void CUiTool::OnLbnSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_ListBox.GetCurSel();

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);
	pToolView->m_bIsSelectMonster = false;
	pToolView->m_iTileIndex = -1;
	pToolView->m_bIsSelectUI = true;
	pToolView->m_bIsSelectMonster = false;
	pToolView->m_bIsSelectTile = false;


	UpdateData(TRUE);

	index = m_ListBox.GetCurSel();
	CString selectName;
	m_ListBox.GetText(index, selectName);

	for (auto& MyUi : m_mapUiPos)
	{
		if (MyUi.first == selectName)
		{
			m_pSelectUI = MyUi.second;
		}
	}
	pToolView->m_pSelectUI = m_pSelectUI;
	/*if (m_pSelectUI)
		m_pSelectUI = nullptr;*/

	UpdateData(FALSE);
}
