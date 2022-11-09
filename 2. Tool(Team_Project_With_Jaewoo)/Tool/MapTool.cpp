// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "TileMgr.h"
#include "Tile.h"
#include "FileInfo.h"
#include "UnitTool.h"
#include "RenderMgr.h"
// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_strEvent(_T(""))
{
	m_iTileIndex = 2;
}

CMapTool::~CMapTool()
{

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_strEvent);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
	DDX_Control(pDX, IDC_PICTURE_MINI, m_PictureCtrl_Minimap);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBoxWall);
	DDX_Control(pDX, IDC_WALL, m_StaticWall);
	DDX_Control(pDX, IDC_PICTURE_MONSTER, m_MonsterPicCtrl);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeTileIndex)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnClickedLoad)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapTool::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnBnClickedCheckBoxWall)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedApply)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCHAR szTile[MID_STR] = L"";
	TCHAR szIndex[MIN_STR] = L"";
	for (size_t i = 0; i < TILE_TEX; ++i)
	{
		TCHAR szTile[MID_STR] = L"Tile";
		TCHAR szIndex[MIN_STR] = L"";
		swprintf_s(szIndex, L"%d", (int)i);
		lstrcat(szTile, szIndex);

		m_ListBox.AddString(szTile);
	}

	m_CheckBoxWall.SetCheck(FALSE);
	m_StaticWall.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::OnLbnSelchangeTileIndex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int index = m_ListBox.GetCurSel();
	if (index == -1)
		return;

	CString strSelectName = L"";
	m_ListBox.GetText(index, strSelectName);

	int i = 0;
	for (; i < strSelectName.GetLength(); ++i)
	{
		//현재 문자가 숫자인지 글자인지 구분.
		//isdigit함수: 현재 문자가 숫자라면 true, 글자라면 false
		if (isdigit(strSelectName[i]))
			break;
	}

	//CString::Delete(index, count)
	//현재 문자열의 index부터 count만큼 삭제
	strSelectName.Delete(0, i);

	m_iTileIndex = _wtoi(strSelectName.GetString());

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);

	pToolView->Set_IsSelectMonster(false);
	pToolView->Set_TileIndex(m_iTileIndex);
	pToolView->m_bIsSelectTile = true;
	CMFCDeviceMgr::Get_Instance()->Render_Begin();

	const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(L"Terrain", L"Tile", m_iTileIndex);
	NULL_CHECK(pTexInfo);

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width, WINCY / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&mTrans, 55.f, 20.f, 0.f);
	
	mWorld = mScale*mTrans;
	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f),nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CMFCDeviceMgr::Get_Instance()->Render_End(m_PictureCtrl.m_hWnd);

	Render_MiniMap();
}


void CMapTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(
		FALSE,  /* 열기는 TRUE, 저장은 FALSE*/
		L"dat", /* 디폴트 확장자*/
		L"Map.dat", /*디폴트 파일이름*/
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

		for (auto& pTile : *CTileMgr::Get_Instance()->Get_TileVector())
		{
			WriteFile(hFile, &pTile->Get_Info(), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_TileInfo(), sizeof(TILE_INFO), &dwByte, nullptr);
		}
		CloseHandle(hFile);


		hFile = CreateFile(L"../SaveData/Map_Monster.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Save Failed");
			return;
		}

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
		NULL_CHECK(pToolView);

		map<CString, vector<MONSTER_POS*>>& rMonsterInfo = pToolView->m_mapMonsterInfo;

		for (auto& Monster : rMonsterInfo)
		{
			for (size_t i = 0; i < Monster.second.size(); ++i)
			{
				WriteFile(hFile, Monster.first.GetString() , sizeof(WCHAR) * MID_STR, &dwByte, nullptr);
				WriteFile(hFile, Monster.second[i], sizeof(MONSTER_POS), &dwByte, nullptr);
				int a = 5;
			}
		}
		CloseHandle(hFile);
		ERR_MSG(L"Save Complete");
	}
}


void CMapTool::OnBnClickedLoad()
{
	CTileMgr::Get_Instance()->Release();

	CFileDialog Dlg(
		TRUE,  /* 열기는 TRUE, 저장은 FALSE*/
		L"dat", /* 디폴트 확장자*/
		L"Map.dat", /*디폴트 파일이름*/
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Load Failed");
			return;
		}

		WCHAR   strData[MAX_STR] = L"";

		INFO tInfo;
		TILE_INFO tTileInfo;

		DWORD dwByte = 0;

		CTile* pTile = nullptr;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			ReadFile(hFile, &tTileInfo, sizeof(TILE_INFO), &dwByte, nullptr);

			pTile = new CTile;
			pTile->Set_Info(tInfo);
			dynamic_cast<CTile*>(pTile)->Set_TileInfo(tTileInfo);

			CTileMgr::Get_Instance()->Get_TileVector()->emplace_back(pTile);
		}

		CloseHandle(hFile);

		hFile = CreateFile(L"../SaveData/Map_Monster.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Load Failed");
			return;
		}

		WCHAR strName[MID_STR] = L"";
		MONSTER_POS tPos;

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
		NULL_CHECK(pToolView);

		auto& iter_begin = pToolView->m_mapMonsterInfo.begin();

		for (; iter_begin != pToolView->m_mapMonsterInfo.end(); )
		{
			for (auto& pMonsterPos : (*iter_begin).second)
			{
				SAFE_DELETE(pMonsterPos);
			}
			(*iter_begin).second.clear();
		}
		pToolView->m_mapMonsterInfo.clear();

		while (true)
		{
			ReadFile(hFile, strName, sizeof(WCHAR) * MID_STR, &dwByte, nullptr);
			ReadFile(hFile, &tPos, sizeof(MONSTER_POS), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			MONSTER_POS* pPos = new MONSTER_POS();
			*pPos = tPos;
			
			pToolView->m_mapMonsterInfo[strName].emplace_back(pPos);
		}

		CloseHandle(hFile);
		ERR_MSG(L"Load Complete");
	}

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);

	pToolView->Invalidate(FALSE);
}

//파일을 끌어다 놓을때 호출되는 함수(WM_DROPFILES)
void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MID_STR] = L"";
	CString strRelative = L"";
	CString strFileName = L"";

	//두번째 인자에 -1을 전달하면 드래그앤드롭된 전체 파일 개수를 리턴.
	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		//DragQueryFile(hDropInfo, index, outstring, buffersize)
		//HDROP은 드롭된 배열의 첫번째 주소를 가리킨다.
		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		//상대경로로 변환 (여기서 굳이 안해도됨)
		strRelative = CFileInfo::Convert_RelativePath(szFilePath);
		//현재 경로에서 "파일명.확장자" 얻어오는 함수 
		strFileName = PathFindFileName(strRelative);
		lstrcpy(szFileName, strFileName.GetString());
		//확장자 지우기
		PathRemoveExtension(szFileName);

		m_ListBox.AddString(szFileName);
	}

	Create_HorizontalScroll();
}

void CMapTool::Create_HorizontalScroll()
{
	CString strName = L"";

	CDC* pDC = m_ListBox.GetDC();
	CSize size;
	int iCX = 0;
	//CListBox::GetCount함수(리스트 박스 목록의 전체 개수)
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		//CDC::GetTextExtent함수 (현재 텍스트의 픽셀 단위 크기를 얻어오는 함수)
		size= pDC->GetTextExtent(strName);
		if (iCX < size.cx)
			iCX = size.cx;
	}

	//현재 리스트박스의 수평스크롤 최대 범위를 얻어오는 함수
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);		//수평 스크롤 최대범위 설정.
	
	ReleaseDC(pDC);
}

void CMapTool::Render_MiniMap()
{
	CMFCDeviceMgr::Get_Instance()->Render_Begin();
	CTileMgr::Get_Instance()->Mini_Render();
	CRenderMgr::Get_Instance()->Render_MapTool_MiniMap();
	CMFCDeviceMgr::Get_Instance()->Render_End(m_PictureCtrl_Minimap.m_hWnd);
}

void CMapTool::Update_Unit()
{
	m_ComboBox.ResetContent();
	map<CString, UNIT_DATA*>	pMapUnit = m_pUnitTool->m_mapUnitData;
	m_pSelectUnit = nullptr;
	for (auto& MyUnit : pMapUnit)
	{
		if (MyUnit.second->eID == UNIT_DATA::MONSTER)		//몬스터만 배치
		{
			m_ComboBox.AddString(MyUnit.first);
		}
	}
}

void CMapTool::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	m_iTileIndex = -1;

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);

	pToolView->Set_TileIndex(m_iTileIndex);

	m_ListBox.SetCurSel(-1);

	map<CString, UNIT_DATA*>	pMapUnit = m_pUnitTool->m_mapUnitData;
	int index = m_ComboBox.GetCurSel();
	CString selectName;
	m_ComboBox.GetLBText(index, selectName);

	for (auto& MyUnit : pMapUnit)
	{
		if (MyUnit.first == selectName)
		{
			m_pSelectUnit = MyUnit.second;
		}
	}
	pToolView->m_bIsSelectTile = false;
	pToolView->Set_IsSelectMonster(true);
	pToolView->Set_SelectMonster(m_pSelectUnit);

	CRenderMgr::Get_Instance()->m_pMonsterPreview = m_pSelectUnit;
	CRenderMgr::Get_Instance()->Render_MonsterPreview(m_MonsterPicCtrl.m_hWnd);
	if (m_pSelectUnit)
		m_pSelectUnit = nullptr;

	UpdateData(FALSE);

}

void CMapTool::OnBnClickedCheckBoxWall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (TRUE == m_CheckBoxWall.GetCheck())
		m_StaticWall.EnableWindow(TRUE);
	else
		m_StaticWall.EnableWindow(FALSE);
}


void CMapTool::OnBnClickedApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	ERR_MSG(L"적용 완료");

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);

	if (TRUE == m_CheckBoxWall.GetCheck())
		pToolView->m_bWallTileActive = TRUE;
	else
		pToolView->m_bWallTileActive = FALSE;

	wcscpy_s(pToolView->m_strEventTileActive, m_strEvent.GetString());

	UpdateData(FALSE);
}
