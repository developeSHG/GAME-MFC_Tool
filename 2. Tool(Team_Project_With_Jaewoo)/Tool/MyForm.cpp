// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "RenderMgr.h"
// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeToolTab)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.



void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Font.CreatePointFont(180, L"궁서");

	// 현재 대화상자에 배치된 오브젝트를 얻어오는 함수.
	
	if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);

	if (nullptr == m_AnimTool.GetSafeHwnd())
		m_AnimTool.Create(IDD_ANIMATIONTOOL);
	
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);

	if (nullptr == m_UiTool.GetSafeHwnd())
		m_UiTool.Create(IDD_UITOOL);

	//#1
	m_UnitTool.Set_AnimTool(&m_AnimTool);

	//#2
	m_UnitTool.Set_MapTool(&m_MapTool);
	m_AnimTool.Set_UnitPtr(&m_UnitTool.m_mapUnitData);
	m_AnimTool.Initialize();

	m_MapTool.Set_UnitTool(&m_UnitTool);
	m_MapTool.Update_Unit();

	m_UiTool.Initialize();


	CString tab1 = L"Unit Tool";
	CString tab2 = L"Animation Tool";
	CString tab3 = L"Map Tool";
	CString tab4 = L"UI Tool";

	m_Tab.InsertItem(1, tab1);
	m_Tab.InsertItem(2, tab2);
	m_Tab.InsertItem(3, tab3);
	m_Tab.InsertItem(4, tab4);

	m_UnitTool.ShowWindow(SW_SHOW);
}




void CMyForm::OnTcnSelchangeToolTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int iTabIndex = m_Tab.GetCurSel();

	switch (iTabIndex)
	{
	case 0:
		m_UnitTool.ShowWindow(SW_SHOW);
		m_AnimTool.ShowWindow(SW_HIDE);
		m_MapTool.ShowWindow(SW_HIDE);
		m_UiTool.ShowWindow(SW_HIDE);
		CRenderMgr::Get_Instance()->Set_ToolID(CRenderMgr::UNIT);
		//4개다 SW_HIDE
		break;
	case 1:
		m_UnitTool.ShowWindow(SW_HIDE);
		m_AnimTool.ShowWindow(SW_SHOW);
		m_MapTool.ShowWindow(SW_HIDE);
		m_UiTool.ShowWindow(SW_HIDE);
		CRenderMgr::Get_Instance()->Set_ToolID(CRenderMgr::ANIMATION);
		m_AnimTool.Render_Texture();
		break;
	case 2:
		m_UnitTool.ShowWindow(SW_HIDE);
		m_AnimTool.ShowWindow(SW_HIDE);
		m_MapTool.ShowWindow(SW_SHOW);
		m_UiTool.ShowWindow(SW_HIDE);
		CRenderMgr::Get_Instance()->Set_ToolID(CRenderMgr::MAP);
		m_MapTool.Render_MiniMap();
		break;
	case 3:
		m_UnitTool.ShowWindow(SW_HIDE);
		m_AnimTool.ShowWindow(SW_HIDE);
		m_MapTool.ShowWindow(SW_HIDE);
		m_UiTool.ShowWindow(SW_SHOW);
		CRenderMgr::Get_Instance()->Set_ToolID(CRenderMgr::UI);
		break;
	default:
		break;
	}
}
