#include "stdafx.h"
#include "MyTimeMgr.h"

IMPLEMENT_SINGLETON(CMyTimeMgr)

CMyTimeMgr::CMyTimeMgr()
	: m_fDeltaTime(0.f)
{
}


CMyTimeMgr::~CMyTimeMgr()
{
}

float CMyTimeMgr::GetDeltaTime() const
{
	return m_fDeltaTime;
}

void CMyTimeMgr::InitTime()
{
	// ���κ���(�ϵ����)�� ���� �ִ� ���ػ� Ÿ�̸� ���� ������ �Լ�.
	// �� Ÿ�̸� ���� CPU�� �������� ���� ����ȴ�. Ÿ�̸� �� = CPU ���� ������
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU�� �ʴ� �������� ������ �Լ�.
	QueryPerformanceFrequency(&m_CpuTick);
}

void CMyTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuTick.QuadPart)
		QueryPerformanceFrequency(&m_CpuTick);

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime.QuadPart = m_CurTime.QuadPart;
}

void CMyTimeMgr::Initialize()
{
}
