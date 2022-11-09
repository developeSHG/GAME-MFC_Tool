#pragma once
#include "MyTexture.h"
class CMySingleTexture :
	public CMyTexture
{
public:
	explicit CMySingleTexture();

public:
	virtual ~CMySingleTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"",
		const int& iIndex = 0) override;

public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT LoadTexture(
		const wstring & wstrFilePath,
		const wstring & wstrStateKey = L"",
		const int & iCount = 0) override;
	virtual void Release() override;

private:
	TEX_INFO*	m_pTexInfo;
};

