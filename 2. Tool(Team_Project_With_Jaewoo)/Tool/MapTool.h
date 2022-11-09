#pragma once
#include "afxwin.h"


// CMapTool 대화 상자입니다.
class CUnitTool;

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:	
	//virtual
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

public:		
	//Message
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeTileIndex();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:
	int Get_TileIndex() { return m_iTileIndex; }

public:
	void Create_HorizontalScroll();
	void Render_MiniMap();

public:
	// Message
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnStnClickedWall();
	afx_msg void OnBnClickedCheckBoxWall();
	afx_msg void OnBnClickedApply();

public:
	void Set_UnitTool(CUnitTool* pUnit) { m_pUnitTool = pUnit; }
	void Update_Unit();

public:	    
	// Control Value
	CListBox m_ListBox;
	CStatic m_PictureCtrl;
	CStatic m_PictureCtrl_Minimap;
	CStatic m_MonsterPicCtrl;
	CComboBox m_ComboBox;
	CButton m_CheckBoxWall;
	CStatic m_StaticWall;

public:
	// User Value
	CUnitTool* m_pUnitTool;
	UNIT_DATA* m_pSelectUnit;
	CString m_strEvent;
	int m_iTileIndex;
};
