#pragma once


// CMiniFrm �������Դϴ�.

class CMiniFrm : public CFrameWnd
{
	DECLARE_DYNCREATE(CMiniFrm)
protected:
	CMiniFrm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMiniFrm();

protected:
	DECLARE_MESSAGE_MAP()


private:
	CSplitterWnd m_MainSplitt;	//���� ������
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


