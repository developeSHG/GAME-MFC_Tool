
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


public:
	void Set_TileIndex(int index) { m_iTileIndex = index; }
	void Render_Unit();

public:
	void Set_IsSelectMonster(bool _b) { m_bIsSelectMonster = _b; }
	void Set_SelectMonster(UNIT_DATA* pData) { m_pSelectMonster = pData; }
	HRESULT Collision_Check(D3DXVECTOR4 _vPos, CPoint _point);
	HRESULT Collision_UI_Check(D3DXVECTOR4 _vPos, CPoint _point, int _iUiType);

public:
	map<CString, vector<MONSTER_POS*>> m_mapMonsterInfo;
	int m_iTileIndex;
	UNIT_DATA* m_pSelectMonster;
	bool m_bIsSelectMonster;
	bool m_bIsSelectTile;
	bool m_bIsPosMoveActive;
	MONSTER_POS* m_pPosMoveMonster;

public:
	void Set_MapUIPos(map<CString, UI_POS*>* pUI) { m_pMapUIInfo = pUI; }
	map<CString, UI_POS*>* m_pMapUIInfo;
	UI_POS* m_pSelectUI;
	bool m_bIsSelectUI;
	bool m_bIsPosUiMoveActive;
	bool m_bWallTileActive;
	TCHAR m_strEventTileActive[MIN_STR] = L"";
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

