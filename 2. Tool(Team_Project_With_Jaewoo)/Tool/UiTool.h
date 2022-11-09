#pragma once
#include "afxwin.h"


// CUiTool 대화 상자입니다.

class CUiTool : public CDialog
{
	DECLARE_DYNAMIC(CUiTool)

public:
	CUiTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUiTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	void Initialize();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

	UI_POS* m_pSelectUI;

	map<CString, UI_POS*> m_mapUiPos;
	afx_msg void OnLbnSelchangeList();
};
