// AnimationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"
#include "MyTexture.h"
#include "MySingleTexture.h"
#include "MyMultiTexture.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "RenderMgr.h"

// CAnimationTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
	, m_strUnit(_T(""))
	, m_strState(_T(""))
	, m_strDir(_T(""))
	, m_strSearch(_T(""))
	, m_fFrameSpeed(0)
	, m_strCurrentFrame(_T(""))
	, m_strTotalFrame(_T(""))
	, m_strFrameSpeed(_T(""))
{
	m_bDirCheck = false;
	m_bStateCheck = false;
	m_iFrameStart = 0;
	m_iFrameEnd = 0;
	m_iSelectIndex = -1;

	m_strFrameSpeed = L"1";
	m_bIsStop = true;

}

CAnimationTool::~CAnimationTool()
{

	for (auto& MyPair : m_MapAnimation)
	{
		for (auto& MyData : MyPair.second)
			SAFE_DELETE(MyData.second);
		MyPair.second.clear();
	}
	m_MapAnimation.clear();
}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_UnitList);
	DDX_Control(pDX, IDC_LIST2, m_StateList);
	DDX_Control(pDX, IDC_LIST3, m_DirList);
	DDX_Control(pDX, IDC_LIST5, m_KeyList);
	DDX_LBString(pDX, IDC_LIST1, m_strUnit);
	DDX_LBString(pDX, IDC_LIST2, m_strState);
	DDX_LBString(pDX, IDC_LIST3, m_strDir);
	DDX_Text(pDX, IDC_EDIT1, m_strSearch);
	DDX_Control(pDX, IDC_PICTURE2, m_PictureCtrl);
	DDX_Text(pDX, IDC_EDIT3, m_strTotalFrame);
	DDX_Text(pDX, IDC_EDIT10, m_strFrameSpeed);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl);	
}

void CAnimationTool::Initialize()
{
	for (auto& mPair : *m_pMapUnitData)
	{
		m_UnitList.AddString(mPair.second->strName);
	}

	map<const wstring, CMyTexture*>* pTexMap = CMyTextureMgr::Get_Instance()->Get_mapTexture();

	for (auto& MyPair : *pTexMap)
	{
		if (dynamic_cast<CMySingleTexture*>(MyPair.second) != nullptr)
			continue;
		ANIM_INFO mtInfo;
		for (auto& MyTexKey : *dynamic_cast<CMyMultiTexture*>(MyPair.second)->Get_mapMultiTex())
		{
			/*ANIM_DATA* pAnimData = new ANIM_DATA;
			pAnimData->m_fFrameSpeed = 1;*/
			ANIM_DATA* pData = new ANIM_DATA;
			pData->m_fFrameSpeed=1.f;
			
			mtInfo.insert(pair<CString, ANIM_DATA*>(MyTexKey.first.c_str(), pData));
			//*mtInfo->emplace()
			//mtInfo[MyTexKey.first.c_str()] = pData;
			//m_MapAnimation[MyPair.first.c_str()] = mtInfo;
			
		}
		m_MapAnimation[MyPair.first.c_str()] = mtInfo;
	}

	CRenderMgr::Get_Instance()->Set_AnimTool(this);

	UpdateData(FALSE);

	
}

void CAnimationTool::Add_ListBox(CString unitName)
{
	m_UnitList.AddString(unitName);
}

void CAnimationTool::Delete_ListBox(CString unitName)
{
	int index = m_UnitList.FindString(-1, unitName);
	m_UnitList.DeleteString(index);
}

BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationTool::OnLbnSelUnitChange)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimationTool::OnLbnSelStateList)
	ON_LBN_SELCHANGE(IDC_LIST3, &CAnimationTool::OnLbnSelDirList)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimationTool::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_BUTTON9, &CAnimationTool::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON10, &CAnimationTool::OnBnClickedStop)
	ON_EN_CHANGE(IDC_EDIT10, &CAnimationTool::OnEnChangeSpeed)
	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(IDC_LIST5, &CAnimationTool::OnLbnSelKeyList)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimationTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimationTool::OnBnClickedModify)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CAnimationTool 메시지 처리기입니다.


void CAnimationTool::OnLbnSelUnitChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	m_iSelectIndex = -1;
	m_StateList.ResetContent();
	m_DirList.ResetContent();
	m_KeyList.ResetContent();
	m_strTotalFrame = L"";
	m_strFrameSpeed = L"";

	m_bDirCheck = false;
	m_bStateCheck = false;

	int iIndex = m_UnitList.GetCurSel();

	if (iIndex == -1)
		return;

	CString strSelectName = L"";
	
	static CString oldState = L"";
	static CString oldDir = L"";

	m_UnitList.GetText(iIndex, strSelectName);
	
	map<const wstring, CMyTexture*>* pTextureMap = CMyTextureMgr::Get_Instance()->Get_mapTexture();

	for (auto& MyPair : *pTextureMap)
	{
		if (dynamic_cast<CMyMultiTexture*>(MyPair.second) == nullptr)
			continue;
		if (MyPair.first == strSelectName.GetString())
		{
			//wstring tmp = MyPair.second->
			map<const wstring, VEC_TEXINFO>* pMapMultitex = dynamic_cast<CMyMultiTexture*>(MyPair.second)->Get_mapMultiTex();

			for (auto& MyMultiTex : *pMapMultitex)
			{
				wstring wStateDir = MyMultiTex.first;
				WCHAR wStateArr[16] = L"";
				WCHAR wDirArr[16] = L"";
				
				int i = 0;
				for (; wStateDir[i] != '_'; ++i)
				{
					wStateArr[i] = wStateDir[i];
				}
				++i;
				int j = 0;
				for (; i<wStateDir.size(); ++i)
				{
					wDirArr[j] = wStateDir[i];
					++j;
				}

				CString cstrState;
				cstrState = wStateArr;
				if (cstrState != oldState)
				{
					m_StateList.AddString(cstrState);
					oldState = cstrState;
				}
					
				CString cstrDir;
				CString cstrSearchDir;
				cstrDir = wDirArr;

				bool bCheckDir = true;
				for (int k = 0; k < m_DirList.GetCount(); ++k)
				{
					m_DirList.GetText(k, cstrSearchDir);
					if (cstrDir == cstrSearchDir)
						bCheckDir = false;
				}
				if (bCheckDir)
				{
					m_DirList.AddString(cstrDir);
					oldDir = cstrDir;
				}
				
			}
		}
	}

	oldState = L"";
	oldDir = L"";
	m_iFrameEnd = 0;
	UpdateData(FALSE);
	Render_Texture();
}

void CAnimationTool::Render_Texture()
{

	CMFCDeviceMgr::Get_Instance()->Render_Begin();

	int iRow = 50;
	int iCol = 0;

	for (int i = 0; i < m_iFrameEnd; ++i)
	{
		const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			m_strUnit.GetString(), m_TextureKey.GetString(), i);
		if (pTexInfo == nullptr)
			break;

		if (i % 5 == 0 && i >= 5)
		{
			iRow = 50;
			iCol += 250;
		}

		float fSizeX = pTexInfo->tImgInfo.Width;
		float fSizeY = pTexInfo->tImgInfo.Height;
		
		float fScaleX = 220.f / fSizeX;
		float fScaleY = 270.f / fSizeY;

		D3DXMATRIX mScale, mWorld, mTrans;
		D3DXMatrixScaling(&mScale, fScaleX, fScaleY, 0.f);

		if (i >= 4)
			D3DXMatrixTranslation(&mTrans, iRow + (i % 5) * 220.f, iCol, 0.f);
		else
			D3DXMatrixTranslation(&mTrans, iRow + (i % 5) * 220.f, iCol, 0.f);


		mWorld = mScale*mTrans;
		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		WCHAR wIndex[8] = L"";
		_itot_s(i, wIndex, sizeof(wIndex));

		D3DXMatrixTranslation(&mTrans, iRow + (i % 5) * 220.f - 10.f, iCol, 0.f);
		mWorld = mScale * mTrans;

		if (i == m_iSelectIndex)
		{
			RECT rc = { 0,0,50,50 };
			CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);
			CMFCDeviceMgr::Get_Instance()->Get_Font()->DrawTextW(CMFCDeviceMgr::Get_Instance()->Get_Sprite(),
				wIndex, lstrlen(wIndex), &rc, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0x00));
		}
		else
		{
			RECT rc = { 0,0,50,50 };
			CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);
			CMFCDeviceMgr::Get_Instance()->Get_Font()->DrawTextW(CMFCDeviceMgr::Get_Instance()->Get_Sprite(),
				wIndex, lstrlen(wIndex), &rc, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));
		}

		iRow++;
		
	}

	//m_PictureCtrl.scr
	CMFCDeviceMgr::Get_Instance()->Render_End(m_PictureCtrl.m_hWnd);
}


void CAnimationTool::OnLbnSelStateList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	m_bStateCheck = true;

	//m_TextureKey = m_strState + '_' + m_strDir;

	if (m_bStateCheck && m_bDirCheck)
	{
		//프레임 속도 값 정해주기.


		for (auto& MyPair : m_MapAnimation)
		{
			if (m_strUnit == MyPair.first)
			{
				for (auto& MyTexKey : MyPair.second)
				{
					if (MyTexKey.first == m_TextureKey)
					{
						m_fFrameSpeed = MyTexKey.second->m_fFrameSpeed;
						WCHAR wFrameSpeed[32] = L"";
						//_itot_s(m_fFrameSpeed, wFrameSpeed, sizeof(wFrameSpeed));
						swprintf_s(wFrameSpeed, L"%.f", m_fFrameSpeed);

						m_strFrameSpeed = wFrameSpeed;
					}
				}
			}

		}

		Render_Texture();
	}

	UpdateData(FALSE);
}


void CAnimationTool::OnLbnSelDirList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_bDirCheck = true;

	if (m_bStateCheck && m_bDirCheck)
	{
		m_KeyList.ResetContent();


		for (auto& MyMap : *CMyTextureMgr::Get_Instance()->Get_mapTexture())
		{
			wstring wUnitName = MyMap.first;

			if (wUnitName == m_strUnit.GetString())
			{
				map<const wstring, VEC_TEXINFO>* pMultiTex = dynamic_cast<CMyMultiTexture*>(MyMap.second)->Get_mapMultiTex();

				for (auto& MyMultiMap : *pMultiTex)
				{
					CString TextureKey = L"";
					m_TextureKey = TextureKey = m_strState + '_' + m_strDir;

					if (TextureKey.GetString() == MyMultiMap.first)
					{
						size_t i = 0;
						m_iFrameStart = 0;
						for (; i < MyMultiMap.second.size(); ++i)
						{
							WCHAR wIndex[4] = L"";
							//_itot_s(i, wIndex, sizeof(wIndex));
							swprintf_s(wIndex, L"%.d", i);
							TextureKey = TextureKey + wIndex;

							m_KeyList.AddString(TextureKey);

							TextureKey = m_strState + '_' + m_strDir;
						}
						m_iFrameEnd = i;

						WCHAR wEndFrame[32] = L"";
						swprintf_s(wEndFrame, L"%.d", m_iFrameEnd - 1);
						m_strTotalFrame = wEndFrame;
					}

				}

			}

			CString TextureName = L"";
			TextureName = m_strUnit + '_' + m_strState + '_' + m_strDir;

		}
	}
	if (m_bStateCheck && m_bDirCheck)
	{
		Render_Texture();


		for (auto& MyPair : m_MapAnimation)
		{
			if (m_strUnit == MyPair.first)
			{
				for (auto& MyTexKey : MyPair.second)
				{
					if (MyTexKey.first == m_TextureKey)
					{
						m_fFrameSpeed = MyTexKey.second->m_fFrameSpeed;

						WCHAR wFrameSpeed[32] = L"";
						swprintf_s(wFrameSpeed, L"%.f", m_fFrameSpeed);
						m_strFrameSpeed = wFrameSpeed;
					}
				}
			}

		}
	}

	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int a = m_UnitList.GetCurSel();
	auto iter_find = m_pMapUnitData->find(m_strSearch);

	if (m_pMapUnitData->end() == iter_find)		//검색하려는 문자열 존재 안할시.
		return;

	int iIndex = m_UnitList.FindString(-1, m_strSearch);
	
	m_UnitList.SetCurSel(iIndex);
	OnLbnSelUnitChange();
}

void CAnimationTool::Set_FrameIndex(float _fCurFrame)
{
	WCHAR wCurFrame[8] = L"";
	//_itot_s((int)_fCurFrame, wCurFrame, sizeof(wCurFrame));
	m_strCurrentFrame = wCurFrame;

	//보류

	//UpdateData(FALSE);
	//Invalidate(FALSE);
}



void CAnimationTool::OnBnClickedPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CRenderMgr::Get_Instance()->Set_IsAnimation(true);
	CRenderMgr::Get_Instance()->Set_Unit(m_strUnit.GetString());
	CRenderMgr::Get_Instance()->Set_Key(m_TextureKey.GetString());
	CRenderMgr::Get_Instance()->Set_ToolID(CRenderMgr::ANIMATION);
	CRenderMgr::Get_Instance()->Set_FrameStart(0);
	CRenderMgr::Get_Instance()->Set_FrameEnd(m_KeyList.GetCount() - 1);

	wstring strSpeed = L"";
	strSpeed = m_strFrameSpeed.GetString();
	float fSpeed = _ttof(strSpeed.c_str());
	CRenderMgr::Get_Instance()->Set_FrameSpeed(fSpeed);

	m_bIsStop = false;
	UpdateData(FALSE);

}


void CAnimationTool::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRenderMgr::Get_Instance()->Set_IsAnimation(false);
	m_bIsStop = true;
	
}


void CAnimationTool::OnEnChangeSpeed()
{
	//UpdateData(TRUE);
	//wstring strSpeed = L"";
	//strSpeed = m_strFrameSpeed.GetString();
	//float fSpeed = _ttof(strSpeed.c_str());
	//CRenderMgr::Get_Instance()->Set_FrameSpeed(fSpeed);

	//OnBnClickedPlay();

	//UpdateData(FALSE);
}


void CAnimationTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateData(TRUE);
	if (m_bIsStop)
	{
		if (pScrollBar == (CScrollBar*)&m_SliderCtrl)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_SliderCtrl.GetPos();
			int iFrameIndex = (m_iFrameEnd - 1) * nPos / 100;
			if (iFrameIndex == m_iFrameEnd)
				iFrameIndex -= 1;
			CRenderMgr::Get_Instance()->Set_FrameStart(iFrameIndex);
			m_iSelectIndex = iFrameIndex;
			Render_Texture();
		}
	}
}


void CAnimationTool::OnLbnSelKeyList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int iIndex = m_KeyList.GetCurSel();

	m_iSelectIndex = iIndex;
	Render_Texture();
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(
		FALSE,  /* 열기는 TRUE, 저장은 FALSE*/
		L"dat", /* 디폴트 확장자*/
		L"Anim.dat", /*디폴트 파일이름*/
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

		
		for (auto& AnimMap : m_MapAnimation)
		{
			for (auto& UnitMap : AnimMap.second)
			{
				if (AnimMap.first == L"Terrain")
					break;
				WriteFile(hFile, AnimMap.first.GetString(), sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				WriteFile(hFile, UnitMap.first.GetString(), sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				WriteFile(hFile, UnitMap.second, sizeof(ANIM_DATA), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
		ERR_MSG(L"Save Complete");
	}

}


void CAnimationTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(
		TRUE,  /* 열기는 TRUE, 저장은 FALSE*/
		L"dat", /* 디폴트 확장자*/
		L"Anim.dat", /*디폴트 파일이름*/
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

		WCHAR	strUnit[MAX_STR] = L"";
		WCHAR	strState[MAX_STR] = L"";

		bool bCheck = false;

		while(true)
		{
			WCHAR	strUnit[MAX_STR] = L"";
			WCHAR	strState[MAX_STR] = L"";
				
			ReadFile(hFile, strUnit, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);

			if (dwByte == 0)
				break;

			auto& iter = m_MapAnimation.find(strUnit);
			if (iter == m_MapAnimation.end())
			{
				ERR_MSG(L"잘못된 저장파일");
				break;
			}

			ANIM_DATA tData;
			ReadFile(hFile, strState, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);

			ReadFile(hFile, &tData, sizeof(ANIM_DATA), &dwByte, nullptr);

			ANIM_DATA* pData = new ANIM_DATA;
			*pData = tData;

			delete iter->second[strState];
			iter->second[strState] = pData;

			
		}
			
		
				
		
		CloseHandle(hFile);
		ERR_MSG(L"Load Complete");
	}
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fFrameSpeed = _ttoi(m_strFrameSpeed.GetString());
	for (auto& MyPair : m_MapAnimation)
	{
		if (m_strUnit == MyPair.first)
		{
			for (auto& MyTexKey : MyPair.second)
			{
				if (MyTexKey.first == m_TextureKey)
				{
					MyTexKey.second->m_fFrameSpeed = m_fFrameSpeed;
				}
			}
		}

	}

	UpdateData(FALSE);
}


void CAnimationTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		ERR_MSG(L"sss");
	}
}
