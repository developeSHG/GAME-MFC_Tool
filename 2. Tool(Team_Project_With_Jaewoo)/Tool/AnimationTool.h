#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAnimationTool 대화 상자입니다.

class CAnimationTool : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Set_UnitPtr(map<CString, UNIT_DATA*>* _pMap) { m_pMapUnitData = _pMap; }
	void Initialize();
	void Add_ListBox(CString unitName);
	void Delete_ListBox(CString unitName);

public:
	map<CString, UNIT_DATA*>*	m_pMapUnitData;
	CListBox m_UnitList;

	afx_msg void OnLbnSelUnitChange();
	CListBox m_StateList;
	CListBox m_DirList;
	CListBox m_KeyList;

public:
	void Render_Texture();


public:
	bool m_bDirCheck;
	bool m_bStateCheck;
	afx_msg void OnLbnSelStateList();
	afx_msg void OnLbnSelDirList();
	CString m_strUnit;
	CString m_strState;
	CString m_strDir;
	CString m_strSearch;
	afx_msg void OnBnClickedSearch();
	CStatic m_PictureCtrl;

	CString m_TextureKey;

	void Set_FrameIndex(float _fCurFrame);
	void Set_SelectIndex(int i) { m_iSelectIndex = i;  }


public:
	int m_iFrameStart;
	int m_iFrameEnd;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	float m_fFrameSpeed;
	afx_msg void OnEnChangeSpeed();
	CString m_strCurrentFrame;
	CString m_strTotalFrame;
	CString m_strFrameSpeed;
	CSliderCtrl m_SliderCtrl;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool m_bIsStop;
	afx_msg void OnLbnSelKeyList();

	int m_iSelectIndex;

	typedef map<CString, ANIM_DATA*> ANIM_INFO;
	map<CString, ANIM_INFO>	m_MapAnimation;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedModify();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
