
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

