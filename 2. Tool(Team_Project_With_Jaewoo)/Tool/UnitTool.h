#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CUnitTool 대화 상자입니다.

class CAnimationTool;
class CMapTool;
class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	enum INFO
	{
		NAME, HP, MP, ATT, DEF, LIFE, SPEED, GOLD, LEVEL, EXP, END
	};

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnLbnSelchangeUnitData();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSearch();

	afx_msg void OnBnClickedRadioPlayer();
	afx_msg void OnBnClickedRadioMonster();
	afx_msg void OnBnClickedRadioItem();
	afx_msg void OnTcnSelchangeUnitListTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void Load_Data();
	void Set_AnimTool(CAnimationTool* pTool) { m_pAnimTool = pTool; }
	void DataReset();

	void Set_MapTool(CMapTool* pMapTool) { m_pMapTool = pMapTool; }
	CMapTool* m_pMapTool;
public:
	// Control Value
	CListBox m_listBox;
	CTabCtrl m_Tab;
	CEdit m_EditDataInfo[INFO::END];
	CButton m_RadioID[UNIT_DATA::ID::END];
	CStatic m_StaticUnitlist;
	CStatic m_StaticName;
	CStatic m_Stats;

public:
	CFont		m_Font;


public:
	// User Value
	CAnimationTool*				m_pAnimTool;
	map<CString, UNIT_DATA*>	m_mapUnitData;

	UNIT_DATA::ID m_eID;		// ID
	CString		m_strName;		// 이름
	UINT		m_iHp;			// Hp
	UINT	    m_iMp;			// Mp
	UINT		m_iAtt;			// 공격력
	UINT		m_iDef;			// 방어력
	UINT		m_iLife;		// 라이프
	float		m_fSpeed;		// 스피드
	UINT		m_iGold;		// 돈
	UINT		m_iLevel;		// 레벨
	double		m_dExp;			// 경험치
	CString		m_strSearch;	// 검색하려는 문자열

};
