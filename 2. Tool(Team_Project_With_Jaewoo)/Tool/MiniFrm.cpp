// MiniFrm.cpp : 구현 파일입니다.
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


// CMiniFrm 메시지 처리기입니다.


BOOL CMiniFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	m_MainSplitt.CreateStatic(this, 2, 1);			//this가 프레임윈도우

													//분할된 영역에 view를 배치 (2개 배치)
													//뷰 클래스들은 생성자가 protected로 되어 있으므로 마음대로 동적할당 불가능하다.
													//-> RUNTIME_CLASS 매크로를 사용.
	m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, WINCY / 2), pContext);
	m_MainSplitt.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY / 2), pContext);

	return TRUE;

}
