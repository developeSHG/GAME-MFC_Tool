#pragma once


// CMiniFrm 프레임입니다.

class CMiniFrm : public CFrameWnd
{
	DECLARE_DYNCREATE(CMiniFrm)
protected:
	CMiniFrm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMiniFrm();

protected:
	DECLARE_MESSAGE_MAP()


private:
	CSplitterWnd m_MainSplitt;	//분할 윈도우
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


