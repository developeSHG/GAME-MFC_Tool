#pragma once
#include "afxwin.h"


// CUiTool ��ȭ �����Դϴ�.

class CUiTool : public CDialog
{
	DECLARE_DYNAMIC(CUiTool)

public:
	CUiTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUiTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


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
