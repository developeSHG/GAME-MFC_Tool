// MiniFrm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniFrm.h"
#include "MiniView.h"
#include "MyForm.h"


// CMiniFrm

IMPLEMENT_DYNCREATE(CMiniFrm, CFrameWnd)

CMiniFrm::CMiniFrm()
{

}

CMiniFrm::~CMiniFrm()
{
}


BEGIN_MESSAGE_MAP(CMiniFrm, CFrameWnd)
END_MESSAGE_MAP()


// CMiniFrm �޽��� ó�����Դϴ�.


BOOL CMiniFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	m_MainSplitt.CreateStatic(this, 2, 1);			//this�� ������������

													//���ҵ� ������ view�� ��ġ (2�� ��ġ)
													//�� Ŭ�������� �����ڰ� protected�� �Ǿ� �����Ƿ� ������� �����Ҵ� �Ұ����ϴ�.
													//-> RUNTIME_CLASS ��ũ�θ� ���.
	m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, WINCY / 2), pContext);
	m_MainSplitt.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY / 2), pContext);

	return TRUE;

}
