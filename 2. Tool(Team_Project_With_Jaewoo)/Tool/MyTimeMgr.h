#pragma once
class CMyTimeMgr
{
	DECLARE_SINGLETON(CMyTimeMgr)

private:
	CMyTimeMgr();
	~CMyTimeMgr();

public:
	float GetDeltaTime() const;

public:
	void InitTime();
	void UpdateTime();
	void Initialize();

private:
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CpuTick;

	float			m_fDeltaTime;
};

