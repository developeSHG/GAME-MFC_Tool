
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "TileMgr.h"
#include "MyTimeMgr.h"
#include "RenderMgr.h"
#include "MyForm.h"
#include "MapTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	
	m_bIsSelectMonster = false;
	m_pSelectMonster = nullptr;
	m_bIsPosMoveActive = false;
	m_pPosMoveMonster = nullptr;
	m_bIsSelectUI = false;
	m_pSelectUI = nullptr;
	m_bIsPosUiMoveActive = false;
	m_bWallTileActive = false;
	CRenderMgr::Get_Instance()->Set_MapMosterInfo(&m_mapMonsterInfo);
}

CToolView::~CToolView()
{
	

	for (auto& MyMap : m_mapMonsterInfo)
	{
		for (size_t i = 0; i < MyMap.second.size(); ++i)
		{
			SAFE_DELETE(MyMap.second[i]);
		}
		MyMap.second.clear();
	}
	m_mapMonsterInfo.clear();


	CMyTimeMgr::Get_Instance()->Destroy_Instance();
	CRenderMgr::Get_Instance()->Destroy_Instance();
	CTileMgr::Get_Instance()->Destroy_Instance();
	CMyTextureMgr::Get_Instance()->Destroy_Instance();
	CMFCDeviceMgr::Get_Instance()->Destroy_Instance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//��ũ�� ũ�� ����

	CScrollView::SetScrollSizes(MM_TEXT, CSize(TILE_X * TILECX, TILE_Y * TILECY / 2));

	//ȭ�� ũ�� ����
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());


	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is nullptr");

	// GetWindowRect�Լ�: �׵θ��� ������ ��ü ������ ��Ʈ�� ����. �� �� ��ǥ�� ��ũ�� ��ǥ ����.
	// GetClientRect�Լ�: �׵θ��� �������� Ŭ���̾�Ʈ ������ ����. �� �� ��ǥ�� Ŭ���̾�Ʈ ��ǥ ����.
	RECT rcFrame = {};
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	// ���� ������������� ���� x, y ����.
	int iCX = rcFrame.right - rcView.right;
	int iCY = rcFrame.bottom - rcView.bottom;

	// FrameWnd ��ġ, ũ�� ������.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iCX, WINCY + iCY, SWP_NOZORDER);

	HRESULT hr = 0;
	
	CMFCDeviceMgr::Get_Instance()->DX_Setup(m_hWnd, WINCX, WINCY);
	//// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	CTileMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Set_ToolView(this);

	CMyTextureMgr::Get_Instance()->Initialize();


	m_iTileIndex = 2;
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitt.GetPane(0, 1));
	NULL_CHECK(pMyForm);

	pMyForm->m_MapTool.Render_MiniMap();

	if (CRenderMgr::Get_Instance()->Get_ToolID() == CRenderMgr::MAP)
	{
		if (m_bIsSelectTile)
		{
			CScrollView::OnLButtonDown(nFlags, point);

			if (m_iTileIndex == -1)
				return;

			RECT rc = {};
			GetClientRect(&rc);

			POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
			mousePos = { mousePos.x + CScrollView::GetScrollPos(0), mousePos.y + CScrollView::GetScrollPos(1) };
			/*TCHAR szMessage[MIN_STR] = L"";
			swprintf_s(szMessage, L"%d, %d ���� Ŭ��", mousePos.x, mousePos.y);
			AfxMessageBox(szMessage);*/

			CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
			NULL_CHECK(pFrameWnd);

			CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitt.GetPane(0, 1));
			NULL_CHECK(pMyForm);

			CTileMgr::Get_Instance()->Change_Tile(mousePos, m_iTileIndex, m_bWallTileActive, m_strEventTileActive);
			Invalidate(FALSE);
			pMyForm->m_MapTool.Render_MiniMap();
		}
		else if(m_bIsSelectMonster)
		{
			CScrollView::OnLButtonDown(nFlags, point);

			RECT rc = {};
			GetClientRect(&rc);

			POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
			mousePos = { mousePos.x + CScrollView::GetScrollPos(0), mousePos.y + CScrollView::GetScrollPos(1) };
	
			NULL_CHECK(m_pSelectMonster);
			for (auto& pMonster : m_mapMonsterInfo[m_pSelectMonster->strName])
			{
				if (!FAILED(Collision_Check(pMonster->vPos, mousePos)))
				{
					m_bIsPosMoveActive = true;
					m_pPosMoveMonster = pMonster;
					return;
				}
			}
			if (false == m_bIsPosMoveActive)
			{
				MONSTER_POS* pPos = new MONSTER_POS;
				pPos->vPos = D3DXVECTOR4{ (float)mousePos.x - M_JELLY_CX / 2.f, (float)mousePos.y - M_JELLY_CY / 2.f, 0.f, 0.f };
				m_mapMonsterInfo[m_pSelectMonster->strName].emplace_back(pPos);
			}
		}
	}
	else if (CRenderMgr::Get_Instance()->Get_ToolID() == CRenderMgr::UI)
	{
		CScrollView::OnLButtonDown(nFlags, point);

		RECT rc = {};
		GetClientRect(&rc);

		POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
		mousePos = { mousePos.x, mousePos.y };

		NULL_CHECK(m_pSelectUI);
		for (auto& MyUI : *m_pMapUIInfo)
		{
			if (m_pSelectUI->strName == MyUI.second->strName)
			{
				/*UI_POS* pPos = new UI_POS;
				pPos->vPos = D3DXVECTOR4{ (float)mousePos.x, (float)mousePos.y, 0.f, 0.f };
				pPos->strName = m_pSelectUI->strName;

				SAFE_DELETE(MyUI.second);
				MyUI.second = pPos;*/

				int iUiType = 0;
				if (MyUI.second->strName == L"UI_CHARACTER")
					iUiType = 1;
				else if (MyUI.second->strName == L"UI_SKILLBAR")
					iUiType = 2;
					
				if (!FAILED(Collision_UI_Check(MyUI.second->vPos, mousePos, iUiType)))
				{
					m_bIsPosUiMoveActive = true;
					m_pSelectUI = MyUI.second;
					return;
				}
				else
				{
					if (false == m_bIsPosUiMoveActive)
					{			
						MyUI.second->vPos.x = mousePos.x;
						MyUI.second->vPos.y = mousePos.y;
					}
				}
			}
		}
	}
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnRButtonDown(nFlags, point);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitt.GetPane(0, 1));
	NULL_CHECK(pMyForm);

	pMyForm->m_MapTool.Render_MiniMap();

	RECT rc = {};
	GetClientRect(&rc);

	POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
	mousePos = { mousePos.x + CScrollView::GetScrollPos(0), mousePos.y + CScrollView::GetScrollPos(1) };

	NULL_CHECK(m_pSelectMonster);
	
	auto& iter_begin = m_mapMonsterInfo[m_pSelectMonster->strName].begin();
	
	for (; iter_begin != m_mapMonsterInfo[m_pSelectMonster->strName].end();)
	{
		if (!FAILED(Collision_Check((*iter_begin)->vPos, mousePos)))
		{
			SAFE_DELETE(*iter_begin);
			iter_begin = m_mapMonsterInfo[m_pSelectMonster->strName].erase(iter_begin);
		}
		else
			++iter_begin;
	}
}

void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonUp(nFlags, point);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitt.GetPane(0, 1));
	NULL_CHECK(pMyForm);

	pMyForm->m_MapTool.Render_MiniMap();

	if (true == m_bIsPosMoveActive)
	{
		RECT rc = {};
		GetClientRect(&rc);

		POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
		mousePos = { mousePos.x + CScrollView::GetScrollPos(0), mousePos.y + CScrollView::GetScrollPos(1) };

		m_bIsPosMoveActive = false;

		NULL_CHECK(m_pPosMoveMonster);
		m_pPosMoveMonster->vPos.x = mousePos.x - M_JELLY_CX / 2.f;
		m_pPosMoveMonster->vPos.y = mousePos.y - M_JELLY_CY / 2.f;
	}

	if (true == m_bIsPosUiMoveActive)
	{
		RECT rc = {};
		GetClientRect(&rc);

		POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
		mousePos = { mousePos.x, mousePos.y };

		m_bIsPosUiMoveActive = false;

		NULL_CHECK(m_pSelectUI);
		m_pSelectUI->vPos.x = mousePos.x;
		m_pSelectUI->vPos.y = mousePos.y;
	}
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);

	RECT rc = {};
	GetClientRect(&rc);

	if (true == m_bIsPosMoveActive)
	{
		POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
		mousePos = { mousePos.x + CScrollView::GetScrollPos(0), mousePos.y + CScrollView::GetScrollPos(1) };

		NULL_CHECK(m_pPosMoveMonster);
		m_pPosMoveMonster->vPos.x = mousePos.x - M_JELLY_CX / 2.f;
		m_pPosMoveMonster->vPos.y = mousePos.y - M_JELLY_CY / 2.f;
		return;
	}

	if (true == m_bIsPosUiMoveActive)
	{
		POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
		mousePos = { mousePos.x, mousePos.y };

		NULL_CHECK(m_pSelectUI);
		m_pSelectUI->vPos.x = mousePos.x;
		m_pSelectUI->vPos.y = mousePos.y;
		return;
	}

	

	if (m_iTileIndex == -1)
		return;

	if (CRenderMgr::Get_Instance()->Get_ToolID() == CRenderMgr::MAP)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			CScrollView::SetScrollSizes(MM_TEXT, CSize(TILE_X * TILECX + (rc.right - WINCX), TILE_Y * TILECY / 2 + (rc.bottom - WINCY)));

			POINT mousePos = { point.x * WINCX / rc.right, point.y * WINCY / rc.bottom };
			mousePos = { mousePos.x + CScrollView::GetScrollPos(0), mousePos.y + CScrollView::GetScrollPos(1) };

			CTileMgr::Get_Instance()->Change_Tile(mousePos, m_iTileIndex, m_bWallTileActive, m_strEventTileActive);
			Invalidate(FALSE);
		}
	}
	
}

void CToolView::Render_Unit()
{
	
}

HRESULT CToolView::Collision_Check(D3DXVECTOR4 _vPos, CPoint _point)
{
	RECT rc = {
		_vPos.x,
		_vPos.y ,
		_vPos.x + M_JELLY_CX,
		_vPos.y + M_JELLY_CY,
	};

	if (_point.x >= rc.left 
		&& _point.x <= rc.right
		&& _point.y >= rc.top
		&& _point.y <= rc.bottom)
	{
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CToolView::Collision_UI_Check(D3DXVECTOR4 _vPos, CPoint _point, int _iUiType)
{
	RECT rc = {};
	switch (_iUiType)
	{
	case 1:
		rc = {
			LONG(_vPos.x),
			LONG(_vPos.y),
			LONG(_vPos.x + 328),
			LONG(_vPos.y + 80),
		};
		break;
	case 2:
		rc = {
			LONG(_vPos.x),
			LONG(_vPos.y),
			LONG(_vPos.x + 640),
			LONG(_vPos.y + 160),
		};
		break;
	}

	if (_point.x >= rc.left
		&& _point.x <= rc.right
		&& _point.y >= rc.top
		&& _point.y <= rc.bottom)
	{
		return S_OK;
	}

	return E_FAIL;
}

