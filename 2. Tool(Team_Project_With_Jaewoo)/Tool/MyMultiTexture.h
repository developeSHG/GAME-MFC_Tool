#pragma once
#include "MyTexture.h"
class CMyMultiTexture :
	public CMyTexture
{
public:
	CMyMultiTexture();
	virtual ~CMyMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual const TEX_INFO* GetTexInfo(
		const wstring & wstrStateKey = L"",
		const int & iIndex = 0) override;

public:
	virtual HRESULT LoadTexture(
		const wstring & wstrFilePath,
		const wstring & wstrStateKey = L"",
		const int & iCount = 0) override;
	virtual void Release() override;

private:
	//map<wstring, vec>
	MAP_MULTITEX	m_mapMultiTex;


public:
	map<const wstring, VEC_TEXINFO>* Get_mapMultiTex() { return &m_mapMultiTex; }
};

