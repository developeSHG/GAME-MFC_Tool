
// Tool.h : Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CToolApp:
// �� Ŭ������ ������ ���ؼ��� Tool.cpp�� �����Ͻʽÿ�.
//

class CToolApp : public CWinApp
{
public:
	CToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CToolApp theApp;




//1. ������
// Apply ����
//���̶� �̺�Ʈ �ؽ����߰�
//(�̴ϸʿ����߰�)
//�̴ϸ��Լ����Ǹ����ߴ�
//
//2. ���� ���̺�ε�
//
//3. �����Ѱ�