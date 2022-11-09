#pragma once



// CMyForm 폼 뷰입니다.

#include "UnitTool.h"
#include "MapTool.h"
#include "AnimationTool.h"
#include "UiTool.h"
#include "afxcmn.h"
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTcnSelchangeToolTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	virtual void OnInitialUpdate();

	CFont	  m_Font;
	CTabCtrl m_Tab;
public:
	CUnitTool			m_UnitTool;
	CAnimationTool		m_AnimTool;
	CMapTool			m_MapTool;
	CUiTool				m_UiTool;
};


