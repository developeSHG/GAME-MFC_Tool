// UiTool.cpp : ���� �����Դϴ�.
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


// CUiTool ��ȭ �����Դϴ�.

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


// CUiTool �޽��� ó�����Դϴ�.


void CUiTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog Dlg(
		FALSE,  /* ����� TRUE, ������ FALSE*/
		L"dat", /* ����Ʈ Ȯ����*/
		L"UI.dat", /*����Ʈ �����̸�*/
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, /* �б����� ���� üũ�ڽ� ����ڴ�, ��� �� �޼��� ���̰ڴ�*/
		L"Data Files(*.dat)|*.dat||",	/*���� ����*/
		this);

	//��޸��� ����   : ���� â �Ӹ� �ƴ϶� �ٸ�â���� ����� ����. 
	//�������		: ������� ���� â���� ����

	TCHAR szCurDir[MAX_STR] = L"";

	//���� �۾� ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_STR, szCurDir);
	//��λ� ���ϸ��� �����ϴ� �Լ�. ������ ���ϸ��� ���ٸ� ���� �������� ������.
	PathRemoveFileSpec(szCurDir);


	lstrcat(szCurDir, L"\\SaveData");

	//��ȭ���ڸ� ������ �� �ʱ��� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog Dlg(
		TRUE,  /* ����� TRUE, ������ FALSE*/
		L"dat", /* ����Ʈ Ȯ����*/
		L"UI.dat", /*����Ʈ �����̸�*/
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, /* �б����� ���� üũ�ڽ� ����ڴ�, ��� �� �޼��� ���̰ڴ�*/
		L"Data Files(*.dat)|*.dat||",	/*���� ����*/
		this);

	//��޸��� ����   : ���� â �Ӹ� �ƴ϶� �ٸ�â���� ����� ����. 
	//�������		: ������� ���� â���� ����

	TCHAR szCurDir[MAX_STR] = L"";

	//���� �۾� ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_STR, szCurDir);
	//��λ� ���ϸ��� �����ϴ� �Լ�. ������ ���ϸ��� ���ٸ� ���� �������� ������.
	PathRemoveFileSpec(szCurDir);


	lstrcat(szCurDir, L"\\SaveData");

	//��ȭ���ڸ� ������ �� �ʱ��� ����
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
				ERR_MSG(L"�߸��� ��������");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
