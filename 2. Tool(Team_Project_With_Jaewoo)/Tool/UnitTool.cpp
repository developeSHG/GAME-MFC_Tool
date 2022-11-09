// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "MyForm.h"
#include "afxdialogex.h"
#include "RenderMgr.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "ToolView.h"
// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strName(_T(""))
	, m_iAtt(0)
	, m_iHp(0)
	, m_strSearch(_T(""))
	, m_iMp(0)
	, m_iDef(0)
	, m_iLife(0)
	, m_fSpeed(0)
	, m_iGold(0)
	, m_iLevel(0)
	, m_dExp(0)
{

}

CUnitTool::~CUnitTool()
{
	for_each(m_mapUnitData.begin(), m_mapUnitData.end(),
		[](auto& MyPair)
	{
		SAFE_DELETE(MyPair.second);
	});

	m_mapUnitData.clear();

}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// Control Value
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Text(pDX, IDC_EDIT6, m_strSearch);
	DDX_Control(pDX, IDC_TAB2, m_Tab);
	DDX_Control(pDX, IDC_EDIT4, m_EditDataInfo[INFO::NAME]);
	DDX_Control(pDX, IDC_EDIT5, m_EditDataInfo[INFO::HP]);
	DDX_Control(pDX, IDC_EDIT7, m_EditDataInfo[INFO::MP]);
	DDX_Control(pDX, IDC_EDIT3, m_EditDataInfo[INFO::ATT]);
	DDX_Control(pDX, IDC_EDIT11, m_EditDataInfo[INFO::DEF]);
	DDX_Control(pDX, IDC_EDIT12, m_EditDataInfo[INFO::LIFE]);
	DDX_Control(pDX, IDC_EDIT13, m_EditDataInfo[INFO::SPEED]);
	DDX_Control(pDX, IDC_EDIT14, m_EditDataInfo[INFO::GOLD]);
	DDX_Control(pDX, IDC_EDIT15, m_EditDataInfo[INFO::LEVEL]);
	DDX_Control(pDX, IDC_EDIT16, m_EditDataInfo[INFO::EXP]);
	// User Value
	DDX_Text(pDX, IDC_EDIT4, m_strName);
	DDX_Text(pDX, IDC_EDIT5, m_iHp);
	DDX_Text(pDX, IDC_EDIT7, m_iMp);
	DDX_Text(pDX, IDC_EDIT3, m_iAtt);
	DDX_Text(pDX, IDC_EDIT11, m_iDef);
	DDX_Text(pDX, IDC_EDIT12, m_iLife);
	DDX_Text(pDX, IDC_EDIT13, m_fSpeed);
	DDX_Text(pDX, IDC_EDIT14, m_iGold);
	DDX_Text(pDX, IDC_EDIT15, m_iLevel);
	DDX_Text(pDX, IDC_EDIT16, m_dExp);
	DDX_Control(pDX, IDC_RADIO1, m_RadioID[UNIT_DATA::ID::PLAYER]);
	DDX_Control(pDX, IDC_RADIO3, m_RadioID[UNIT_DATA::ID::MONSTER]);
	DDX_Control(pDX, IDC_RADIO5, m_RadioID[UNIT_DATA::ID::ITEM]);
	DDX_Control(pDX, IDC_STATIC1, m_StaticUnitlist);
	DDX_Control(pDX, IDC_STATIC2, m_StaticName);
	DDX_Control(pDX, IDC_STATS, m_Stats);
}

BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelchangeUnitData)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedAdd)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, &CUnitTool::OnBnClickedRadioPlayer)
	ON_BN_CLICKED(IDC_RADIO3, &CUnitTool::OnBnClickedRadioMonster)
	ON_BN_CLICKED(IDC_RADIO5, &CUnitTool::OnBnClickedRadioItem)
	ON_BN_CLICKED(IDC_BUTTON8, &CUnitTool::OnBnClickedModify)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CUnitTool::OnTcnSelchangeUnitListTab)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Font.CreatePointFont(102, L"Verdana");

	// ���� ��ȭ���ڿ� ��ġ�� ������Ʈ�� ������ �Լ�.
	GetDlgItem(IDC_STATIC1)->SetFont(&m_Font);
	GetDlgItem(IDC_STATS)->SetFont(&m_Font);

	CString tab1 = L"All ";
	CString tab2 = L"Player ";
	CString tab3 = L"Monster";
	CString tab4 = L"Item";

	m_Tab.InsertItem(1, tab1);
	m_Tab.InsertItem(2, tab2);
	m_Tab.InsertItem(3, tab3);
	m_Tab.InsertItem(4, tab4);

	for (int i = 0; i < INFO::END; ++i)
		m_EditDataInfo[i].EnableWindow(false);

	Load_Data();

	CRenderMgr::Get_Instance()->Set_UnitTool(this);
	CRenderMgr::Get_Instance()->Set_CurRenderID(UNIT_DATA::END);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

HBRUSH CUnitTool::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	if (m_StaticName.m_hWnd == pWnd->m_hWnd)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CUnitTool::OnLbnSelchangeUnitData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	//���콺 Ŀ���� ������ �ε��� ��ȣ�� ��ȯ.
	int iIndex = m_listBox.GetCurSel();

	if (iIndex == -1)
		return;

	CString strSelectName = L"";

	m_listBox.GetText(iIndex, strSelectName);

	auto iter_find = m_mapUnitData.find(strSelectName);
	if (m_mapUnitData.end() == iter_find)
		return;

	//for (int i = 0; i < UNIT_DATA::ID::END; ++i)
	//{
	//	if (1 == m_RadioID[i].GetCheck())
	//	{
	//		m_eID = UNIT_DATA::ID(i);
	//		break;
	//	}
	//}

	m_eID = m_mapUnitData[strSelectName]->eID;
	switch (m_eID)
	{
	case UNIT_DATA::PLAYER:
		OnBnClickedRadioPlayer();
		break;
	case UNIT_DATA::MONSTER:
		OnBnClickedRadioMonster();
		break;
	case UNIT_DATA::ITEM:
		OnBnClickedRadioItem();
		break;
	}
	for (int i = 0; i < UNIT_DATA::ID::END; ++i)
		m_RadioID[i].SetCheck(FALSE);
	m_RadioID[m_eID].SetCheck(TRUE);

	m_strName = m_mapUnitData[strSelectName]->strName;
	m_iAtt = m_mapUnitData[strSelectName]->iAtt;
	m_iHp = m_mapUnitData[strSelectName]->iHp;
	m_iMp = m_mapUnitData[strSelectName]->iMp;
	m_iAtt = m_mapUnitData[strSelectName]->iAtt;
	m_iDef = m_mapUnitData[strSelectName]->iDef;
	m_iLife = m_mapUnitData[strSelectName]->iLife;
	m_fSpeed = m_mapUnitData[strSelectName]->fSpeed;
	m_iGold = m_mapUnitData[strSelectName]->iGold;
	m_iLevel = m_mapUnitData[strSelectName]->iLevel;
	m_dExp = m_mapUnitData[strSelectName]->dExp;

	UpdateData(FALSE);
}

// Unit ������ư
void CUnitTool::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	bool bCheck = false;

	for (int i = 0; i < UNIT_DATA::ID::END; ++i)
	{
		if (1 == m_RadioID[i].GetCheck())
		{
			bCheck = true;
			break;
		}
		else
			bCheck = false;
	}
	if (false == bCheck)
	{
		ERR_MSG(L"������ Ÿ���� üũ���ּ���.");
		return;
	}

	if (0 == m_strName.GetLength())
	{
		ERR_MSG(L"�� Name�� ������ �� �����ϴ�.");
		return;
	}

	auto iter_find = m_mapUnitData.find(m_strName);

	if (m_mapUnitData.end() != iter_find)
	{
		ERR_MSG(L"ĳ���� �̸��� �̹� ����");
		return;
	}

	UNIT_DATA* pUnitData = new UNIT_DATA;

	pUnitData->eID = m_eID;
	pUnitData->strName = m_strName;
	pUnitData->iAtt = m_iAtt;
	pUnitData->iHp = m_iHp;
	pUnitData->iMp = m_iMp;
	pUnitData->iAtt = m_iAtt;
	pUnitData->iDef = m_iDef;
	pUnitData->iLife = m_iLife;
	pUnitData->fSpeed = m_fSpeed;
	pUnitData->iGold = m_iGold;
	pUnitData->iLevel = m_iLevel;
	pUnitData->dExp = m_dExp;

	m_mapUnitData.insert({ m_strName,pUnitData });
	m_listBox.AddString(m_strName);

	m_pAnimTool->Add_ListBox(m_strName);

	m_pMapTool->Update_Unit();
	
	UpdateData(FALSE);
}

// Unit ������ư
void CUnitTool::OnBnClickedModify()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int iIndex = m_listBox.GetCurSel();

	if (iIndex == -1)
	{
		ERR_MSG(L"���� �� Unit�� �������ּ���.");
		return;
	}

	CString strSelectName = L"";

	m_listBox.GetText(iIndex, strSelectName);

	auto& iter_find = m_mapUnitData.find(m_strName);

	if (m_mapUnitData.end() != iter_find && strSelectName != m_strName)
	{
		ERR_MSG(L"ĳ���� �̸��� �̹� ����");
		return;
	}

	iter_find = m_mapUnitData.find(strSelectName);
	Safe_Delete(iter_find->second);
	m_mapUnitData.erase(iter_find);

	UNIT_DATA* pUnitData = new UNIT_DATA;

	pUnitData->eID = m_eID;
	pUnitData->strName = m_strName;
	pUnitData->iAtt = m_iAtt;
	pUnitData->iHp = m_iHp;
	pUnitData->iMp = m_iMp;
	pUnitData->iAtt = m_iAtt;
	pUnitData->iDef = m_iDef;
	pUnitData->iLife = m_iLife;
	pUnitData->fSpeed = m_fSpeed;
	pUnitData->iGold = m_iGold;
	pUnitData->iLevel = m_iLevel;
	pUnitData->dExp = m_dExp;

	m_mapUnitData.insert({ m_strName,pUnitData });

	m_listBox.DeleteString(iIndex);
	m_listBox.InsertString(iIndex, m_strName);

	ERR_MSG(L"����Ϸ�");
	m_listBox.SetCurSel(iIndex);

	m_pAnimTool->Delete_ListBox(strSelectName);
	m_pAnimTool->Add_ListBox(m_strName);

	m_pMapTool->Update_Unit();
	UpdateData(FALSE);
}

// Unit ������ư
void CUnitTool::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iIndex = m_listBox.GetCurSel();

	if (iIndex == -1)
	{
		ERR_MSG(L"���� �� Unit�� �������ּ���.");
		return;
	}

	CString strSelectName = L"";

	m_listBox.GetText(iIndex, strSelectName);

	auto iter_find = m_mapUnitData.find(strSelectName);
	if (m_mapUnitData.end() == iter_find)
		return;

	m_listBox.DeleteString(iIndex);

	Safe_Delete(iter_find->second);
	m_mapUnitData.erase(iter_find);

	m_strName = L"";
	m_iAtt = m_iHp = 0;

	m_pAnimTool->Delete_ListBox(strSelectName);

	DataReset();

	m_pMapTool->Update_Unit();


	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitt.GetPane(0, 0));
	NULL_CHECK(pToolView);

	pToolView->Set_SelectMonster(nullptr);
	pToolView->Set_IsSelectMonster(false);
	UpdateData(FALSE);
}

// Unit List ���̺��ư
void CUnitTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//���� �Ǵ� ���⿡ �ʿ��� ��ȭ���ڸ� �����ϴ� MFCŬ����
	CFileDialog Dlg(
		FALSE,  /* ����� TRUE, ������ FALSE*/
		L"dat", /* ����Ʈ Ȯ����*/
		L"Unit.dat", /*����Ʈ �����̸�*/
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

		for (auto& pMap : m_mapUnitData)
		{
			switch (pMap.second->eID)
			{
			case UNIT_DATA::ID::PLAYER:
				WriteFile(hFile, &pMap.second->eID, sizeof(UNIT_DATA::ID), &dwByte, nullptr);
				WriteFile(hFile, pMap.second->strName.GetString(), sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iHp, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iMp, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iAtt, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iDef, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iLife, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->fSpeed, sizeof(float), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iGold, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iLevel, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->dExp, sizeof(double), &dwByte, nullptr);
				break;
			case UNIT_DATA::ID::MONSTER:
				WriteFile(hFile, &pMap.second->eID, sizeof(UNIT_DATA::ID), &dwByte, nullptr);
				WriteFile(hFile, pMap.second->strName.GetString(), sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iHp, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iAtt, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iDef, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->fSpeed, sizeof(float), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iGold, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->dExp, sizeof(double), &dwByte, nullptr);
				break;
			case UNIT_DATA::ID::ITEM:
				WriteFile(hFile, &pMap.second->eID, sizeof(UNIT_DATA::ID), &dwByte, nullptr);
				WriteFile(hFile, pMap.second->strName.GetString(), sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iHp, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iAtt, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iDef, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->fSpeed, sizeof(float), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iGold, sizeof(UINT), &dwByte, nullptr);
				WriteFile(hFile, &pMap.second->iLevel, sizeof(UINT), &dwByte, nullptr);
				break;
			}
		}
		CloseHandle(hFile);
		ERR_MSG(L"Save Complete");
	}
}

void CUnitTool::OnBnClickedLoad()
{
	m_listBox.ResetContent();	// ���� ����Ʈ�ڽ��� ��� ���.
	for (auto& MyPair : m_mapUnitData)
		delete MyPair.second;
	m_mapUnitData.clear();


	CFileDialog Dlg(
		TRUE,  /* ����� TRUE, ������ FALSE*/
		L"dat", /* ����Ʈ Ȯ����*/
		L"Unit.dat", /*����Ʈ �����̸�*/
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"Load Failed");
			return;
		}

		UNIT_DATA::ID eID;
		WCHAR		  strData[MAX_STR] = L"";
		UINT		  iHp = 0;
		UINT		  iMp = 0;
		UINT		  iAtt = 0;
		UINT		  iDef = 0;
		UINT		  iLife = 0;
		float		  fSpeed = 0.f;
		UINT		  iGold = 0;
		UINT		  iLevel = 0;
		double		  dExp = 0;

		DWORD dwByte = 0;

		UNIT_DATA* pData = nullptr;

		m_pAnimTool->m_UnitList.ResetContent();

		while (true)
		{
			iHp = 0;
			iMp = 0;
			iAtt = 0;
			iDef = 0;
			iLife = 0;
			fSpeed = 0.f;
			iGold = 0;
			iLevel = 0;
			dExp = 0;
			ReadFile(hFile, &eID, sizeof(UNIT_DATA::ID), &dwByte, nullptr);

			switch (eID)
			{
			case UNIT_DATA::ID::PLAYER:
				ReadFile(hFile, strData, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				ReadFile(hFile, &iHp, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iMp, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iAtt, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iDef, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iLife, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
				ReadFile(hFile, &iGold, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iLevel, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &dExp, sizeof(double), &dwByte, nullptr);
				break;
			case UNIT_DATA::ID::MONSTER:
				ReadFile(hFile, strData, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				ReadFile(hFile, &iHp, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iAtt, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iDef, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
				ReadFile(hFile, &iGold, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &dExp, sizeof(double), &dwByte, nullptr);
				break;
			case UNIT_DATA::ID::ITEM:
				ReadFile(hFile, strData, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
				ReadFile(hFile, &iHp, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iAtt, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iDef, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
				ReadFile(hFile, &iGold, sizeof(UINT), &dwByte, nullptr);
				ReadFile(hFile, &iLevel, sizeof(UINT), &dwByte, nullptr);
				break;
			}

			if (0 == dwByte)
				break;

			pData = new UNIT_DATA;
			pData->eID = eID;
			pData->strName.SetString(strData);
			pData->iHp = iHp;
			pData->iMp = iMp;
			pData->iAtt = iAtt;
			pData->iDef = iDef;
			pData->iLife = iLife;
			pData->fSpeed = fSpeed;
			pData->iGold = iGold;
			pData->iLevel = iLevel;
			pData->dExp = dExp;

			m_mapUnitData.insert({ pData->strName, pData });
			m_listBox.AddString(pData->strName);

			m_pAnimTool->Add_ListBox(pData->strName);
		}
		CloseHandle(hFile);
		ERR_MSG(L"Load Complete");
	}
}

// ó�� ���� ������ Load
void CUnitTool::Load_Data()
{
	HANDLE hFile = CreateFile(L"../SaveData/Unit.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Load Failed");
		return;
	}

	UNIT_DATA::ID eID;
	WCHAR		  strData[MAX_STR] = L"";
	UINT		  iHp = 0;
	UINT		  iMp = 0;
	UINT		  iAtt = 0;
	UINT		  iDef = 0;
	UINT		  iLife = 0;
	float		  fSpeed = 0.f;
	UINT		  iGold = 0;
	UINT		  iLevel = 0;
	double		  dExp = 0;

	DWORD dwByte = 0;

	UNIT_DATA* pData = nullptr;

	while (true)
	{
		iHp = 0;
		iMp = 0;
		iAtt = 0;
		iDef = 0;
		iLife = 0;
		fSpeed = 0.f;
		iGold = 0;
		iLevel = 0;
		dExp = 0;
		ReadFile(hFile, &eID, sizeof(UNIT_DATA::ID), &dwByte, nullptr);

		switch (eID)
		{
		case UNIT_DATA::ID::PLAYER:
			ReadFile(hFile, strData, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
			ReadFile(hFile, &iHp, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iMp, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iAtt, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iDef, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iLife, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &iGold, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iLevel, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &dExp, sizeof(double), &dwByte, nullptr);
			break;
		case UNIT_DATA::ID::MONSTER:
			ReadFile(hFile, strData, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
			ReadFile(hFile, &iHp, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iAtt, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iDef, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &iGold, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &dExp, sizeof(double), &dwByte, nullptr);
			break;
		case UNIT_DATA::ID::ITEM:
			ReadFile(hFile, strData, sizeof(WCHAR) * MAX_STR, &dwByte, nullptr);
			ReadFile(hFile, &iHp, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iAtt, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iDef, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &fSpeed, sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &iGold, sizeof(UINT), &dwByte, nullptr);
			ReadFile(hFile, &iLevel, sizeof(UINT), &dwByte, nullptr);
			break;
		}

		if (0 == dwByte)
			break;

		pData = new UNIT_DATA;
		pData->eID = eID;
		pData->strName.SetString(strData);
		pData->iHp = iHp;
		pData->iMp = iMp;
		pData->iAtt = iAtt;
		pData->iDef = iDef;
		pData->iLife = iLife;
		pData->fSpeed = fSpeed;
		pData->iGold = iGold;
		pData->iLevel = iLevel;
		pData->dExp = dExp;

		m_mapUnitData.insert({ pData->strName, pData });
		m_listBox.AddString(pData->strName);
	}
	CloseHandle(hFile);
	//ERR_MSG(L"Load Complete");
}

// ������ ����
void CUnitTool::DataReset()
{
	m_strName = L"";
	m_iHp = 0;
	m_iMp = 0;
	m_iAtt = 0;
	m_iDef = 0;
	m_iLife = 0;
	m_fSpeed = 0.f;
	m_iGold = 0;
	m_iLevel = 0;
	m_dExp = 0;
}

// Unit �˻���ư
void CUnitTool::OnBnClickedSearch()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strSearch);
	if (m_mapUnitData.end() == iter_find)
		return;

	int iIndex = m_listBox.FindString(-1, m_strSearch);

	m_listBox.SetCurSel(iIndex);

	OnLbnSelchangeUnitData();

	UpdateData(FALSE);
}

// ���� �÷��̾��ư
void CUnitTool::OnBnClickedRadioPlayer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	for (int i = 0; i < UNIT_DATA::ID::END; ++i)
		m_RadioID[i].SetCheck(FALSE);
	m_RadioID[UNIT_DATA::ID::PLAYER].SetCheck(TRUE);

	for (int i = 0; i < INFO::END; ++i)
		m_EditDataInfo[i].EnableWindow(TRUE);

	m_eID = UNIT_DATA::PLAYER;

	UpdateData(FALSE);
}

// ���� ���͹�ư
void CUnitTool::OnBnClickedRadioMonster()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	for (int i = 0; i < UNIT_DATA::ID::END; ++i)
		m_RadioID[i].SetCheck(FALSE);
	m_RadioID[UNIT_DATA::ID::MONSTER].SetCheck(TRUE);

	for (int i = 0; i < INFO::END; ++i)
		m_EditDataInfo[i].EnableWindow(TRUE);

	m_EditDataInfo[INFO::MP].EnableWindow(FALSE);
	m_EditDataInfo[INFO::LIFE].EnableWindow(FALSE);
	m_EditDataInfo[INFO::LEVEL].EnableWindow(FALSE);

	m_eID = UNIT_DATA::MONSTER;
	m_iMp = 0;
	m_iLife = 0;
	m_iLevel = 0;

	UpdateData(FALSE);
}

// ���� �����۹�ư
void CUnitTool::OnBnClickedRadioItem()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	for (int i = 0; i < UNIT_DATA::ID::END; ++i)
		m_RadioID[i].SetCheck(FALSE);
	m_RadioID[UNIT_DATA::ID::ITEM].SetCheck(TRUE);

	for (int i = 0; i < INFO::END; ++i)
		m_EditDataInfo[i].EnableWindow(TRUE);

	m_EditDataInfo[INFO::MP].EnableWindow(FALSE);
	m_EditDataInfo[INFO::LIFE].EnableWindow(FALSE);
	m_EditDataInfo[INFO::EXP].EnableWindow(FALSE);

	m_eID = UNIT_DATA::ITEM;
	m_iMp = 0;
	m_iLife = 0;
	m_dExp = 0;

	UpdateData(FALSE);
}

// ���� �ǹ�ư Ŭ��
void CUnitTool::OnTcnSelchangeUnitListTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	
	UpdateData(TRUE);
	int iTabIndex = m_Tab.GetCurSel();

	m_listBox.ResetContent();	// ���� ����Ʈ�ڽ��� ��� ���.
	auto& iter_begin = m_mapUnitData.begin();
	auto& iter_end = m_mapUnitData.end();
	switch (iTabIndex)
	{
	case 0:	//all
		for (int i = 0; i < INFO::END; ++i)
			m_EditDataInfo[i].EnableWindow(false);

		for (int i = 0; i < UNIT_DATA::ID::END; ++i)
		{
			m_RadioID[i].EnableWindow(TRUE);
			m_RadioID[i].SetCheck(FALSE);
		}

		for (; iter_begin != iter_end; ++iter_begin)
			m_listBox.AddString(iter_begin->second->strName);

		CRenderMgr::Get_Instance()->Set_CurRenderID(UNIT_DATA::END);
		break;
	case 1:	//Player
		OnBnClickedRadioPlayer();

		for (int i = 0; i < UNIT_DATA::ID::END; ++i)
			m_RadioID[i].EnableWindow(FALSE);
		m_RadioID[UNIT_DATA::ID::PLAYER].EnableWindow(TRUE);

		for (; iter_begin != iter_end; ++iter_begin)
		{
			if (UNIT_DATA::ID::PLAYER == iter_begin->second->eID)
				m_listBox.AddString(iter_begin->second->strName);
		}

		CRenderMgr::Get_Instance()->Set_CurRenderID(UNIT_DATA::PLAYER);
		break;
	case 2:		//Monster
		OnBnClickedRadioMonster();

		for (int i = 0; i < UNIT_DATA::ID::END; ++i)
			m_RadioID[i].EnableWindow(FALSE);
		m_RadioID[UNIT_DATA::ID::MONSTER].EnableWindow(TRUE);

		for (; iter_begin != iter_end; ++iter_begin)
		{
			if (UNIT_DATA::ID::MONSTER == iter_begin->second->eID)
				m_listBox.AddString(iter_begin->second->strName);
		}

		CRenderMgr::Get_Instance()->Set_CurRenderID(UNIT_DATA::MONSTER);
		break;
	case 3:		//Item
		OnBnClickedRadioItem();

		for (int i = 0; i < UNIT_DATA::ID::END; ++i)
			m_RadioID[i].EnableWindow(FALSE);
		m_RadioID[UNIT_DATA::ID::ITEM].EnableWindow(TRUE);

		for (; iter_begin != iter_end; ++iter_begin)
		{
			if (UNIT_DATA::ID::ITEM == iter_begin->second->eID)
				m_listBox.AddString(iter_begin->second->strName);
		}

		CRenderMgr::Get_Instance()->Set_CurRenderID(UNIT_DATA::ITEM);
		break;
	default:
		break;
	}
	DataReset();				// ������ ����
	UpdateData(FALSE);
}
